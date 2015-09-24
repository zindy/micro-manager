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

#include "DeviceBase.h"

#include "IIDCInterface.h"

#include <boost/thread.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>

#include <utility>
#include <vector>


class MMIIDCHub; // Not a Micro-Manager "Hub" device


class MMIIDCCamera : public CCameraBase<MMIIDCCamera>
{
   boost::shared_ptr<MMIIDCHub> hub_;

   boost::shared_ptr<IIDC::Camera> iidcCamera_;
   std::vector< boost::shared_ptr<IIDC::VideoMode> > videoModes_;

   // Cache the video mode info, because getting it during a capture can hang
   // (observed on OpenSUSE 12.3, libdc1394 2.2.0).
   boost::shared_ptr<IIDC::VideoMode> currentVideoMode_;

   // Unchanging settings (set once from pre-init properties)
   double shutterUsPerUnit_, shutterOffsetUs_;
   bool absoluteGainIsReadOnly_;

   // Cached settings and properties
   unsigned cachedBitsPerSample_; // Depends on video mode
   double cachedFramerate_; // Depends on video mode and Format_7 packet size
   double cachedExposure_; // User settable but may also depend on video mode

   boost::mutex sampleProcessingMutex_;
   bool rightShift16BitSamples_; // Guarded by sampleProcessingMutex_

   bool stopOnOverflow_; // Set by StartSequenceAcquisition(), read by SequenceCallback()

   int nextAdHocErrorCode_;

   /*
    * Keep snapped image in our own buffer
    */
   boost::shared_array<unsigned char> snappedPixels_;
   size_t snappedWidth_, snappedHeight_, snappedBytesPerPixel_;

public:
   MMIIDCCamera();
   virtual ~MMIIDCCamera();

   /*
    * Device methods
    */

   virtual int Initialize();
   virtual int Shutdown();

   virtual bool Busy();
   virtual void GetName(char* name) const;

   /*
    * Camera methods
    */

   virtual int SnapImage();
   virtual const unsigned char* GetImageBuffer() { return GetImageBuffer(0); }
   virtual const unsigned char* GetImageBuffer(unsigned chan);

   // virtual const unsigned int* GetImageBufferAsRGB32(); // TODO
   // virtual unsigned GetNumberOfComponents() const;
   // virtual int GetComponentName(unsigned component, char* name);

   virtual long GetImageBufferSize() const;
   virtual unsigned GetImageWidth() const;
   virtual unsigned GetImageHeight() const;
   virtual unsigned GetImageBytesPerPixel() const;
   virtual unsigned GetNumberOfComponents() const;
   virtual unsigned GetBitDepth() const;

   virtual int GetBinning() const { return 1; }
   virtual int SetBinning(int) { return DEVICE_UNSUPPORTED_COMMAND; }
   virtual void SetExposure(double milliseconds);
   virtual double GetExposure() const { return cachedExposure_; }

   virtual int SetROI(unsigned x, unsigned y, unsigned xSize, unsigned ySize);
   virtual int GetROI(unsigned& x, unsigned& y, unsigned& xSize, unsigned& ySize);
   virtual int ClearROI();

   virtual int StartSequenceAcquisition(long count, double intervalMs, bool stopOnOverflow);
   virtual int StartSequenceAcquisition(double intervalMs)
   { return StartSequenceAcquisition(LONG_MAX, intervalMs, false); }
   virtual int StopSequenceAcquisition();
   virtual bool IsCapturing();

   virtual int IsExposureSequenceable(bool& f) const { f = false; return DEVICE_OK; }

private:
   /*
    * Property action handlers
    */

   int OnMaximumFramerate(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnRightShift16BitSamples(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnFormat7PacketSizeNegativeDelta(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnVideoMode(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnExposure(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnBrightness(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnGain(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnReadOnlyAbsoluteGain(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   /*
    * Internal functions
    */

   int InitializeInformationalProperties();
   int InitializeBehaviorTweakProperties();
   int InitializeVideoMode();
   int InitializeVideoModeDependentState();
   int InitializeFeatureProperties();

   int VideoModeDidChange();

   void SetExposureImpl(double exposure);
   double GetExposureUncached();
   std::pair<double, double> GetExposureLimits();

   void SnapCallback(const void* pixels, size_t width, size_t height, IIDC::PixelFormat format);
   void SequenceCallback(const void* pixels, size_t width, size_t height, IIDC::PixelFormat format);
   void SequenceFinishCallback();

   int AdHocErrorCode(const std::string& message);
};
