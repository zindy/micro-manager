/*
 * ASI programmable logic card device adapter
 *
 * AUTHOR:
 * Jon Daniels (jon@asiimaging.com) 05/2014
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

#ifndef _ASIPLOGIC_H_
#define _ASIPLOGIC_H_

#include "ASIPeripheralBase.h"
#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"

class CPLogic : public ASIPeripheralBase<CShutterBase, CPLogic>
{
public:
   CPLogic(const char* name);
   ~CPLogic() { Shutdown(); }
  
   // Device API
   // ----------
   int Initialize();
   bool Busy() { return false; }

   // Shutter API
   int SetOpen(bool open = true);
   int GetOpen(bool& open);
   int Fire(double /*deltaT*/) { return DEVICE_UNSUPPORTED_COMMAND; }

   // action interface
   // ----------------
   int OnPLogicMode           (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnSetShutterChannel    (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPLogicOutputState    (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnFrontpanelOutputState(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnBackplaneOutputState (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnTriggerSource        (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnClearAllCellStates   (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnSetCardPreset        (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPointerPosition      (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnEditCellType         (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnEditCellConfig       (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnEditCellInput1       (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnEditCellInput2       (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnEditCellInput3       (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnEditCellInput4       (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnEditCellUpdates      (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnSaveCardSettings     (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnRefreshProperties    (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAdvancedProperties   (MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnCellType             (MM::PropertyBase* pProp, MM::ActionType eAct, long index);
   int OnCellConfig           (MM::PropertyBase* pProp, MM::ActionType eAct, long index);
   int OnInput1               (MM::PropertyBase* pProp, MM::ActionType eAct, long index);
   int OnInput2               (MM::PropertyBase* pProp, MM::ActionType eAct, long index);
   int OnInput3               (MM::PropertyBase* pProp, MM::ActionType eAct, long index);
   int OnInput4               (MM::PropertyBase* pProp, MM::ActionType eAct, long index);
   int OnIOType               (MM::PropertyBase* pProp, MM::ActionType eAct, long index);
   int OnIOSourceAddress      (MM::PropertyBase* pProp, MM::ActionType eAct, long index);


private:
   string axisLetter_;
   unsigned int numCells_;
   unsigned int currentPosition_;  // cached value of current position
//   static const int NUM_CELLS = 16;
   bool useAsdiSPIMShutter_;
   bool shutterOpen_;
   bool advancedPropsEnabled_;
   bool editCellUpdates_;

   int SetShutterChannel();
   int SetPositionDirectly(unsigned int position);
   int GetCellPropertyName(long index, string suffix, char* name);
   int GetIOPropertyName(long index, string suffix, char* name);
   int RefreshAdvancedCellPropertyValues(long index);
   int RefreshCurrentPosition();
   int RefreshEditCellPropertyValues();
};

#endif //_ASIPLOGIC_H_
