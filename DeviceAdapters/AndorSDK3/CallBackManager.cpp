/*
 * Copyright (c) 2007, Regents of the University of California
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

#include "CallBackManager.h"
#include "SnapShotControl.h"
#include "AndorSDK3.h"

using namespace std;

CCallBackManager::CCallBackManager(CAndorSDK3Camera * _parentClass, MySequenceThread * _seqAcqThread, SnapShotControl * _snapControl)
:  parentClass_(_parentClass),
   thd_(_seqAcqThread),
   ssControl_(_snapControl)
{
}

CCallBackManager::~CCallBackManager()
{
}

bool CCallBackManager::IsSSCPoised()
{
   bool b_retCode = false;
   if (ssControl_)
   {
      b_retCode = ssControl_->isPoised();
   }
   return b_retCode;
}

bool CCallBackManager::SSCEnterPoised()
{
   bool b_retCode = false;
   if (ssControl_)
   {
      ssControl_->poiseForSnapShot();
      b_retCode = true;
   }
   return b_retCode;
}

bool CCallBackManager::SSCLeavePoised()
{
   bool b_retCode = false;
   if (ssControl_)
   {
      ssControl_->leavePoisedMode();
      b_retCode = true;
   }
   return b_retCode;
}

bool CCallBackManager::IsLiveModeActive()
{
   bool b_retCode = false;
   if (thd_)
   {
      b_retCode = !thd_->IsStopped() && LONG_MAX == thd_->GetLength();
   }
   return b_retCode;
}

int CCallBackManager::CPCCreateProperty(const char* name, const char* value, MM::PropertyType eType, 
                                          bool readOnly, MM::ActionFunctor* pAct, bool initStatus)
{
   return parentClass_->CreateProperty(name, value, eType, readOnly, pAct, initStatus);
}

int CCallBackManager::CPCSetAllowedValues(const char* pszName, vector<string>& values)
{
   return parentClass_->SetAllowedValues(pszName, values);
}

int CCallBackManager::CPCAddAllowedValueWithData(const char* pszName, const string & value, long data)
{
   return parentClass_->AddAllowedValue(pszName, value.c_str(), data);
}

int CCallBackManager::CPCLog(const char * msg)
{
   return parentClass_->LogMessage(msg);
}

void CCallBackManager::PauseLiveAcquisition()
{
   thd_->Suspend();
   ssControl_->resetCameraAcquiring();
}

void CCallBackManager::CPCRestartLiveAcquisition()
{
   parentClass_->RestartLiveAcquisition();
   thd_->Resume();
}

void CCallBackManager::CPCStopLiveAcquisition()
{
   parentClass_->StopSequenceAcquisition();
   parentClass_->GetCoreCallback()->ClearImageBuffer(parentClass_);
}

void CCallBackManager::CPCStartLiveAcquisition()
{
   parentClass_->StartSequenceAcquisition(0.f);
}

void CCallBackManager::CPCResizeImageBuffer()
{
   parentClass_->ResizeImageBuffer();
}

int CCallBackManager::CPCGetBinningFactor()
{
   return parentClass_->GetBinning();
}

andor::IDevice * CCallBackManager::GetCameraDevice()
{
   return parentClass_->GetCameraDevice();
}
