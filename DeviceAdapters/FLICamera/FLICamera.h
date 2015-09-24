/*
 * FLI Camera interface for MicroManager
 *
 * AUTHOR:
 * Jim Moronski, jim@flicamera.com, 12/2010
 *
 * Copyright (c) 2010 Finger Lakes Instrumentation, LLC
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

#ifndef _FLICAMERA_H_
#define _FLICAMERA_H_

#include "libfli.h"
#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ImgBuffer.h"
#include "../../MMDevice/DeviceUtils.h"
#include <string>
#include <map>

class CFLICamera : public CCameraBase<CFLICamera>
{
	public:
		CFLICamera();
		~CFLICamera();

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
		int PrepareSequenceAcqusition();
		double GetNominalPixelSizeUm() const;
		double GetPixelSizeUm() const;
		int GetBinning() const;
		int SetBinning(int binSize);
		int IsExposureSequenceable(bool& seq) const {seq = false; return DEVICE_OK;}
		int GetComponentName(unsigned channel, char* name);
		unsigned GetNumberOfChannels() const;
		unsigned GetNumberOfComponents() const;

    // action interface
		int OnPixelType(MM::PropertyBase* pProp, MM::ActionType eAct);
		int OnBinning(MM::PropertyBase* pProp, MM::ActionType eAct);
		int OnCCDTemperature(MM::PropertyBase* pProp, MM::ActionType eAct);
		int OnCCDTemperatureSetpoint(MM::PropertyBase* pProp, MM::ActionType eAct);
		int OnExposure(MM::PropertyBase* pProp, MM::ActionType eAct);
		int OnShutterSetting(MM::PropertyBase* pProp, MM::ActionType eAct);

	private:
		int ResizeImageBuffer();

		MMThreadLock* pDemoResourceLock_;
		bool initialized_;

		flidev_t dev_;
		ImgBuffer img_;
		ImgBuffer dlimg_;

		long image_width_;
		long image_height_;
		long image_offset_x_;
		long image_offset_y_;

		long offset_x_;
		long offset_y_;
		long width_;
		long height_;
		long bin_x_;
		long bin_y_;

		long offset_x_last_;
		long offset_y_last_;
		long width_last_;
		long height_last_;
		long bin_x_last_;
		long bin_y_last_;

		long exposure_;
		long shutter_;
		long downloaded_;

		double pixel_x_;
		double pixel_y_;

public:
	void Disconnect(void);
};


#endif
