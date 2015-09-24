/*
 * Z  Stage
 *
 * AUTHOR:
 * Lon Chu (lonchu@yahoo.com) created on June 2011
 *
 * Copyright (c)  Sutter Instrument
 * Copyright (c) 2011 Mission Bay Imaging, San Francisco
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

#ifndef _MP285ZSTAGE_H_
#define _MP285ZSTAGE_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include "MP285.h"

//
// define Z stage class that is atached to the MP285 controller
//
class ZStage : public CStageBase<ZStage>
{
public:
    ZStage();			// Z stage constructor
    ~ZStage();			// Z stage destructor

    // Device API
    // ----------

    // Z stage initialization & shutdown
    int Initialize();	
    int Shutdown();

    // Get Z stage device name
    void GetName(char* pszName) const;

    // Busy is not aplicable for MP285
    // It will return false always
    bool Busy() { return false; }

    // Stage API
    // ---------

	// setup motion mode (1: relative, 0: relative
	int SetMotionMode(long lMotionMode);

    // Move Z stage to position in um
    int SetPositionUm(double dZPosUm);
	int SetRelativePositionUm(double dZPosUm);

    // Get Z stage position in um
    int GetPositionUm(double& dZPosUm);

    // Move Z stage to positiion in uSteps
    int SetPositionSteps(long lZPosSteps);
	int SetRelativePositionSteps(long lZPosSteps);
	int _SetPositionSteps(long lXPosSteps, long lYPosSteps, long lZPosSteps);

    // Get Z stage position in uSteps
    int GetPositionSteps(long& lZPosSteps);

    // Set Z stage origin
    int SetOrigin();

    // Stop Z stage motion
    int Stop();

    // Get limits of Zstage
    // This function is not applicable for
    // MP285, the function will return DEVICE_OK
    // insttead.
    int GetLimits(double& /*min*/, double& /*max*/) { return DEVICE_OK/*DEVICE_UNSUPPORTED_COMMAND*/; }

    // action interface
    // ----------------
    int OnStepSize (MM::PropertyBase* /*pProp*/, MM::ActionType /*eAct*/);
    int OnSpeed(MM::PropertyBase* /*pPro*/, MM::ActionType /*eAct*/);
    int OnGetPositionZ(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnSetPositionZ(MM::PropertyBase* pProp, MM::ActionType eAct);

    // Sequence functions
    int IsStageSequenceable(bool& isSequenceable) const { isSequenceable = false; return DEVICE_OK;}
    int GetStageSequenceMaxLength(long& /*nrEvents*/) const  {return DEVICE_OK;}
    int StartStageSequence() {return DEVICE_OK;}
    int StopStageSequence() {return DEVICE_OK;}
    int LoadStageSequence(std::vector<double> /*positions*/) const {return DEVICE_OK;}
    int ClearStageSequence() {return DEVICE_OK;}
    int AddToStageSequence(double /*position*/) {return DEVICE_OK;}
    int SendStageSequence() {return DEVICE_OK;} 
    bool IsContinuousFocusDrive() const {return true;}

private:

    int WriteCommand(unsigned char* sCommand, int nLength);
    int ReadMessage(unsigned char* sResponse, int nBytesRead);
    int CheckError(unsigned char bErrorCode);
    //int GetCommand(const std::string& cmd, std::string& response);

    bool        m_yInitialized;         // z stage initialization flag
    int         m_nAnswerTimeoutMs;     // timeout value of Z stage waiting for response message
    int         m_nAnswerTimeoutTrys;   // timeout trys
};

#endif  // _MP285ZSTAGE_H_
