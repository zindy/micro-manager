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

#include <dc1394/dc1394.h>
#ifdef _MSC_VER
#undef restrict
#endif

#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/utility.hpp>


namespace IIDC {

class Capture : boost::noncopyable
{
public:
   virtual ~Capture() {}

   /*
    * Run the capture, blocking until finished or asynchronously stopped.
    * Must be called from the thread that owns this instance.
    */
   virtual void Run() = 0;

   /*
    * Stop the capture; the frame handler will no longer be called once Stop()
    * has returned. May be called from any thread.
    */
   virtual void Stop() = 0;

   /*
    * Return true if capture is running (not all requested frames have been
    * acquired, and Stop() has not been called). May be called from any thread.
    */
   virtual bool IsRunning() = 0;
};


/*
 * Implementation class for frame retrieval from the DMA ring buffer.
 * This implementation is common to the different types of capture (continuous,
 * one-shot, and multi-shot), so it is factored out into its own class.
 */
class FrameRetriever : boost::noncopyable
{
public:
   typedef boost::function<void (dc1394video_frame_t*)> FrameCallbackFunction;

private:
   dc1394camera_t* libdc1394camera_;

   const size_t requestedFrames_;
   const size_t expectedFrames_;
   size_t retrievedFrames_;
   unsigned firstFrameTimeoutMs_;

   boost::mutex statusMutex_; // Protect started_ and stopped_
   bool started_;
   bool stopped_;

   FrameCallbackFunction frameCallback_;

public:
   /*
    * Once Run() is called, retrieve nrFrames from libdc1394camera, waiting
    * firstFrameTimeoutMs for the first frame to arrive, and call frameCallback
    * with each frame as the argument.
    */
   FrameRetriever(dc1394camera_t* libdc1394camera,
         size_t desiredNrFrames, size_t expectedNrFrames,
         unsigned firstFrameTimeoutMs, FrameCallbackFunction frameCallback);

   /*
    * Run the retrieval cycle, blocking until finished. Must be called from the
    * thread that owns this instance.
    */
   void Run();

   /*
    * Stop the retrieval cycle and ensure that no further calls to
    * frameCallback will occur.
    */
   void Stop();

   /*
    * Return true if Run() has been called, retrieval of the requested frames
    * has not finished, and Stop() has not been called.
    */
   bool IsRunning();

private:
   void RetrieveFrame(unsigned timeoutMs = 0, unsigned pollingIntervalMs = 0);
   dc1394video_frame_t* PollForFrame(unsigned timeoutMs, unsigned pollingIntervalMs);
   dc1394video_frame_t* WaitForFrame();
};


class CaptureImpl : public Capture
{
public:
   typedef FrameRetriever::FrameCallbackFunction FrameCallbackFunction;
   typedef boost::function<void ()> FinishCallbackFunction;

private:
   FrameRetriever retriever_;
   FinishCallbackFunction finishCallback_;

public:
   CaptureImpl(dc1394camera_t* libdc1394camera,
         size_t nrFrames, size_t finiteNrFrames, unsigned firstFrameTimeoutMs,
         FrameCallbackFunction frameHandler,
         FinishCallbackFunction finishHandler) :
      retriever_(libdc1394camera, nrFrames, finiteNrFrames, firstFrameTimeoutMs, frameHandler),
      finishCallback_(finishHandler)
   {}

   virtual void Run();
   virtual void Stop() { retriever_.Stop(); }
   virtual bool IsRunning() { return retriever_.IsRunning(); }

private:
   // Subclasses override these to provide implementation:
   virtual void SetUp() = 0;
   virtual void CleanUp() = 0;

   void Finish()
   {
      if (finishCallback_)
         finishCallback_();
      CleanUp();
   }
};


class ContinuousCapture : public CaptureImpl
{
   dc1394camera_t* libdc1394camera_;
   const uint32_t nrDMABuffers_;

public:
   ContinuousCapture(dc1394camera_t* libdc1394camera,
         uint32_t nrDMABuffers, size_t nrFrames, unsigned firstFrameTimeoutMs,
         FrameCallbackFunction frameHandler,
         FinishCallbackFunction finishHandler) :
      CaptureImpl(libdc1394camera, nrFrames, 0, firstFrameTimeoutMs,
            frameHandler, finishHandler),
      libdc1394camera_(libdc1394camera),
      nrDMABuffers_(nrDMABuffers)
   {}

private:
   virtual void SetUp();
   virtual void CleanUp();
};


class MultiShotCapture : public CaptureImpl
{
   dc1394camera_t* libdc1394camera_;
   const uint32_t nrDMABuffers_;
   uint16_t nrFrames_;

public:
   MultiShotCapture(dc1394camera_t* libdc1394camera,
         uint32_t nrDMABuffers, uint16_t nrFrames, unsigned firstFrameTimeoutMs,
         FrameCallbackFunction frameHandler,
         FinishCallbackFunction finishHandler) :
      CaptureImpl(libdc1394camera, nrFrames, nrFrames, firstFrameTimeoutMs,
            frameHandler, finishHandler),
      libdc1394camera_(libdc1394camera),
      nrDMABuffers_(nrDMABuffers),
      nrFrames_(nrFrames)
   {}

private:
   virtual void SetUp();
   virtual void CleanUp();
};


class OneShotCapture : public CaptureImpl
{
   dc1394camera_t* libdc1394camera_;
   const uint32_t nrDMABuffers_;

public:
   OneShotCapture(dc1394camera_t* libdc1394camera,
         uint32_t nrDMABuffers, unsigned timeoutMs,
         FrameCallbackFunction frameHandler,
         FinishCallbackFunction finishHandler) :
      CaptureImpl(libdc1394camera, 1, 1, timeoutMs,
            frameHandler, finishHandler),
      libdc1394camera_(libdc1394camera),
      nrDMABuffers_(nrDMABuffers)
   {}

private:
   virtual void SetUp();
   virtual void CleanUp();
};

} // namespace IIDC
