/*
 * Thorlabs device adapters: BBD102 Controller
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, 2011
 * http://nenad.amodaj.com
 *
 * Copyright (c) 2011 Thorlabs Inc
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

#ifndef _MOTORZSTAGE_H_
#define _MOTORZSTAGE_H_

#include <MMDevice.h>
#include <DeviceBase.h>
#include "MotorStage.h"
#include "Thorlabs.h"

//////////////////////////////////////////////////////////////////////////////
// MotorZStage class
// (device adapter)
//////////////////////////////////////////////////////////////////////////////
class MotorZStage : public CStageBase<MotorZStage>
{
public:
   MotorZStage();
   ~MotorZStage();

   // Device API
   // ----------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();

   // Stage API
   // ---------
   int SetOrigin();
   int SetPositionUm(double pos);
   int GetPositionUm(double& pos);
   int SetPositionSteps(long steps);
   int GetPositionSteps(long& steps);
   int GetLimits(double& min, double& max);

   int IsStageSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}
   bool IsContinuousFocusDrive() const {return false;}

   // action interface
   // ----------------
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnStepSizeX(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnStepSizeY(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnMaxVelocity(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAcceleration(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnMoveTimeout(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnChangeMotor(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   int GetCommand(unsigned char* answer, unsigned answerLength, double TimeoutMs);
   int SetCommand(const unsigned char* command, unsigned cmdLength);
   int MoveBlocking(long pos, bool relative = false);
   bool RecognizedDevice(char *model);
   int Home();

   class CommandThread;

   int attachedMotor_;           // type of motor attached to the controller
   HWINFO info_;                 // hardware information
   std::string port_;            // com port name
   bool initialized_;            // true if the device is intitalized
   double answerTimeoutMs_;      // max wait for the device to answer
   double curPosUm_;             // cached current position
   double moveTimeoutMs_;        // max wait for stage to finish moving
   MotorStage *zstage_;          // fundamental stage device
   bool home_;                   // true if stage is homed
   CommandThread* cmdThread_;    // thread used to execute move commands
   double maxSteps_;             // maximum number of steps supported by motor
   double stepSizeUm_;           // step size per appropriate unit for the current motor
};

#endif //_MOTORZSTAGE_H_
