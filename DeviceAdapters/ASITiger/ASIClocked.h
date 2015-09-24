/*
 * ASI clocked device adapter (filter slider, turret)
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

#ifndef _ASIClocked_H_
#define _ASIClocked_H_

#include "ASIPeripheralBase.h"
#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"

using namespace std;

class CClocked : public ASIPeripheralBase<CStateDeviceBase, CClocked>
{
public:
   CClocked(const char* name);
   ~CClocked() { Shutdown(); }
  
   // Generic device API
   // ----------
   int Initialize();
   bool Busy();

   // State device API
   // -----------
   unsigned long GetNumberOfPositions() const { return numPositions_; }

   // action interface
   // ----------------
   int OnState                (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnLabel                (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnSaveCardSettings     (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnRefreshProperties    (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnJoystickSelect       (MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   unsigned int numPositions_;
   unsigned int curPosition_;

   // private helper functions
   int OnSaveJoystickSettings();

protected: // needs to be inherited
   string axisLetter_;
};

class CFSlider : public CClocked
{
public:
   CFSlider(const char* name);

   int Initialize();

   // action interface
   // ---------------

};

class CTurret : public CClocked
{
public:
   CTurret(const char* name);

   int Initialize();
};

#endif //_ASIClocked_H_
