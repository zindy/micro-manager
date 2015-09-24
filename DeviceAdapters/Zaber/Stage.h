/*
 * Stage
 *
 * AUTHOR:
 * Athabasca Witschi (contact@zaber.com)
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

#ifndef _STAGE_H_
#define _STAGE_H_

#include "Zaber.h"

extern const char* g_StageName;
extern const char* g_StageDescription;

class Stage : public CStageBase<Stage>, public ZaberBase
{
public:
	Stage();
	~Stage();

	// Device API
	// ----------
	int Initialize();
	int Shutdown();
	void GetName(char* name) const;
	bool Busy();

	// Stage API
	// ---------
	int GetPositionUm(double& pos);
	int GetPositionSteps(long& steps);
	int SetPositionUm(double pos);
	int SetRelativePositionUm(double d);
	int SetPositionSteps(long steps);
	int SetRelativePositionSteps(long steps);
	int Move(double velocity);
	int Stop();
	int Home();
	int SetAdapterOriginUm(double d);
	int SetOrigin();
	int GetLimits(double& lower, double& upper);

	int IsStageSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}
	bool IsContinuousFocusDrive() const {return false;}

	// action interface
	// ----------------
	int OnPort          (MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnDeviceAddress (MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnAxisNumber       (MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnMotorSteps    (MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnLinearMotion  (MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnSpeed         (MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnAccel         (MM::PropertyBase* pProp, MM::ActionType eAct);

private:
	long deviceAddress_;
	long axisNumber_;
	int homingTimeoutMs_;
	double stepSizeUm_;
	double convFactor_; // not very informative name
	std::string cmdPrefix_;
	long resolution_;
	long motorSteps_;
	double linearMotion_;
};

#endif //_STAGE_H_
