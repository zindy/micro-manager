/*
 * nPoint C400 Controller
 *
 * AUTHOR:
 * Lon Chu (lonchu@yahoo.com) created on August 2011
 *
 * Copyright (c) 2011 nPoint
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
	
//
// MAP285 is a micromanipulator controller from Sutter Instrument Comapny.
// It accept remote serial input to conrol micromanipulator.
//
class nPC400Ctrl : public CGenericBase<nPC400Ctrl>
{
   public:

	  // contructor & destructor
	  // .......................
      nPC400Ctrl();
      ~nPC400Ctrl();

      // Device API
      // ---------
      int Initialize();
      int Shutdown();

      void GetName(char* pszName) const;

	  // MP285 doesn't support equivalent command 
	  // return false for now
	  bool Busy() { return false; }


	  int DeInitialize() { m_yInitialized = false; return DEVICE_OK; };
      bool Initialized() { return m_yInitialized; };
      int SetOrigin() { return DEVICE_UNSUPPORTED_COMMAND; }
      int Stop() { return DEVICE_UNSUPPORTED_COMMAND; }
      int Home() { return DEVICE_UNSUPPORTED_COMMAND; }
      int GetChannelsConnected(unsigned char* sResp);
      int GetNumberOfAxes(int nChannelsConnected);

      // action interface
      // ---------------
      int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
      int OnDebugLogFlag(MM::PropertyBase* pProp, MM::ActionType eAct);
      int OnSpeed(MM::PropertyBase* /*pProp*/, MM::ActionType /*eAct*/) { return DEVICE_UNSUPPORTED_COMMAND; }
      int OnMotionMode(MM::PropertyBase* /*pProp*/, MM::ActionType /*eAct*/) { return DEVICE_UNSUPPORTED_COMMAND; }

   private:
      //void tohex(const unsigned char bByte, char* sHex);
      int WriteCommand(const unsigned char* sCommand);
      int ReadMessage(unsigned char* sMessage);

	  // montoring controller status
      // ---------------------------

	  // int checkError(std::string sWhat, unsigned char* sResp);
      // int checkStatus(unsigned char* sResponse, unsigned int nLength);

      std::string   m_sCommand;             // Command exchange with MMCore
      std::string   m_sPort;                // serial port id
      double        m_dAnswerTimeoutMs;     // maximum waiting time for receiving reolied message
      bool          m_yInitialized;         // controller initialized flag
};
