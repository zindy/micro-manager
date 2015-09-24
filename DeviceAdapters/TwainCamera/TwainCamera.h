/*
 * generic Twain camera adapter
 *
 * Copyright (c) 2009 Regents of the University of California
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

#ifndef _TWAIN_CAMERA_H_
#define _TWAIN_CAMERA_H_

#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ImgBuffer.h"
#include "../../MMDevice/DeviceThreads.h"
#include <string>
#include <map>


// forward declaration for implementation class
class TwainDevice;
class CameraSequenceThread;

class TwainBad  // exception to throw upon error in Twain device
{
public:

	TwainBad(){};
	#ifdef WIN32
		#pragma warning(disable : 4996)
	#endif
	TwainBad(const char *const ptext):reason_(ptext){ };
	#ifdef WIN32
		#pragma warning(default : 4996)
	#endif
	const char* ReasonText(void){ return reason_.c_str();};
	std::string reason_;

};

//////////////////////////////////////////////////////////////////////////////
// Error codes
//
#define ERR_UNKNOWN_MODE            102
#define ERR_BUSY_ACQIRING           105
#define ERR_UNSUPPORTED_IMAGE_TYPE  106
#define ERR_DEVICE_NOT_AVAILABLE    107

//////////////////////////////////////////////////////////////////////////////
// TwainCamera class
//streaming Camera device
//////////////////////////////////////////////////////////////////////////////
class TwainCamera : public CCameraBase<TwainCamera>  
{
public:
   TwainCamera();
   ~TwainCamera();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   void GetName(char* name) const;      
   bool Busy();
   
   // MMCamera API
   // ------------
   int SnapImage();
   const unsigned char* GetImageBuffer();
   const unsigned int* GetImageBufferAsRGB32();
   unsigned GetNumberOfComponents() const;
   int GetComponentName(unsigned int channel, char* name);
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
   int StartSequenceAcquisition(long numImages, double interval_ms, bool stopOnOverflow);
	int StopSequenceAcquisition();
   double GetNominalPixelSizeUm() const {return nominalPixelSizeUm_;}
   double GetPixelSizeUm() const {return nominalPixelSizeUm_ * GetBinning();}
   int GetBinning() const;
   int SetBinning(int binSize);
   int IsExposureSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}
	int NextTwainImageIntoImageBuffer(ImgBuffer& img);
	int StartTwainCamera(void);

	std::string deviceName_;

	// expose  CDeviceBase accessors, so that PImpl can use them
	MM::MMTime GetCurrentMMTime()
	{
		return CCameraBase<TwainCamera>::GetCurrentMMTime();
	};


private:

   // action interface
   // ----------------
	int OnCamera(MM::PropertyBase* pProp, MM::ActionType eAct);

	int OnVendorSettings(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnBinning(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPixelType(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnReadoutTime(MM::PropertyBase* pProp, MM::ActionType eAct);

   int SetPixelTypesValues();

   //Do necessary for capturing
   //Is called from the thread function
   //Overrides ones defined in the CCameraBase class 
   bool IsCapturing();

   int ThreadRun();
   int PushImage();

   static const double nominalPixelSizeUm_;
   static int imageSizeW_;
   static int imageSizeH_;

   ImgBuffer img_[3];
   bool initialized_;
	bool cameraStarted_;
   bool busy_;
   long readoutUs_;
   MM::MMTime readoutStartTime_;
   bool color_;
   unsigned char* rawBuffer_;
   bool stopOnOverflow_;


   int ResizeImageBuffer(  int imageSizeW = imageSizeW_, 
                           int imageSizeH = imageSizeH_);
   int ResizeImageBuffer(
                           int imageSizeW, 
                           int imageSizeH, 
                           int byteDepth, 
                           int binSize = 1);
	TwainDevice* pTwainDevice_;
	bool stopRequest_;
	

   CameraSequenceThread * thd_;
   friend class CameraSequenceThread;


};


#endif //_TWAIN_CAMERA_H_
