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

#ifndef _XYSTAGE_H_
#define _XYSTAGE_H_

#include <MMDevice.h>
#include <DeviceBase.h>
#include "Thorlabs.h"
#include "MotorStage.h"

//////////////////////////////////////////////////////////////////////////////
// XYStage class
// (device adapter)
//////////////////////////////////////////////////////////////////////////////
class ThorlabsStage;

class XYStage : public CXYStageBase<XYStage>
{
public:
   XYStage();
   ~XYStage();

   // Device API
   // ----------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();

   // XYStage API
   // -----------
   int SetPositionSteps(long x, long y);
   int SetRelativePositionSteps(long x, long y);
   int GetPositionSteps(long& x, long& y);
   int Home();
   int Stop();
   int SetOrigin();
   int GetLimitsUm(double& xMin, double& xMax, double& yMin, double& yMax);
   int GetStepLimits(long& xMin, long& xMax, long& yMin, long& yMax);
   double GetStepSizeXUm();
   double GetStepSizeYUm();
   int IsXYStageSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}

   // action interface
   // ----------------
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnStepSizeX(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnStepSizeY(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnMaxVelocity(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAcceleration(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnMoveTimeout(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   
   enum Axis {X, Y};

   int MoveBlocking(long x, long y, bool relative = false);
   int SetCommand(const unsigned char* command, unsigned cmdLength);
   int GetCommand(unsigned char* answer, unsigned answerLength, double TimeoutMs);
   int SetVelocityProfile(const MOTVELPARAMS& params, Axis a);
   int GetVelocityProfile(MOTVELPARAMS& params, Axis a);
   int ParseVelocityProfile(const unsigned char* buf, int bufLen, MOTVELPARAMS& params);
   int GetStatus(DCMOTSTATUS& stat, Axis a);

   class CommandThread;

   bool initialized_;            // true if the device is intitalized
   bool home_;                   // true if stage is homed
   std::string port_;            // com port name
   double answerTimeoutMs_;      // max wait for the device to answer
   double moveTimeoutMs_;        // max wait for stage to finish moving
   MotorStage *xstage_;          // x-axis stage device
   MotorStage *ystage_;          // y-axis stage device
   HWINFO info_;                 // hardware information
   CommandThread* cmdThread_;    // thread used to execute move commands
};

#endif //_XYSTAGE_H_
