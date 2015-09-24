/*
 * Definition of X-Cite Led Device Class
 *
 * AUTHOR:
 * Lon Chu (lonchu@yahoo.com) created on July 2011
 *
 * Copyright (c) 2011 Lumen Dynamics
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

#pragma once

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
//#include "../../MMDevice/ModuleInterface.h"
//#include "XLed.h"
//#include "XLedCtrl.h"
	
//
// define LED device class that is atached to the XLED1 controller
//
class XLedDev : public CShutterBase<XLedDev>
{
public:
    XLedDev(int nLedDevNumber = 0);			// Led Device constructor
    ~XLedDev();                         // Led Device destructor

    // Device API
    // ----------

    // Led device initialization & shutdown
    int Initialize();	
    int Shutdown();

    // Led device name
    void GetName(char* pszName) const;

    // Busy is not aplicable for MP285
    // It will return false always
    bool Busy() { return false; }

    // Led device API
    // ---------

    int SetOpen(bool yOpen = true);
    int GetOpen(bool&  yOpen);
    int Fire(double /*deltaT*/)  { return DEVICE_UNSUPPORTED_COMMAND; }

    // get Led parameter Value
    int GetLedParm(unsigned char* sCmd, unsigned char* sResp, char* sParm = NULL);
    int GetLedParmVal(unsigned char* sResp, char* sParm);
    int GetStatusDescription(long lStatus, char* sStatus);


    // action interface
    // ----------------
    int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnHours(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnTemperature(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnTempHyst(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnLedOnOff(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnLedIntensity(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnPulseMode(MM::PropertyBase* pProp, MM::ActionType pAct);
    int OnSignalDelayTime(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnSignalOnTime(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnSignalOffTime(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnTriggerDelayTime(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnPWMUnit(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
    int XLedSerialIO(unsigned char* sCmd, unsigned char* sResp);  // write comand to serial port and read message from serial port
    int WriteCommand(const unsigned char* sCommand);              // write command to serial port
    int ReadMessage(unsigned char* sMessage);                     // read message from serial port

    bool          m_yInitialized;         // controller initialized flag
    double        m_dAnswerTimeoutMs;     // maximum waiting time for receiving reolied message
    int           m_nLedDevNumber;        // Led device number
    long          m_lOnOffState;          // Led on/off state
    long          m_lIntensity;           // Led intensity
    long          m_lSignalDelayTime;     // Led signal delay time
    long          m_lSignalOnTime;        // Led signal on time
    long          m_lSignalOffTime;       // Led signal off time
    long          m_lTriggerDelay;        // Led trigger delay time
    long          m_lPulseMode;           // pulse mode
    long          m_lPWMUnit;             // Led PWM unit
};
