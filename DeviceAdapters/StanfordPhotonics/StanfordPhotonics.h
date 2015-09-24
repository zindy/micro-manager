/*
 * Stanford Photonics MEGA-10 adapter
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, 08/24/2006
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

/*
 * NOTES:
 *            This adapter is unfinished and does not work.
 */

#ifndef _STANFORD_PHOTONICS_H_
#define _STANFORD_PHOTONICS_H_

#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ImgBuffer.h"
#include "../../MMDevice/DeviceUtils.h"
#include <string>
#include <map>

// error codes
#define ERR_BUFFER_ALLOCATION_FAILED 101
#define ERR_INVALID_ROI 103
#define ERR_BOARD_INITIALIZATION_FAILED 104
#define ERR_UNSUPPORTED_CAMERA 105

//////////////////////////////////////////////////////////////////////////////
// Implementation of the MMDevice and MMCamera interfaces
//

//struct MODCNF;

class MegaZ : public CCameraBase<MegaZ>
{
public:
   static MegaZ* GetInstance();
   ~MegaZ();
   
   // MMDevice API
   int Initialize();
   int Shutdown();
   
   void GetName(char* pszName) const;
   bool Busy() {return busy_;}
   
   // MMCamera API
   int SnapImage();
   const unsigned char* GetImageBuffer();
   unsigned GetImageWidth() const {return img_.Width();}
   unsigned GetImageHeight() const {return img_.Height();}
   unsigned GetImageBytesPerPixel() const {return img_.Depth();} 
   long GetImageBufferSize() const {return img_.Width() * img_.Height() * GetImageBytesPerPixel();}
   unsigned GetBitDepth() const;
   double GetExposure() const;
   void SetExposure(double dExp);
   int SetROI(unsigned uX, unsigned uY, unsigned uXSize, unsigned uYSize); 
   int GetROI(unsigned& uX, unsigned& uY, unsigned& uXSize, unsigned& uYSize);
   int ClearROI();

   // action interface
   int OnBinning(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnExposure(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnShutterMode(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPixelType(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnGain(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnEMGain(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnReadoutMode(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnReadoutTime(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnOffset(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnTemperature(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnDriverDir(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   MegaZ();
   int ResizeImageBuffer();

   static MegaZ* instance_;
   static unsigned refCount_;
   ImgBuffer img_;
   bool busy_;
   bool initialized_;
   bool snapInProgress_;

   struct ROI {
      int x;
      int y;
      int xSize;
      int ySize;

      ROI() : x(0), y(0), xSize(0), ySize(0) {}
      ~ROI() {}

      bool isEmpty() {return x==0 && y==0 && xSize==0 && ySize == 0;}
   };

   ROI roi_;
   int binSize_;
   double expMs_;
   std::string driverDir_;
   unsigned short fullFrameX_;
   unsigned short fullFrameY_;
   short* fullFrameBuffer_;
   std::vector<std::string> readoutModes_;
   MODCNF *mod_;		// IC-PCI board module handle
	MODCNF *ammod_;  // IC-PCI board Acquisition sub-module handle
   short frame_;    // frame handle
   int bitspp_;
   int bytespp_;
};

#endif //_STANFORD_PHOTONICS_H_
