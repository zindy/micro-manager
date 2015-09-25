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

#include "IntegerProperty.h"
#include "AndorSDK3.h"
#include "SnapShotControl.h"

using namespace andor;
using namespace std;

TIntegerProperty::TIntegerProperty(const string & MM_name, IInteger * integer_feature, CAndorSDK3Camera * camera,
                                   MySequenceThread * thd, SnapShotControl * snapShotController, bool readOnly,
                                   bool needsCallBack)
: MM_name_(MM_name),
  integer_feature_(integer_feature),
  camera_(camera),
  thd_(thd),
  snapShotController_(snapShotController),
  callBackRegistered_(needsCallBack)
{
   if (integer_feature->IsImplemented())
   {
      CPropertyAction * pAct = new CPropertyAction (this, &TIntegerProperty::OnInteger);
      camera_->CreateProperty(MM_name.c_str(), "", MM::Integer, readOnly, pAct);

      try
      {
         if (needsCallBack)
         {
            integer_feature_->Attach(this);
         }
      }
      catch (exception & e)
      {
         // Callback not implemented for this feature
         camera_->LogMessage(e.what());
      }
   }
   else
   {
      callBackRegistered_ = false;
   }
}

TIntegerProperty::~TIntegerProperty()
{
   if (callBackRegistered_)
   {
      try 
      {
         integer_feature_->Detach(this);
      }
      catch (exception & e)
      {
         // Callback not implemented for this feature
         camera_->LogMessage(e.what());
      }
   }
   //Clean up memory, created as passed in
   camera_->GetCameraDevice()->Release(integer_feature_);
}

void TIntegerProperty::Update(ISubject * /*Subject*/)
{
   // This property has been changed in SDK3. The new value will be set by a
   // call to TIntegerProperty::OnInteger, in here reset the limits
   camera_->SetPropertyLimits(MM_name_.c_str(), (long)integer_feature_->Min(), (long)integer_feature_->Max());
}

int TIntegerProperty::OnInteger(MM::PropertyBase * pProp, MM::ActionType eAct)
{
   if (eAct == MM::BeforeGet)
   {
      pProp->Set((long)integer_feature_->Get());
   }
   else if (eAct == MM::AfterSet)
   {
      if (!thd_->IsStopped())
      {
         camera_->StopSequenceAcquisition();
      }

      bool was_poised = false;
      if (snapShotController_->isPoised())
      {
         snapShotController_->leavePoisedMode();
         was_poised = true;
      }

      long new_value;
      pProp->Get(new_value);
      if (new_value < integer_feature_->Min())
      {
         new_value = (long)integer_feature_->Min();
      }
      else if (new_value > integer_feature_->Max())
      {
         new_value = (long)integer_feature_->Max();
      }

      if (integer_feature_->IsWritable())
      {
         integer_feature_->Set(new_value);
      }

      if (was_poised)
      {
         snapShotController_->poiseForSnapShot();
      }
   }

   return DEVICE_OK;
}
