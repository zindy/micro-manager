/*
 * Device adapter for Thorlabs USB cameras DCU223M, DCU223C,
 * DCU224M, DCU224C, DCC1545M, DCC1645C, DCC1240M, DCC1240C.
 * Has been developed and tested with the DCC1545M, based on the
 * source code of the DemoCamera device adapter
 *
 * AUTHOR:
 * Christophe Dupre, christophe.dupre@gmail.com, 09/25/2012
 * Updated to support DC3240C features, Nenad Amodaj, 09/2013
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

#ifndef _DEMOCAMERA_H_
#define _DEMOCAMERA_H_

#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ImgBuffer.h"
#include "../../MMDevice/DeviceThreads.h"
#include <string>
#include <map>
#include <algorithm>
#include "uc480.h"

//////////////////////////////////////////////////////////////////////////////
// Error codes
//
#define ERR_THORCAM_LIVE_TIMEOUT 11001
#define ERR_THORCAM_LIVE_UNKNOWN_EVENT 11002
#define ERR_THORCAM_UNKNOWN_PIXEL_TYPE 11003
#define ERR_THORCAM_UNKNOWN_BIN_SIZE 11004

//////////////////////////////////////////////////////////////////////////////
// ThorlabsUSBCam class
// Simulation of the Camera device
//////////////////////////////////////////////////////////////////////////////

class MySequenceThread;

class ThorlabsUSBCam : public CCameraBase<ThorlabsUSBCam>  
{
public:
   ThorlabsUSBCam();
   ~ThorlabsUSBCam();
  
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
   int PrepareSequenceAcqusition()
   {
      return DEVICE_OK;
   }
   int StartSequenceAcquisition(double interval);
   int StartSequenceAcquisition(long numImages, double interval_ms, bool stopOnOverflow);
   int StopSequenceAcquisition();
   int InsertImage();
   int ThreadRun();
   bool IsCapturing();
   void OnThreadExiting() throw(); 
   int GetBinning() const;
   int SetBinning(int bS);
   int IsExposureSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}

   unsigned  GetNumberOfComponents() const { return nComponents_;};

   // action interface
   // ----------------
   int OnBinning(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPixelType(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnExposure(MM::PropertyBase* , MM::ActionType );
   int OnHardwareGain(MM::PropertyBase* , MM::ActionType );
   int OnPixelClock(MM::PropertyBase* , MM::ActionType );
   int OnFPS(MM::PropertyBase* , MM::ActionType );

private:
   int ResizeImageBuffer();

   ImgBuffer img_;
   bool busy_;
   bool stopOnOverFlow_;
   bool initialized_;
   MM::MMTime readoutStartTime_;
   MM::MMTime sequenceStartTime_;
   int bitDepth_;
   unsigned roiX_;
   unsigned roiY_;
   unsigned roiWidth_;
   unsigned roiHeight_;
   long imageCounter_;
	long binSize_;
	SENSORINFO sensorInfo;
   volatile double framesPerSecond;

   MMThreadLock imgPixelsLock_;
   int nComponents_;
   friend class MySequenceThread;
   MySequenceThread * thd_;

	HCAM	camHandle_;			// handle to camera driver
   char* cameraBuf;        // camera buffer for image transfer
   HANDLE hEvent;          // event handle for camera notifications
   int cameraBufId;        // buffer id, required by the SDK
   double Exposure_;
   double HardwareGain_;
};

class MySequenceThread : public MMDeviceThreadBase
{
   friend class ThorlabsUSBCam;
   enum { default_numImages=1, default_intervalMS = 100 };
   public:
      MySequenceThread(ThorlabsUSBCam* pCam);
      ~MySequenceThread();
      void Stop();
      void Start(long numImages, double intervalMs);
      bool IsStopped();
      void Suspend();
      bool IsSuspended();
      void Resume();
      double GetIntervalMs(){return intervalMs_;}                               
      void SetLength(long images) {numImages_ = images;}                        
      long GetLength() const {return numImages_;}
      long GetImageCounter(){return imageCounter_;}                             
      MM::MMTime GetStartTime(){return startTime_;}                             
      MM::MMTime GetActualDuration(){return actualDuration_;}
   private:                                                                     
      int svc(void) throw();
      ThorlabsUSBCam* camera_;                                                     
      bool stop_;                                                               
      bool suspend_;                                                            
      long numImages_;                                                          
      long imageCounter_;                                                       
      double intervalMs_;                                                       
      MM::MMTime startTime_;                                                    
      MM::MMTime actualDuration_;                                               
      MM::MMTime lastFrameTime_;                                                
      MMThreadLock stopLock_;                                                   
      MMThreadLock suspendLock_;                                                
}; 



#endif //_DEMOCAMERA_H_
