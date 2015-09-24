/*
 * AUTHOR:
 * Brian Ashcroft, ashcroft@leidenuniv.nl
 *
 * Copyright (c) 2009 Leiden University, Leiden
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

#ifndef _NIZStage_H_
#define _NIZStage_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ImgBuffer.h"
#include "flexmotn.h"
#include "MotnCnst.h"
#include <string>
#include <map>

//////////////////////////////////////////////////////////////////////////////
// Error codes
//
#define ERR_UNKNOWN_MODE         102
#define ERR_UNKNOWN_POSITION     103


//////////////////////////////////////////////////////////////////////////////
// CNIMotionZStage class
// Uses flexmotion to automate a focus stage(or any single axis)
//////////////////////////////////////////////////////////////////////////////


class CNIMotionZStage : public CStageBase<CNIMotionZStage>
{
private:
   u8 Axis;
   u8 BoardID;					// Board identification number
   i32 MoveVelocity;
   u32 MoveAcceleration;
   u16 MoveJerk;

   double stepSize_um_;
   double pos_um_;
   bool busy_;
   bool initialized_;
   double lowerLimit_;
   double upperLimit_;

   int SetBoard();
   int MoveBoard(int pos_steps );
   int Stop();
   const char* ZStageDeviceName;
public:
   CNIMotionZStage();
   ~CNIMotionZStage();

   bool Busy();// {return busy_;}
   void GetName(char* pszName) const;

   int Initialize();
   int Shutdown();
     
   // Stage API
    int SetPositionUm(double pos);// {pos_um_ = pos; return DEVICE_OK;}
    int GetPositionUm(double& pos);// {pos = pos_um_; return DEVICE_OK;}
   virtual double GetStepSize() {return stepSize_um_;}
    int SetPositionSteps(long steps);// {pos_um_ = steps * stepSize_um_; return DEVICE_OK;}
    int GetPositionSteps(long& steps);// {steps = (long)(pos_um_ / stepSize_um_); return DEVICE_OK;}
   virtual int SetOrigin() {return DEVICE_OK;}
   virtual int GetLimits(double& lower, double& upper)
   {
      lower = lowerLimit_;
      upper = upperLimit_;
      return DEVICE_OK;
   }

   // action interface
   // ----------------
   int OnPosition(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAxis(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnBoardID(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnSetVelocity(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnSetAcceleration(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnSetJerk(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnSetStepSize(MM::PropertyBase* pProp, MM::ActionType eAct);

   int IsStageSequenceable(bool & seq) const { seq=false; return DEVICE_OK; }
   bool IsContinuousFocusDrive() const { return false; }


  
};



#endif //_NIZStage_H_
