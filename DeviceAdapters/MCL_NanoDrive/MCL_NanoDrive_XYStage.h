/*
 * Copyright (c) 2008, Mad City Labs Inc.
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
File:		MCL_NanoDrive_XYStage.h
Copyright:	Mad City Labs Inc., 2008
License:	Distributed under the BSD license.
*/
#ifndef _MCL_NANODRIVE_XYSTAGE_H_
#define _MCL_NANODRIVE_XYSTAGE_H_

#pragma once 

// MCL headers
#include "Madlib.h"
#include "MCL_NanoDrive.h"

// MM headers
#include "../../MMDevice/ModuleInterface.h"
#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"

// List/heap headers
#include "device_list.h"
#include "handle_list_if.h"
#include "HandleListType.h"
#include "heap.h"

class MCL_NanoDrive_XYStage : public CXYStageBase<MCL_NanoDrive_XYStage>
{
public:
	
	MCL_NanoDrive_XYStage();
	~MCL_NanoDrive_XYStage();

	bool Busy();
	void GetName(char* name) const;

	int Initialize();
	int Shutdown();

	// XY Stage API
    virtual double GetStepSize();
    virtual int SetPositionSteps(long x, long y);
    virtual int GetPositionSteps(long &x, long &y);
	virtual int SetRelativePositionUm(double dx, double dy);
    virtual int Home();
    virtual int Stop();
    virtual int SetOrigin();
    virtual int GetLimits(double& lower, double& upper);
    virtual int GetLimitsUm(double& xMin, double& xMax, double& yMin, double& yMax);
	virtual int GetStepLimits(long& xMin, long& xMax, long& yMin, long& yMax);
	virtual double GetStepSizeXUm();
	virtual double GetStepSizeYUm();
	virtual int IsXYStageSequenceable(bool& isSequenceable) const;
	int getHandle(){  return MCLhandle_;}


	// Action interface
	int OnPositionXUm(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnPositionYUm(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnSettlingTimeXMs(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnSettlingTimeYMs(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnSetOrigin(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
	int SetDeviceProperties();
	int SetPositionXUm(double x);
	int SetPositionYUm(double y);
	void PauseDevice(int axis);
	int SetPositionUm(double x, double y);
    int GetPositionUm(double& x, double& y);

	bool busy_;
	bool initialized_;
	bool is20Bit_;

	int MCLhandle_;

	double stepSizeX_um_;
	double stepSizeY_um_;

	double xMin_;
	double xMax_;
	double yMin_;
	double yMax_;
	
	int serialNumber_;
	int settlingTimeX_ms_;
	int settlingTimeY_ms_;

	double curXpos_;
	double curYpos_;

	bool firstWriteX_;
	bool firstWriteY_;
};

#endif // _MCL_NANODRIVE_XYSTAGE_H_
