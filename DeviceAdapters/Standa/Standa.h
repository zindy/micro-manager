/*
 * Standa device adapters: 8SMC1-USBhF Microstep Driver
 *
 * AUTHOR:
 * Clarence Leung, clarence.leung@mail.mcgill.ca, 2013
 *
 * Copyright (c) 2013 Leslie Lab, McGill University, Montreal
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

#ifndef _STANDA_H_
#define _STANDA_H_

// Standa headers
#include "USMCDLL.h"

// MM headers
#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ModuleInterface.h"

//////////////////////////////////////////////////////////////////////////////
// StandaZStage class
// (device adapter)
//////////////////////////////////////////////////////////////////////////////
class StandaZStage : public CStageBase<StandaZStage>
{
public:
   StandaZStage();
   ~StandaZStage();
  
   // Device API
   // ----------
   int Initialize();
   int Shutdown();
  
   static const char* DeviceName_;
   void GetName(char* pszName) const;
   bool Busy();

   // Stage API
   // ---------
   int SetPositionUm(double pos);
   int GetPositionUm(double& pos);
   int SetPositionSteps(long steps);
   int GetPositionSteps(long& steps);
   int SetOrigin();
   int GetLimits(double& min, double& max);

   int IsStageSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}
   bool IsContinuousFocusDrive() const {return false;}

   // action interface
   // ----------------
   int OnAxisLimit(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnDeviceNumber(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPosition(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   bool initialized_;
   long curSteps_;
   double stepSizeUm_;
   double axisLimitUm_;
   BYTE stepDivisor_;
   float stageSpeed_;
   MM::TimeoutMs* timeOutTimer_;
   std::string deviceString_;
   DWORD deviceNumber_;
   USMC_State currentState_;
   USMC_StartParameters startParameters_;
};

#endif //_STANDA_H_
