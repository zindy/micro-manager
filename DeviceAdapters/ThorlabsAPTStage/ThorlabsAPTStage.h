/*
 * Control of Thorlabs stages using the APT library
 *
 * AUTHOR:
 * Emilio J. Gualda, IGC, 2012
 * Egor Zindy (egor.zindy@manchester.ac.uk)
 * Contributions and testing (TDC001): Alfie O'Neill / Christopher Blount
 *
 * Copyright (c) 2012 Emilio J. Gualda
 * Copyright (c) 2013 Egor Zindy, University of Manchester
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

#ifndef _THORLABSDCSTAGE_H_
#define _THORLABSDCSTAGE_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include "APTAPI.h"
#include <string>
#include <map>

//////////////////////////////////////////////////////////////////////////////
// Error codes
#define ERR_PORT_CHANGE_FORBIDDEN    10004
#define ERR_UNRECOGNIZED_ANSWER      10009
#define ERR_UNSPECIFIED_ERROR        10010
#define ERR_HOME_REQUIRED            10011
#define ERR_INVALID_PACKET_LENGTH    10012
#define ERR_RESPONSE_TIMEOUT         10013
#define ERR_BUSY                     10014
#define ERR_STEPS_OUT_OF_RANGE       10015
#define ERR_STAGE_NOT_ZEROED         10016

//////////////////////////////////////////////////////////////////////////////
// Global flag used for the initialisation of the APT subsystem.
// Want to initialise only once for any number of stages
// as initialisation takes time.
//
bool aptInitialized = false;

class ThorlabsAPTStage : public CStageBase<ThorlabsAPTStage>
{
public:
    ThorlabsAPTStage();
    ThorlabsAPTStage(int hwType, std::string deviceName, long chNumber);
    ~ThorlabsAPTStage();

    // Device API
    // ----------
    int Initialize();
    int Shutdown();

    void GetName(char* pszName) const;
    bool Busy();

    // Stage API
    // ---------
    int SetPositionUm(double posUm);
    int SetPositionUmContinuous(double posUm);
    int GetPositionUm(double& pos);
    int SetPositionSteps(long steps);
    int GetPositionSteps(long& steps);
    int SetOrigin();
    int GetLimits(double& min, double& max);
    int SetLimits(double min, double max);

    int IsStageSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}
    bool IsContinuousFocusDrive() const {return false;}

    // action interface
    // ----------------
    int OnSerialNumber(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnChannelNumber(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnMinPosUm(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnMaxPosUm(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnPosition(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnVelocity(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnHome(MM::PropertyBase* pProp, MM::ActionType eAct);


private:

//   bool GetValue(std::string& sMessage, double& pos);
//   int SetMaxTravel();
//   double GetTravelTimeMs(long steps);

    void init(int hwType, std::string deviceName, long chNumber);
    int SetPositionUmFlag(double posUm, int continuousFlag);
    void LogInit();
    void LogIt();
    int Home();
    int GetVelParam(double &vel);
    int SetVelParam(double vel);

    //Private variables
    std::stringstream tmpMessage;
    int hwType_;
    std::string deviceName_;
    long chNumber_;
    long serialNumber_;
    double stepSizeUm_;
    bool initialized_;
    bool busy_;
    bool homed_;
    double answerTimeoutMs_;
    double minTravelUm_;
    double maxTravelUm_;
    float curPosUm_; // cached current position
    float pfPosition;
    float newPosition;
    float newVel;
    float pfMaxVel;
    float pfMinVel;
    float pfAccn;
    float pfMaxAccn;
    float pfMinPos;
    float pfMaxPos;
    long plUnits;
    float pfPitch;

    double home;

};

#endif //_THORLABSDCSTAGE_H_
