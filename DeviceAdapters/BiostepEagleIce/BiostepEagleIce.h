/*
 * Adapter for biostep EagleIce
 *
 * AUTHOR:
 * Jens Gläser
 * License:       LGPL
 *
 * Copyright (c) 2013 biostep
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

#ifndef EagleIce_H_
#define EagleIce_H_

#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ImgBuffer.h"
#include "../../MMDevice/DeviceThreads.h"
#include "../../../3rdparty/biostep/EI_SDK 1.0/EagleIceSDK.h"
#include <string>

class biThread;

class EagleIce : public CCameraBase<EagleIce>
{
public:
	EagleIce();
	~EagleIce();

	// MMDevice API
	int Initialize();
	int Shutdown();

	void GetName(char* name) const;

	// MMCamera API
	int SnapImage();     
	int InsertImage();
	const unsigned char* GetImageBuffer(){return m_img.GetPixels();}                                    
   unsigned GetImageWidth() const;                                           
   unsigned GetImageHeight() const;                                          
   unsigned GetImageBytesPerPixel() const{return m_img.Depth();};                                   
   unsigned GetBitDepth() const;                                             
   long GetImageBufferSize() const{return lastResolution.X*lastResolution.Y *GetImageBytesPerPixel();};                                          
   double GetExposure() const;                                               
   void SetExposure(double exp);                                             
   int SetROI(unsigned x, unsigned y, unsigned xSize, unsigned ySize);       
   int GetROI(unsigned& x, unsigned& y, unsigned& xSize, unsigned& ySize);   
   int ClearROI();   
   int IsExposureSequenceable(bool& sequenceable) const { sequenceable = false; return DEVICE_OK;};
   int GetBinning() const;                                                   
   int SetBinning(int binF); 
   int StartSequenceAcquisition(double interval);
   int StartSequenceAcquisition(long numImages, double interval_ms, bool stopOnOverflow);
   int StopSequenceAcquisition();
   bool IsCapturing(); 
   int ThreadRun();
	EI_Resolution lastResolution;
	EI_Device* g_device;
	char buffer[2048*2048*2];
	ImgBuffer m_img;
	WORD imgx[2048*2048];
	bool _capturing;
	int OnCoolState(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnTemp(MM::PropertyBase* pProp, MM::ActionType eAct);
	
private:
	int SetAllowedBinning();
	bool _camfound;	
	double* temp;
	EI_Status getFirstCam();
	biThread* thd_;
	
};

class biThread : public MMDeviceThreadBase
{
	friend class EagleIce;

public:
	biThread(EagleIce* pCam);
	~biThread();
	void Stop();
	void Start();
private:
	int svc(void) throw();
	EagleIce* camera_; 
	bool stop_; 
	MMThreadLock stopLock_; 
};

#endif
