/*
 * ASI CRISP autofocus device adapter
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

#ifndef _ASICRISP_H_
#define _ASICRISP_H_

#include "ASIPeripheralBase.h"
#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"

class CCRISP : public ASIPeripheralBase<CAutoFocusBase, CCRISP>
{
public:
   CCRISP(const char* name);
   ~CCRISP() { Shutdown(); }
  
   // Device API
   // ----------
   int Initialize();
   bool Busy();

   // AutoFocus API
   // -------------
   int SetContinuousFocusing(bool state);
   int GetContinuousFocusing(bool& state);
   bool IsContinuousFocusLocked();
   int FullFocus();
   int IncrementalFocus();
   int GetLastFocusScore(double& score);
   int GetCurrentFocusScore(double& score);
   int GetOffset(double& offset);
   int SetOffset(double offset);

   // action interface
   // ----------------
   int OnRefreshProperties (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnFocusState        (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnWaitAfterLock     (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnNA                (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnCalGain           (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnLockRange         (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnLEDIntensity      (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnLoopGainMultiplier(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnNumAvg            (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnSNR               (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnDitherError       (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnLogAmpAGC         (MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   string axisLetter_;
   string focusState_;
   long waitAfterLock_;

   int UpdateFocusState();
   int SetFocusState(string focusState);
};

#endif //_ASICRISP_H_
