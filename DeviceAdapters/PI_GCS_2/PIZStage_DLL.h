/*
 * PI GCS DLL ZStage
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, 08/28/2006
 * Steffen Rau, s.rau@pi.ws, 28/03/2008
 *
 * Copyright (c) 2006 Regents of the University of California
 * Copyright (c) 2008 Physik Instrumente (PI) GmbH & Co. KG
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

#ifndef _PI_ZSTAGE_DLL_H_
#define _PI_ZSTAGE_DLL_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"

class PIController;

class PIZStage : public CStageBase<PIZStage>
{
public:
   PIZStage();
   ~PIZStage();
  
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

   int IsStageSequenceable(bool& isSequenceable) const { isSequenceable = false; return DEVICE_OK; }
   bool IsContinuousFocusDrive() const { return false; }

   // action interface
   // ----------------
   int OnControllerName(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnStepSizeUm(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAxisName(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAxisLimit(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAxisTravelRange(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPosition(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnStageType(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnHoming(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnVelocity(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   std::string axisName_;
   double stepSizeUm_;
   bool initialized_;
   double axisLimitUm_;
   bool invertTravelRange_;
   std::string stageType_;
   std::string controllerName_;
   PIController* ctrl_;
};


#endif //_PI_ZSTAGE_DLL_H_
