/*
 * Copyright (c) 2008, Mad City Labs Inc.
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

/*
File:		MCL_NanoDrive_ZStage.h
Copyright:	Mad City Labs Inc., 2008
License:	Distributed under the BSD license.
*/
#ifndef _MCL_NANODRIVE_ZSTAGE_H_
#define _MCL_NANODRIVE_ZSTAGE_H_

// MCL headers
#include "Madlib.h"
#include "MCL_NanoDrive.h"

// MM headers
#include "../../MMDevice/ModuleInterface.h"
#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"

// List/heap headers
#include "device_list.h"
#include "handle_list_if.h"
#include "HandleListType.h"
#include "heap.h"

class MCL_NanoDrive_ZStage : public CStageBase<MCL_NanoDrive_ZStage>
{
public:
  MCL_NanoDrive_ZStage();
  ~MCL_NanoDrive_ZStage();

  bool Busy();
  void GetName(char* pszName) const;

  int Initialize();
  int Shutdown();
     
  // Stage API
  virtual int SetPositionUm(double pos);
  virtual int GetPositionUm(double& pos);
  virtual int SetRelativePositionUm(double d);
  virtual double GetStepSize();
  virtual int SetPositionSteps(long steps);
  virtual int GetPositionSteps(long& steps);
  virtual int SetOrigin();
  virtual int GetLimits(double& lower, double& upper);
  virtual int IsStageSequenceable(bool& isSequenceable) const;
  virtual int GetStageSequenceMaxLength(long& nrEvents) const;
  virtual int StartStageSequence();
  virtual int StopStageSequence();
  virtual int LoadStageSequence(std::vector<double> positions) const;
  virtual bool IsContinuousFocusDrive() const;
  virtual int ClearStageSequence();
  virtual int AddToStageSequence(double position);
  virtual int SendStageSequence(); 

  int getHandle(){ return MCLhandle_;}

  // Action interface
  int OnPositionUm(MM::PropertyBase* pProp, MM::ActionType eAct);
  int OnSettlingTimeZMs(MM::PropertyBase* pProp, MM::ActionType eAct);
  int OnSetOrigin(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   int CreateZStageProperties();

   double stepSize_um_;
   bool busy_;
   bool initialized_;
   double lowerLimit_;
   double upperLimit_;
   int MCLhandle_;
   double calibration_;
   int serialNumber_;
   int settlingTimeZ_ms_;

   double curZpos_;

   bool firstWrite_;

   int axis_;
};

#endif // _MCL_NANODRIVE_ZSTAGE_H_
