/*
 * The drivers for the Picard Industries USB stages
 * Based on the CDemoStage and CDemoXYStage classes
 *
 * AUTHOR:
 * Johannes Schindelin, Luke Stuyvenberg, 2011 - 2014
 *
 * Copyright (c) 2011-2014 Board of Regents of the University of Wisconsin -- Madison
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

#ifndef _PICARDSTAGE_H_
#define _PICARDSTAGE_H_

#include "../../MMDevice/DeviceBase.h"

//////////////////////////////////////////////////////////////////////////////
// CPiTwister class
//////////////////////////////////////////////////////////////////////////////

class CPiTwister: public CStageBase<CPiTwister>
{
public:
	CPiTwister();
	~CPiTwister();

	bool Busy();
	int Initialize();
	int Shutdown();
	void GetName(char* name) const;

	int SetPositionUm(double pos);
	int GetPositionUm(double& pos);
	int SetPositionSteps(long steps);
	int GetPositionSteps(long& steps);
	int SetOrigin();
	int GetLimits(double& lower, double& upper);
	int GetStepLimits(long& lower, long& upper);
	int IsStageSequenceable(bool& isSequenceable) const;
	bool IsContinuousFocusDrive() const;

	double GetStepSizeUm();

private:
	int OnSerialNumber(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnVelocity(MM::PropertyBase* pProp, MM::ActionType eAct);

	int serial_;
	int velocity_;
	void *handle_;
};

//////////////////////////////////////////////////////////////////////////////
// CPiStage class
//////////////////////////////////////////////////////////////////////////////

class CPiStage : public CStageBase<CPiStage>
{
public:
	CPiStage();
	~CPiStage();

	bool Busy();
	int Initialize();
	int Shutdown();
	void GetName(char* name) const;

	int SetPositionUm(double pos);
	int GetPositionUm(double& pos);
	int SetPositionSteps(long steps);
	int GetPositionSteps(long& steps);
	int SetOrigin();
	int GetLimits(double& lower, double& upper);
	int GetStepLimits(long& lower, long& upper);
	int IsStageSequenceable(bool& isSequenceable) const;
	bool IsContinuousFocusDrive() const;

	double GetStepSizeUm();

private:
	int OnSerialNumber(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnVelocity(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnGoHomeProp(MM::PropertyBase* pProp, MM::ActionType eAct);

	int serial_;
	int velocity_;
	void *handle_;
};

//////////////////////////////////////////////////////////////////////////////
// CPiXYStage class
//////////////////////////////////////////////////////////////////////////////

class CPiXYStage : public CXYStageBase<CPiXYStage>
{
public:
	CPiXYStage();
	~CPiXYStage();

	bool Busy();
	int Initialize();
	int Shutdown();
	void GetName(char* name) const;

	int SetPositionUm(double x, double y);
	int GetPositionUm(double& x, double& y);
	int SetPositionSteps(long x, long y);
	int GetPositionSteps(long& x, long& y);
	int Home();
	int Stop();
	int SetOrigin();
	int GetLimitsUm(double& xMin, double& xMax, double& yMin, double& yMax);
	int GetStepLimits(long& xMin, long& xMax, long& yMin, long& yMax);
	int IsXYStageSequenceable(bool& isSequenceable) const;

	double GetStepSizeXUm();
	double GetStepSizeYUm();

protected:
	int InitStage(void** handleptr, int serial);
	void ShutdownStage(void** handleptr);

private:
	int OnSerialNumberX(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnSerialNumberY(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnVelocityX(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnVelocityY(MM::PropertyBase* pProp, MM::ActionType eAct);

	int serialX_, serialY_;
	int velocityX_, velocityY_;
	void *handleX_, *handleY_;
};

#endif //_PICARDSTAGE_H_
