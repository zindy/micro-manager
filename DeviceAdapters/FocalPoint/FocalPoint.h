/*
 * Adapter for Focalpoint control through Arduino Due
 *
 * AUTHOR:
 * Nico Stuurman, Oct. 2013
 *
 * Copyright (c)  Regents of the University of California
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

#ifndef _FOCALPOINT_H_
#define _FOCALPOINT_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include <string>

//////////////////////////////////////////////////////////////////////////////
// Error codes
//

#define ERR_SERIAL_PORT_NOT_OPEN     10003
#define ERR_PORT_CHANGE_FORBIDDEN    10004
#define ERR_COMMAND_FAILED           10005
#define ERR_UNRECOGNIZED_ANSWER      10009
#define ERR_UNSPECIFIED_ERROR        10010
#define ERR_NOT_LOCKED               10011
#define ERR_NOT_CALIBRATED           10012

MM::DeviceDetectionStatus FocalPointCheckSerialPort(MM::Device& device, MM::Core& core, std::string port, double ato);


class FocalPoint : public CAutoFocusBase<FocalPoint>
{
public:
   FocalPoint();
   ~FocalPoint();

   //MMDevice API
   bool Busy();
   void GetName(char* pszName) const;

   int Initialize();
   int Shutdown();

   // AutoFocus API
   virtual int SetContinuousFocusing(bool state);
   virtual int GetContinuousFocusing(bool& state);
   virtual bool IsContinuousFocusLocked();
   virtual int FullFocus();
   virtual int IncrementalFocus();
   virtual int GetLastFocusScore(double& score);
   virtual int GetCurrentFocusScore(double& /*score*/) {return DEVICE_UNSUPPORTED_COMMAND;}
   virtual int GetOffset(double& offset);
   virtual int SetOffset(double offset);

   // action interface
   // ----------------
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnFocus(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnWaitAfterLock(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnLaser(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnCommand(MM::PropertyBase* pProp, MM::ActionType eAct);


private:
   int CheckForDevice(); // make sure we can communicate
   int QueryCommand(const char* command, std::string answer);
   int SetLaser(bool state);
   int SetCommand(std::string state);

   std::string focusState_;
   std::string laserState_;
   std::string commandState_;
   bool initialized_;
   std::string port_;
   long waitAfterLock_;
};

#endif
