/*
 * Definition of X-Cite Led Controller Class
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
	
//
// XLED1 is a controller from Lumen Dynamics.
// It accept remote serial input to conrol micromanipulator.
//

class XLedCtrl : public CGenericBase<XLedCtrl>
{
   public:

	  // contructor & destructor
	  // .......................
      XLedCtrl();
      ~XLedCtrl();

      // Device API
      // ---------
      int Initialize();
      int Shutdown();  // shutdown the controller

      void GetName(char* pszName) const;
      bool Busy() { return false; }


	  //int DeInitialize() { m_yInitialized = false; return DEVICE_OK; };
      //bool Initialized() { return m_yInitialized; };
      int ReadAllProperty();
      char* GetXLedStatus(unsigned char* sResp, char* sXLedStatus);
      int GetStatusDescription(long lStatus, char* sStatus);
      // int GetSerialNumber(unsigned char* sResp);


      // action interface
      // ---------------
      int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
      int OnDebugLogFlag(MM::PropertyBase* pProp, MM::ActionType eAct);
      int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);
      int OnAllOnOff(MM::PropertyBase* pProp, MM::ActionType eAct);
      int OnPWMStatus(MM::PropertyBase* pProp, MM::ActionType pAct);
      int OnPWMMode(MM::PropertyBase* pProp, MM::ActionType pAct);
      int OnFrontPanelLock(MM::PropertyBase* pProp, MM::ActionType pAct);
      int OnLCDScrnNumber(MM::PropertyBase* pProp, MM::ActionType pAct);
      int OnLCDScrnBrite(MM::PropertyBase* pProp, MM::ActionType pAct);
      int OnLCDScrnSaver(MM::PropertyBase* pProp, MM::ActionType pAct);
      int OnClearAlarm(MM::PropertyBase* pProp, MM::ActionType pAct);
      int OnSpeakerVolume(MM::PropertyBase* pProp, MM::ActionType pAct);
      int ConnectXLed(unsigned char* sResp);


   private:
      int XLedSerialIO(unsigned char* sCmd, unsigned char* sResp);  // write comand to serial port and read message from serial port
      int WriteCommand(const unsigned char* sCommand);              // write command to serial port
      int ReadMessage(unsigned char* sMessage);                     // read message from serial port

      double        m_dAnswerTimeoutMs;     // maximum waiting time for receiving reolied message
      bool          m_yInitialized;         // controller initialized flag
      long          m_lAllOnOff;            // all on/off flag
      long          m_lPWMState;            // PWM status
      long          m_lPWMMode;             // PWM mode
      long          m_lScrnLock;            // front panel lock
      long          m_lScrnNumber;          // screen number
      long          m_lScrnBrite;           // screen brightness
      long          m_lScrnTimeout;         // screen saver time out
      long          m_lSpeakerVol;          // speaker volume
};
