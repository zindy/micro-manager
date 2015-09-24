/*
 * ASI motorized one-axis stage device adapter
 *
 * AUTHOR:
 * Jon Daniels (jon@asiimaging.com) 09/2013
 *
 * Copyright (c)  Applied Scientific Instrumentation, Eugene OR
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

#ifndef _ASIZStage_H_
#define _ASIZStage_H_

#include "ASIPeripheralBase.h"
#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"

class CZStage : public ASIPeripheralBase<CStageBase, CZStage>
{
public:
   CZStage(const char* name);
   ~CZStage() { Shutdown(); }
  
   // Device API
   // ----------
   int Initialize();
   bool Busy();

   // ZStage API
   // -----------
   int Stop();

   // the step size is the programming unit for dimensions and is integer
   // see http://micro-manager.3463995.n2.nabble.com/what-are-quot-steps-quot-for-stages-td7580724.html
   double GetStepSize() {return stepSizeUm_;}
   int GetPositionSteps(long& steps);
   int SetPositionSteps(long steps);
   int SetPositionUm(double pos);
   int GetPositionUm(double& pos);
   int SetRelativePositionUm(double d);
   int GetLimits(double& min, double& max);
   int SetOrigin();

   bool IsContinuousFocusDrive() const {return false;}  // todo figure out what this means and if it's accurate
   int IsStageSequenceable(bool& isSequenceable) const { isSequenceable = false; return DEVICE_OK; }
   int GetStageSequenceMaxLength(long& nrEvents) const  { nrEvents = 0; return DEVICE_OK; }

   // below aren't implemented yet
   int StartStageSequence()  { return DEVICE_UNSUPPORTED_COMMAND; }
   int StopStageSequence()   { return DEVICE_UNSUPPORTED_COMMAND; }
   int ClearStageSequence()  { return DEVICE_UNSUPPORTED_COMMAND; }
   int AddToStageSequence(double /*position*/)  { return DEVICE_UNSUPPORTED_COMMAND; }
   int SendStageSequence()  { return DEVICE_UNSUPPORTED_COMMAND; }

   // action interface
   // ----------------
   int OnSaveCardSettings     (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnRefreshProperties    (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnWaitTime             (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnMaxSpeed             (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnSpeed                (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnBacklash             (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnDriftError           (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnFinishError          (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAcceleration         (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnLowerLim             (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnUpperLim             (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnMaintainState        (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAdvancedProperties   (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnOvershoot            (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnKIntegral            (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnKProportional        (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnKDerivative          (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAAlign               (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAZero                (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnMotorControl         (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnJoystickFastSpeed    (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnJoystickSlowSpeed    (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnJoystickMirror       (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnJoystickSelect       (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnWheelFastSpeed       (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnWheelSlowSpeed       (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnWheelMirror          (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAxisPolarity         (MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   double unitMult_;
   double stepSizeUm_;
   string axisLetter_;
   bool advancedPropsEnabled_;

   // private helper functions
   int OnSaveJoystickSettings();
};

#endif //_ASIZStage_H_
