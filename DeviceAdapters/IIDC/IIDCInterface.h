/*
 * AUTHOR:
 * Mark A. Tsuchida
 *
 * Copyright (c) 2014 Regents of the University of California
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "IIDCFeature.h"

#include <dc1394/dc1394.h>
#ifdef _MSC_VER
#undef restrict
#endif

#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/utility.hpp>
#include <string>
#include <utility>
#include <vector>


namespace IIDC {

class Camera;
class Capture;
class VideoMode;


enum PixelFormat
{
   PixelFormatUnsupported,
   PixelFormatGray8,
   PixelFormatGray16,
   PixelFormatYUV444,
   PixelFormatYUV422,
   PixelFormatYUV411,
   PixelFormatRGB8,
};


class Interface : boost::noncopyable, public boost::enable_shared_from_this<Interface>
{
   dc1394_t* libdc1394context_;

public:
   Interface();
   ~Interface();

   dc1394_t* GetLibDC1394Context() { return libdc1394context_; }

   std::vector<std::string> GetCameraIDs();
   boost::shared_ptr<Camera> NewCamera(const std::string& idString);
};


/*
 * Note: All methods, including the capture-related methods, must be called
 * from the same thread (or be synchronized externally).
 */
class Camera : boost::noncopyable, public boost::enable_shared_from_this<Camera>
{
public:
   /*
    * Callable type for per-frame callback. The void* parameter (pixel buffer)
    * is read-only and only valid for the duration of the call.
    * The two size_t parameters are width and height.
    */
   typedef boost::function<void (const void*, size_t, size_t, PixelFormat)> FrameCallbackFunction;
   typedef boost::function<void ()> FinishCallbackFunction;

private:
   dc1394camera_t* libdc1394camera_;
   FrameCallbackFunction captureFrameCallback_;
   boost::shared_ptr<Capture> currentCapture_;
   boost::unique_future<void> captureFuture_; // Note: boost::future in more recent versions

public:
   // Use Interface::NewCamera to create instances; do not directly call
   // constructor.
   Camera(boost::shared_ptr<Interface> context, uint64_t guid, int unit);
   ~Camera();

   uint64_t GetGUID() const { return libdc1394camera_->guid; }
   uint16_t GetUnitNo() const { return static_cast<uint16_t>(libdc1394camera_->unit); }
   std::string GetCameraID() const;
   std::string GetIIDCVersion() const;
   std::string GetVendor() const { return libdc1394camera_->vendor; }
   std::string GetModel() const { return libdc1394camera_->model; }
   uint32_t GetVendorID() const { return libdc1394camera_->vendor_id; }
   uint32_t GetModelID() const { return libdc1394camera_->model_id; }
   bool Is1394BCapable() const { return libdc1394camera_->bmode_capable != DC1394_FALSE; }
   bool IsOneShotCapable() const { return libdc1394camera_->one_shot_capable != DC1394_FALSE; }
   bool IsMultiShotCapable() const { return libdc1394camera_->multi_shot_capable != DC1394_FALSE; }
   bool IsPowerSwitchable() const { return libdc1394camera_->can_switch_on_off != DC1394_FALSE; }

   void ResetBus(); // Use sparingly.
   std::pair<uint32_t, uint32_t> Get1394NodeAndGeneration();

   void Enable1394B(bool flag);
   bool Is1394BEnabled();
   void SetIsoSpeed(unsigned nominalMbps); // E.g. 400 or 800.
   unsigned GetIsoSpeed(); // Return nominal value such as 400 or 800.

   std::vector< boost::shared_ptr<VideoMode> > GetVideoModes();
   boost::shared_ptr<VideoMode> GetVideoMode();
   void SetVideoMode(boost::shared_ptr<VideoMode> mode);

   unsigned GetBitsPerSample();

   /*
    * Set the framerate to the maximum possible given the current format, video
    * mode, and (for Format_7) color coding and ROI.
    *
    * The format7NegativeDeltaUnits parameter is an advanced setting that might
    * prevent corrupted images in some cases.
    */
   void SetMaxFramerate(unsigned format7NegativeDeltaUnits = 0);

   /*
    * Get the current framerate (assuming that the FRAME_RATE feature is off).
    * This is the maximum framerate, and the actual framerate may be reduced if
    * the SHUTTER value is long. Units: fps.
    */
   float GetFramerate();

   boost::shared_ptr<BrightnessFeature> GetBrightnessFeature()
   { return boost::make_shared<BrightnessFeature>(shared_from_this(), libdc1394camera_); }
   boost::shared_ptr<ShutterFeature> GetShutterFeature()
   { return boost::make_shared<ShutterFeature>(shared_from_this(), libdc1394camera_); }
   boost::shared_ptr<GainFeature> GetGainFeature()
   { return boost::make_shared<GainFeature>(shared_from_this(), libdc1394camera_); }
   boost::shared_ptr<FrameRateFeature> GetFrameRateFeature()
   { return boost::make_shared<FrameRateFeature>(shared_from_this(), libdc1394camera_); }

   /*
    * Start a standard capture and stop after nrFrames have been retrieved. If
    * nrFrames == 0, continue until StopCapture() is called.
    */
   void StartContinuousCapture(uint32_t nrDMABuffers, size_t nrFrames,
         unsigned firstFrameTimeoutMs, FrameCallbackFunction frameCallback,
         FinishCallbackFunction finishCallback);

   /*
    * Start a multi-shot capture. Camera must be multi-shot capable.
    */
   void StartMultiShotCapture(uint32_t nrDMABuffers, uint16_t nrFrames,
         unsigned firstFrameTimeoutMs, FrameCallbackFunction frameCallback,
         FinishCallbackFunction finishCallback);

   /*
    * Start a single-shot caputre. Camera must be single-shot capable.
    */
   void StartOneShotCapture(uint32_t nrDMABuffers, unsigned timeoutMs,
         FrameCallbackFunction frameCallback,
         FinishCallbackFunction finishCallback);
   /*
    * Stop the capture running in the background. Extra calls are innocuous.
    */
   void StopCapture();

   /*
    * Wait until the capture running in the background finishes. If the capture
    * has not been stopped, it must be for a finite number of frames (or this
    * function will hang).
    */
   void WaitForCapture();

   /*
    * Return true if there is an unfinished capture running in the background.
    */
   bool IsCapturing();

private:
   void EnsureNotCapturing();
   void EnsureReadyForCapture();
   void RunCaptureInBackground(boost::shared_ptr<Capture> capture);
   void HandleCapturedFrame(dc1394video_frame_t* frame);
};

} // namespace IIDC
