/*
 * Sensicam camera module
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, 06/08/2005
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

#ifndef _SENSICAM_H_
#define _SENSICAM_H_

#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ImgBuffer.h"
#include "../../MMDevice/DeviceUtils.h"
#include <string>
#include <map>

#define MMSENSICAM_MAX_STRLEN    50
#define ERR_UNKNOWN_CAMERA_TYPE  11
#define ERR_TIMEOUT              12

// warning code definitions from MA_SCDPHPSDKWINE_515.pdf
//   http://www.pco.de/fileadmin/user_upload/db/download/MA_SCDPHPSDKWINE_515.pdf
// (SDK Software Development Kit Windows 95/98/NT/2000 Version 5.15 		 )

#define	NO_IMAGE_IN_PCI_BUFFER	100
#define	PICTURE_TOO_DARK	101
#define	PICTURE_TOO_BRIGHT	102
#define	ONE_OR_MORE_VALUES_CHANGED	103
#define	BUFFER_FOR_BUILDED_STRING_TOO_SHORT	104

#define IsSensicamWarning(x) ((NO_IMAGE_IN_PCI_BUFFER<=(x))&&((x)<=BUFFER_FOR_BUILDED_STRING_TOO_SHORT))
#define IsSensicamError(x) ((0!=(x))&&(!(IsSensicamWarning(x))))

//////////////////////////////////////////////////////////////////////////////
// Implementation of the MMDevice and MMCamera interfaces
//
class CSensicam : public CCameraBase<CSensicam>
{
public:
   static CSensicam* GetInstance()
   {
      if (!m_pInstance)
         m_pInstance = new CSensicam();
      return m_pInstance;
   }
   ~CSensicam();
   
   // MMDevice API
   int Initialize();
   int Shutdown();
   
   void GetName(char* pszName) const {CDeviceUtils::CopyLimitedString(pszName, "Sensicam");}
   bool Busy() {return m_bBusy;}
   
   // MMCamera API
   int SnapImage();
   const unsigned char* GetImageBuffer();
   unsigned GetImageWidth() const {return img_.Width();}
   unsigned GetImageHeight() const {return img_.Height();}
   unsigned GetImageBytesPerPixel() const {return img_.Depth();} 
   unsigned GetBitDepth() const;
   int GetBinning() const;
   int SetBinning(int binSize);
   long GetImageBufferSize() const {return img_.Width() * img_.Height() * GetImageBytesPerPixel();}
   double GetExposure() const {return m_dExposure;}
   void SetExposure(double dExp);
   int SetROI(unsigned uX, unsigned uY, unsigned uXSize, unsigned uYSize); 
   int GetROI(unsigned& uX, unsigned& uY, unsigned& uXSize, unsigned& uYSize); 
   int ClearROI();
   int StartSequenceAcquisition(long numImages, double /*interval_ms*/, bool stopOnOverflow);
   int StopSequenceAcquisition();
   bool IsCapturing();
   int IsExposureSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}


   // action interface
   int OnBoard(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnCameraType(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnCCDType(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnExposure(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnBinning(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPixelType(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnGain(MM::PropertyBase* pProp, MM::ActionType eAct);

 
   /*
   int OnMode(CPropertyBase* pProp, ActionType eAct);
   int OnSubMode(CPropertyBase* pProp, ActionType eAct);
   int OnTrigger(CPropertyBase* pProp, ActionType eAct);
   int OnRoiXMax(CPropertyBase* pProp, ActionType eAct);
   int OnRoiYMax(CPropertyBase* pProp, ActionType eAct);
   int OnRoiYMin(CPropertyBase* pProp, ActionType eAct);
   int OnHBin(CPropertyBase* pProp, ActionType eAct);
   int OnVBin(CPropertyBase* pProp, ActionType eAct);
   int OnCCDType(CPropertyBase* pProp, ActionType eAct);   
   */

private:
   CSensicam();
   int ResizeImageBuffer();

   class SequenceThread : public MMDeviceThreadBase
   {
      public:
         SequenceThread(CSensicam* pCam) : stop_(false), numImages_(0) {camera_ = pCam;}
         ~SequenceThread() {}

         int svc (void);

         void Stop() {stop_ = true;}

         void Start()
         {
            stop_ = false;
            activate();
         }

         void SetLength(long images) {numImages_ = images;}
		 long GetLength(void) {return numImages_;};

      private:
         CSensicam* camera_;
         bool stop_;
         long numImages_;
   };

   SequenceThread* sthd_;
   bool stopOnOverflow_;

   int InsertImage();


   static CSensicam* m_pInstance;
   ImgBuffer img_;
   int pixelDepth_;
   float pictime_;
   bool sequenceRunning_;


   double m_dExposure; 
   bool m_bBusy;
   bool m_bInitialized;

   // sensicam data
   int m_nBoard;
   int m_nCameraType;
   char m_pszTimes[MMSENSICAM_MAX_STRLEN];
   int m_nTimesLen;
   int m_nSubMode;
   int m_nMode;
   int m_nTrig;
   int m_nRoiXMax;
   int m_nRoiXMin;
   int m_nRoiYMax;
   int m_nRoiYMin;
   int m_nHBin;
   int m_nVBin;
   int m_nCCDType;
   int roiXMaxFull_;
   int roiYMaxFull_;
};

#endif //_SENSICAM_H_
