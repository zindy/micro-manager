/*
 * Marzhauser LStep version 1.2
 *
 * AUTHOR:
 * Jannis Uhlendorf (jannis.uhlendorf@gmail.com) 2015
 *
 * Copyright (c)  Jannis Uhlendorf
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

#ifndef _LSTEPOLD_H_
#define _LSTEPOLD_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include <string>
#include <map>

//////////////////////////////////////////////////////////////////////////////
// Error codes
#define ERR_PORT_CHANGE_FORBIDDEN    10004

class LStepOld : public CXYStageBase<LStepOld>
{
public:
   LStepOld();
   ~LStepOld();

   //MMDevice API
   int Initialize();
   int Shutdown();
   void GetName(char* name) const;
   bool Busy();

   //XYStage API
   int SetPositionUm(double x, double y);
   int GetLimitsUm(double& xMin, double& xMax, double& yMin, double& yMax);
   int SetPositionSteps(long x, long y);
   int GetPositionSteps(long& x, long& y);
   int Home();
   int Stop();
   int SetOrigin();
   int GetStepLimits(long& xMin, long& xMax, long& yMin, long& yMax);
   double GetStepSizeXUm();
   double GetStepSizeYUm();
   int IsXYStageSequenceable(bool& isSequenceable) const;

   // action interface
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnSpeed(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnJoystick(MM::PropertyBase* pProp, MM::ActionType eAct);

   bool initialized_;
   std::string port_;    // MMCore name of serial port
   double answerTimeoutMs_;
   double motor_speed_;
   std::string joystick_command_;

private:
   int ExecuteCommand(const std::string& cmd, char* input=NULL, int input_len=0, std::string* ret=NULL);
   int send_msg(char* msg);
};

#endif //_LStepOld_H_
