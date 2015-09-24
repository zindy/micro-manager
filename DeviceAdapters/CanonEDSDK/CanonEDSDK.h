/*
 * Adapter for Canon EDSDK
 *
 * AUTHOR:
 * Nico Stuurman
 * License:       LGPL
 *
 * Copyright (c) 2010 USCF
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CANONEDSDK_H_
#define CANONEDSDK_H_

#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ImgBuffer.h"
#include "../../../3rdparty/trunk/Canon/EDSDK2.13/Mac/EDSDK/Header/EDSDKTypes.h"

//////////////////////////////////////////////////////////////////////////////
// Error codes

class CanonEDCamera : public CCameraBase<CanonEDCamera>
{
   public:
      CanonEDCamera();
      ~CanonEDCamera();

      // MMDevice API
      int Initialize();
      int Shutdown();

      void GetName(char* name) const;

      // MMCamera API
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
   int PrepareSequenceAcqusition() {return DEVICE_OK;}                       
   int StartSequenceAcquisition(double interval);                            
   int StartSequenceAcquisition(long numImages, double interval_ms, bool stopOnOverflow);   
   int StopSequenceAcquisition();                                            
   int IsExposureSequenceable(bool& sequenceable) const { sequenceable = false; return DEVICE_OK;};
   int InsertImage();                                                        
  // int ThreadRun();                                                          
   bool IsCapturing();                                                       
 //  void OnThreadExiting() throw();                                           
   double GetNominalPixelSizeUm() const {return nominalPixelSizeUm_;}        
   double GetPixelSizeUm() const {return nominalPixelSizeUm_ * GetBinning();}
   int GetBinning() const;                                                   
   int SetBinning(int bS); 
   unsigned  GetNumberOfComponents() const { return nComponents_;};


   // action interface
   int OnBinning(MM::PropertyBase* pProp, MM::ActionType eAct);              
   int OnPixelType(MM::PropertyBase* pProp, MM::ActionType eAct);            
   int OnBitDepth(MM::PropertyBase* pProp, MM::ActionType eAct);             
   int OnReadoutTime(MM::PropertyBase* pProp, MM::ActionType eAct);          
   int OnCameraCCDXSize(MM::PropertyBase* , MM::ActionType );                
   int OnCameraCCDYSize(MM::PropertyBase* , MM::ActionType ); 

private:
   EdsError  getFirstCamera(EdsCameraRef  *camera);
   int EdsToMMError(EdsError err);
   static EdsError EDSCALLBACK handleObjectEvent( EdsObjectEvent event, EdsBaseRef  object, EdsVoid * context);
   static EdsError EDSCALLBACK  handlePropertyEvent (EdsPropertyEvent event, EdsPropertyID  property, EdsUInt32 param, EdsVoid * context);
   static EdsError EDSCALLBACK handleStateEvent (EdsStateEvent event, EdsUInt32 parameter, EdsVoid * context);

   static void* g_Self;

   double nominalPixelSizeUm_;
   unsigned nComponents_;
   bool isSDKLoaded_;
   bool isLegacy_;
   EdsCameraRef camera_;
   std::string cameraModel_;
};


#endif
