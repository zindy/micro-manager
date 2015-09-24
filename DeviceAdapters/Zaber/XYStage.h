/*
 * XYStage Device Adapter
 *
 * AUTHOR:
 * David Goosen & Athabasca Witschi (contact@zaber.com)
 *
 * Copyright (c) 2014 Zaber Technologies
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

#ifndef _XYSTAGE_H_
#define _XYSTAGE_H_

#include "Zaber.h"

extern const char* g_XYStageName;
extern const char* g_XYStageDescription;

class XYStage : public CXYStageBase<XYStage>, public ZaberBase
{
public:
	XYStage();
	~XYStage();

	// Device API
	// ----------
	int Initialize();
	int Shutdown();
	void GetName(char* name) const;
	bool Busy();

	// XYStage API
	// -----------
	int GetLimitsUm(double& xMin, double& xMax, double& yMin, double& yMax);
	int Move(double vx, double vy);
	int SetPositionSteps(long x, long y);
	int GetPositionSteps(long& x, long& y);
	int SetRelativePositionSteps(long x, long y);
	int Home();
	int Stop();
	int SetOrigin();
	int SetAdapterOrigin();
	int GetStepLimits(long& xMin, long& xMax, long& yMin, long& yMax);
	double GetStepSizeXUm() {return stepSizeXUm_;}
	double GetStepSizeYUm() {return stepSizeYUm_;}

	int IsXYStageSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}

	// action interface
	// ----------------
	int OnPort          (MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnAxisX         (MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnAxisY         (MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnMotorStepsX   (MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnMotorStepsY   (MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnLinearMotionX (MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnLinearMotionY (MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnSpeedX        (MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnSpeedY        (MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnAccelX        (MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnAccelY        (MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnDeviceAddress (MM::PropertyBase* pProp, MM::ActionType eAct);

private:
	int SendXYMoveCommand(std::string type, long x, long y) const;
	int OnSpeed(long axis, MM::PropertyBase* pProp, MM::ActionType eAct) const;
	int OnAccel(long axis, MM::PropertyBase* pProp, MM::ActionType eAct) const;
	void GetOrientation(bool& mirrorX, bool& mirrorY);

	long deviceAddress_;
	bool rangeMeasured_;
	int homingTimeoutMs_;
	double stepSizeXUm_;
	double stepSizeYUm_;
	double convFactor_; // not very informative name
	long axisX_;
	long axisY_;
	std::string cmdPrefix_;
	long resolutionX_;
	long resolutionY_;
	long motorStepsX_;
	long motorStepsY_;
	double linearMotionX_;
	double linearMotionY_;
};

#endif //_XYSTAGE_H_
