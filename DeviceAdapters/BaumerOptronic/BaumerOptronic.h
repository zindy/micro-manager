/*
 * Baumer Optronic adapter for Leica Cameras.
 *
 * AUTHOR:
 * Karl Hoover
 *
 * Copyright (c) 2006 Regents of the University of California
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

#ifndef _BAUMEROPTRONIC_H_
#define _BAUMEROPTRONIC_H_

#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ImgBuffer.h"
#include "../../MMDevice/DeviceThreads.h"

#pragma warning(push)
#pragma warning(disable: 4245)
#include "FxApi.h"
#include "FxError.h"
#pragma warning(pop)

#include <string>
#include <map>
#include <vector>


enum WorkerCommand {
   Noop = 0,
   InitializeLibrary = 10,
   StartSequence = 20,
   SnapCommand = 30,
   StopSequence = 40,
   Exit = 999
};
enum WorkerState {
   Idle = 0,
   Ready = 10,
   Acquiring = 20,
   Snapping = 30,
   Busy = 777,
   Exiting = 999
};


class BOImplementationThread;


class CBaumerOptronic : public CCameraBase<CBaumerOptronic>
{
public:

   friend class BOImplementationThread;

   CBaumerOptronic();
   ~CBaumerOptronic();

   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();

   void GetName(char* name) const;

   // MMCamera API
   // ------------
   int SnapImage();
   const unsigned char* GetImageBuffer();
   unsigned GetImageWidth() const;
   unsigned GetImageHeight() const;
   unsigned GetImageBytesPerPixel() const;
   unsigned GetBitDepth() const;
   long GetImageBufferSize() const;
   double GetExposure() const;
   void SetExposure(double exp);
   int SetROI(unsigned x, unsigned y, unsigned xSize, unsigned ySize);
   int GetROI(unsigned& x, unsigned& y, unsigned& xSize, unsigned& ySize);
   int ClearROI();
   double GetNominalPixelSizeUm() const { return nominalPixelSizeUm_; }
   double GetPixelSizeUm() const { return nominalPixelSizeUm_ * GetBinning(); }
   int GetBinning() const;
   int SetBinning(int binSize);
   int IsExposureSequenceable(bool& isSequenceable) const { isSequenceable = false; return DEVICE_OK; }

   unsigned GetNumberOfComponents() const;

   int StartSequenceAcquisition(long numImages, double interval_ms, bool stopOnOverflow);
   int StopSequenceAcquisition();
   bool IsCapturing();


   // Action handlers
   // ---------------
   int OnBinning(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPixelType(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnBitDepth(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnExposure(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnGain(MM::PropertyBase* pProp, MM::ActionType eAct);


private:
   int ResizeImageBuffer();
   int WaitForImageAndCopyToBuffer();
   int SendImageToCore();

   static const double nominalPixelSizeUm_;
   ImgBuffer img_;
   bool initialized_;
   bool stopOnOverflow_;

   BOImplementationThread* pWorkerThread_;
};


class BOImplementationThread : public MMDeviceThreadBase
{
public:
   BOImplementationThread(CBaumerOptronic* pCamera);
   ~BOImplementationThread();

   int svc();

   void Interval(double intervalMs) { intervalMs_ = intervalMs; }
   double Interval() const { return intervalMs_; }
   void SetLength(long images) { numImages_ = images; }
   long FrameCount() const { return frameCount_; }
   void FrameCount(const long v_a) { frameCount_ = v_a; }
   void Start() { activate(); }

   // a lock for each data section that will be modified or viewed in the other thread
   MMThreadLock stateMachineLock_;

   void TriggerMode(const bool v);
   void ExposureUs(int v);
   void Gain(double v);

   bool TriggerMode()
   {
      return triggerMode_;
   }

   double Gain()
   {
      MMThreadGuard g(stateMachineLock_);
      return quantizedGain_;
   }

   int ExposureUs()
   {
      MMThreadGuard g(stateMachineLock_);
      return exposureUs_;
   }

   void QueryCapabilities();
   void ParseCapabilities();

   std::pair<double, double> GainLimits();
   std::pair<int, int> ExposureLimits();

   std::vector<int> BinSizes() const { return binSizes_; }
   std::vector<int> PossibleBitsInOneColor() const { return possibleBitsInOneColor_; }

   WorkerCommand Command()
   {
      MMThreadGuard g(stateMachineLock_);
      return command_;
   }

   void Command(const WorkerCommand value)
   {
      MMThreadGuard g(stateMachineLock_);
      command_ = value;
   }

   WorkerState CameraState()
   {
      MMThreadGuard g(stateMachineLock_);
      return cameraState_;
   }

   void* CurrentImage(unsigned& xDim, unsigned& yDim,
         int& bitsInOneColor, int& colors, unsigned long& bufSize,
         MMThreadGuard** pImageBufferGuard_a);

   void CurrentImageSize(unsigned& xDim, unsigned& yDim,
         int& bitsInOneColor, int& colors, unsigned long& bufSize);

   bool MonoChrome();
   bool Color();

   int SetBitsInOneColor(const int bits);
   int GetBitsInOneColor() { return bitsInOneColor_; }

   typedef std::pair<std::string, tBoImgFormat> NamedFormat;
   typedef std::vector<tBoImgCode> ImageCodesPerFormat;
   typedef std::vector<tBoImgFilter> ImageFiltersPerFormat;
   std::vector<NamedFormat> formats;

   int BinSize() const;
   void BinSize(const int v);
   std::string GetSDKErrorMessage(int fxcode);
   static MMThreadLock imageReadyLock_s;
   static MMThreadLock imageBufferLock_s;
   void SetROI(const unsigned int x_a, const unsigned int y_a, const unsigned int xSize_a, const unsigned int ySize_a);
   void GetROI(unsigned int & x,unsigned int & y,unsigned int & xSize,unsigned int & ySize);
   void CancelROI();

   typedef struct
   {
      tBoImgFormat f_;
      int formatIndex_;
      std::string name_;
      ImageCodesPerFormat codes_;
      ImageFiltersPerFormat filters_;
   } CompleteFormat;

   std::vector<CompleteFormat> completeFormats_;


private:

   HANDLE acquisitionThread_;
   HANDLE imageNotificationEvent_;

   void CameraState(const WorkerState v)
   {
      MMThreadGuard g(stateMachineLock_);
      cameraState_ = v;
   }

   void Snap();
   void Acquire();

   double intervalMs_;
   long numImages_;
   CBaumerOptronic* pCamera_;
   WorkerState cameraState_;
   WorkerCommand command_;
   bool stopCameraAfterFirstImage_;
   bool triggerMode_;
   void* pBuf_; // raw buffer from camera library
   unsigned long bufSize_;
   void* pColorBuf_; // extra buffer with empty color dithered in for gui
   unsigned long colorBufSize_;

   unsigned xDim_;
   unsigned yDim_;
   int bitsInOneColor_;
   int nPlanes_;
   long frameCount_;

   int BOInitializationSequence();
   void LLogMessage(const char*const pM_a, const bool debugOnly_a = false); // log a message to the mm device
   void LLogMessage(const std::string m_a, const bool debugOnly_a = false); // log a message to the mm device
   MM::MMTime CurrentMMTimeMM(); // MMTime as milliseconds
   double quantizedGain_;
   int exposureUs_;

   std::vector<int> binSizes_;
   std::vector<int> pixelDepths_;

   std::vector<int> possibleBitsInOneColor_;
   std::vector<int> possibleNPlanes_;

   std::map<int, std::string> filters_;

   int BinSizeFromCompleteFormat(std::vector<CompleteFormat>::iterator index) const;
   int BitDepthFromCompleteFormat(std::vector<CompleteFormat>::iterator index);
   tBoImgCode ImageCode();
   int ImageFormat();
   bool ImageFormat(const int v);
   mutable std::vector<CompleteFormat>::iterator completeFormatIter_; // this should always match the format index
   void FindImageFormatInFormatCache(int imageFormatIndex);
   void QueryCameraCurrentFormat();

   RECT roi_;
   bool partialScanMode_; // if true then roi_ will have the ROI, else use the image format
};

#endif //_BAUMEROPTRONIC_H_
