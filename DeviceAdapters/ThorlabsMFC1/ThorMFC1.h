/*
 * device adapter for ThorLabs MFC1 motor focus controller
 *
 * AUTHOR:
 * Tom Zangle, tzangle@gmail.com, 8/14/13. VS2010 3/28/14
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

#ifndef _THORMFC1_H_
#define _THORMFC1_H_

#include "DeviceBase.h"
#include "DeviceThreads.h"
#include <string>

//////////////////////////////////////////////////////////////////////////////
// Error codes
//
#define ERR_PORT_CHANGE_FORBIDDEN    10004
//#define ERR_UNRECOGNIZED_ANSWER      10009
//#define ERR_UNSPECIFIED_ERROR        10010
//#define ERR_HOME_REQUIRED            10011
//#define ERR_INVALID_PACKET_LENGTH    10012
//#define ERR_RESPONSE_TIMEOUT         10013
//#define ERR_BUSY                     10014
//#define ERR_STEPS_OUT_OF_RANGE       10015
//#define ERR_STAGE_NOT_ZEROED         10016
#define ERR_INVALID_POSITION         10017
//#define ERR_INVALID_NUMBER_OF_POS    10018
//#define ERR_MOVE_FAILED              10019
//#define ERR_UNRECOGNIZED_DEVICE      10020

class ThorZStage : public CStageBase<ThorZStage>
{
public:
	ThorZStage();
	~ThorZStage();
  
	// MM Device API
	int Initialize();
	int Shutdown();
  
	void GetName(char* pszName) const;
	bool Busy();

	// Stage API
	//List of functions taken from ThorLabs MotorZStage.h
    int SetPositionSteps(long steps);
    int GetPositionSteps(long& steps);
    int SetPositionUm(double pos_um);
    int GetPositionUm(double& pos_um);
	int SetRelativePositionSteps(long d_steps);
	int SetRelativePositionUm(double d_um);
	int SetOrigin();
    int GetLimits(double& min, double& max);
	/*List of functions taken from DemoCamera
	int SetPositionUm(double pos);
	int GetPositionUm(double& pos);
	int SetPositionSteps(long steps);
	int GetPositionSteps(long& steps);
	int SetOrigin();
	int GetLimits(double& min, double& max);
	int Stop();
	int Home();
	int Move(double speed);*/

	// Action interface
	//List of functions taken from ThorLabs MotorZStage.h
	//int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnSequence(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnPosition(MM::PropertyBase* pProp, MM::ActionType eAct);
    //int OnStepSizeX(MM::PropertyBase* pProp, MM::ActionType eAct);
    //int OnStepSizeY(MM::PropertyBase* pProp, MM::ActionType eAct);
    //int OnMaxVelocity(MM::PropertyBase* pProp, MM::ActionType eAct);
    //int OnAcceleration(MM::PropertyBase* pProp, MM::ActionType eAct);
    //int OnMoveTimeout(MM::PropertyBase* pProp, MM::ActionType eAct);
    //int OnChangeMotor(MM::PropertyBase* pProp, MM::ActionType eAct);
	//List of functions taken from DemoCamera
	int OnComPort(MM::PropertyBase* pProp, MM::ActionType eAct);
	/*int OnControllerName(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnSlewVelocity(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnInitVelocity(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnAcceleration(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnHoldCurrent(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnRunCurrent(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnMotorSteps(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnFullTurnUm(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnWithLimits(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnPosition(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnSpeed(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnOut1(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnOut2(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnExecute(MM::PropertyBase* pProp, MM::ActionType eAct);*/

    int IsStageSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}
    bool IsContinuousFocusDrive() const {return false;}



private:
   
	UCHAR Status;
	UCHAR Motor;
	UCHAR CommdAddress;
	UCHAR ReplyAddress;
	int Value;
	//const char* ComName;
	DWORD Baudrate;

	HANDLE Handle;

	long CurrPos; //current position in steps
	long ZeroPos; //current location of the origin (also in steps)
	
	double mmPerRot;
	double minMM;
	double maxMM;
	double countsPerRot; //measured value, based on motor controller internal "position"
	double countsPerUm;
	double pos_um_;

	std::string comport_;
	bool sequenceable_;
	bool initialized_;
	bool busy_internal; //internal flag used to indicate that a stage movement is not complete yet
};

#endif //_THORMFC1_H_
