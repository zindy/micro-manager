/*
 * MP285 Micromanipulator Controller
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

#ifndef _MP285CTRL_H_
#define _MP285CTRL_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"

//
// MAP285 is a micromanipulator controller from Sutter Instrument Comapny.
// It accept remote serial input to conrol micromanipulator.
//
class MP285Ctrl : public CGenericBase<MP285Ctrl>
{
    public:

        // contructor & destructor
        // .......................
        MP285Ctrl();
        ~MP285Ctrl();

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
        int SetOrigin();
        int SetResolution(long lResolution);
		int SetVelocity(long lVelocity);
        int SetMotionMode(long lMotionMode);
        int Stop();
        int Home() { return DEVICE_OK/*DEVICE_UNSUPPORTED_COMMAND*/; }

        // action interface
        // ---------------
        int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
		int OnDebugLogFlag(MM::PropertyBase* pProp, MM::ActionType eAct);
        int OnResolution(MM::PropertyBase* pProp, MM::ActionType eAct);
        int OnSpeed(MM::PropertyBase* pProp, MM::ActionType eAct);
        int OnMotionMode(MM::PropertyBase* pProp, MM::ActionType eAct);
        int OnTimeoutInterval(MM::PropertyBase* pProp, MM::ActionType eAct);
        int OnTimeoutTrys(MM::PropertyBase* pProp, MM::ActionType eAct);

    private:

        int WriteCommand(unsigned char* sCommand, int nLength);
        int ReadMessage(unsigned char* sResponse, int nBytesRead);

        // montoring controller status
        // ---------------------------
        bool CheckError(unsigned char bErrorCode);
        int CheckStatus(unsigned char* sResponse, unsigned int nLength);

        std::string   m_sCommand;             // Command exchange with MMCore
        std::string   m_sPort;                // serial port id
        bool          m_yInitialized;         // controller initialized flag
        int           m_nAnswerTimeoutMs;     // maximum waiting time for receiving reolied message
        int           m_nAnswerTimeoutTrys;   // timeout trys
};

#endif  // _MP285CTRL_H_
