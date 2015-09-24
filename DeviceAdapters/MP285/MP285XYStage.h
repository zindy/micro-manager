/*
 * XY Stage
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

#ifndef _MP285XYSTAGE_H_
#define _MP285XYSTAGE_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include "MP285.h"

//
// define XY stage class that is atached to the MP285 controller
//
class XYStage : public CXYStageBase<XYStage>
{
public:

    XYStage();			// X-Y stage constructor
    ~XYStage();			// X-Y stage destructor

    // Device API
    // ----------
    int Initialize();	// X-Y stage initialization

    // get X-Y stage device name
    void GetName(char* pszName) const;

    // Busy is not applicable for MP285
    // the fuction will return false always
    bool Busy() { return false; }
    int Shutdown();

    // XYStage API
    // -----------

	// setup motion mode (1: relative, 0: relative
	int SetMotionMode(long lMotionMode);

    // Move X-Y stage to position in um
    int SetPositionUm(double dXPosUm, double dYPosUm);
	int SetRelativePositionUm(double dXPosUm, double dYPosUm);

    // Get X-Y stage position in um
    int GetPositionUm(double& dXPosUm, double& dYPosUm);

    // Move X-Y stage to position in uSteps
    int SetPositionSteps(long lXPosSteps, long lYPosSteps);
	int SetRelativePositionSteps(long lXPosSteps, long lYPosSteps);
	int _SetPositionSteps(long lXPosSteps, long lYPosSteps, long lZPosSteps);

    // Get X-Y stage position in uSteps
    int GetPositionSteps(long& lXPosSteps, long& lYPosSteps);

    // Get limits of the X-Y stage, not applicable for MP285 XY stage
    // The function will return DEVICE_OK always
    int GetStepLimits(long& /*xMin*/, long& /*xMax*/, long& /*yMin*/, long& /*yMax*/) { return DEVICE_OK/*DEVICE_UNSUPPORTED_COMMAND*/; }
    int GetLimitsUm(double& /*xMin*/, double& /*xMax*/, double& /*yMin*/, double& /*yMax*/) { return DEVICE_OK/*DEVICE_UNSUPPORTED_COMMAND*/; }

    // get step size in um
    double GetStepSizeXUm() { return m_dStepSizeUm; }

    // get step size in um
    double GetStepSizeYUm() { return m_dStepSizeUm; }

    // Set X-Y stage origin
    int SetOrigin();

    // Calibrate home positionK
    int Home() { return DEVICE_OK/*DEVICE_UNSUPPORTED_COMMAND*/; }

    // Stop X-Y-Z stage motion
    int Stop();

    // action interface
    // ----------------

    //int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
    //int OnStepSizeUm(MM::PropertyBase* pPro, MM::ActionType eAct);
    //int OnInterface(MM::PropertyBase* pPro, MM::ActionType eAct) ;
    int OnSpeed(MM::PropertyBase* /*pPro*/, MM::ActionType /*eAct*/);
    int OnGetPositionX(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnGetPositionY(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnSetPositionX(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnSetPositionY(MM::PropertyBase* pProp, MM::ActionType eAct);
    //int OnAccel(MM::PropertyBase* pPro, MM::ActionType eAct);

    /*
    * Returns whether a stage can be sequenced (synchronized by TTLs)
    * If returning true, then an XYStage class should also inherit
    * the SequenceableXYStage class and implement its methods.
    */
    int IsXYStageSequenceable(bool& /*isSequenceable*/) const  { return DEVICE_OK/*DEVICE_UNSUPPORTED_COMMAND*/; }     


private:

    int WriteCommand(unsigned char* sCommand, int nLength);
    int ReadMessage(unsigned char* sResponse, int nBytesRead);
    int CheckError(unsigned char bErrorCode);

    //int GetCommand(const std::string& cmd, std::string& response);
    //bool GetValue(std::string& sMessage, double& pos);

    bool        m_yInitialized;         // x-y stage initialized flag
    int         m_nAnswerTimeoutMs;     // time out value of waiting response message
    int         m_nAnswerTimeoutTrys;   // time out trys
    double      m_dStepSizeUm;          // coverting unit between step and um
};

#endif  // _MP285XYSSTAGE_H_
