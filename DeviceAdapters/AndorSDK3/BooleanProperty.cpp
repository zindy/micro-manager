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

#include "BooleanProperty.h"
#include "CallBackManager.h"


using namespace andor;
using namespace std;

static const char * const g_StatusON = "On";
static const char * const g_StatusOFF = "Off";


TBooleanProperty::TBooleanProperty(const string & MM_name, IBool * boolean_feature,
                                       ICallBackManager * callback, bool readOnly)
: boolean_feature_(boolean_feature),
  callback_(callback)
{
   if (boolean_feature->IsImplemented())
   {
      CPropertyAction * pAct = new CPropertyAction (this, &TBooleanProperty::OnBoolean);
      callback->CPCCreateProperty(MM_name.c_str(), g_StatusON, MM::String, readOnly, pAct);

      vector<string> values;
      values.push_back(g_StatusOFF);
      values.push_back(g_StatusON);
      callback->CPCSetAllowedValues(MM_name.c_str(), values);
      try 
      {
         boolean_feature->Attach(this);
      }
      catch (exception & e)
      {
         // SDK3 Callback not implemented for this feature
         callback->CPCLog(e.what());
      }
   }
}

TBooleanProperty::~TBooleanProperty()
{
   if (boolean_feature_->IsImplemented())
   {
      try 
      {
         boolean_feature_->Detach(this);
      }
      catch (exception & e)
      {
         // SDK3 Callback not implemented for this feature
         callback_->CPCLog(e.what());
      }
   }
   //Clean up memory, created as passed in
   callback_->GetCameraDevice()->Release(boolean_feature_);
}

void TBooleanProperty::Update(ISubject * /*Subject*/)
{
}

void TBooleanProperty::setFeature(const string & value)
{
   try
   {
      if (boolean_feature_->IsWritable())
      {
         if (value.compare(g_StatusON) == 0)
         {
            boolean_feature_->Set(true);
         }
         else
         {
            boolean_feature_->Set(false);
         }
      }
   }
   catch (exception & e)
   {
      callback_->CPCLog(e.what());
   }
}

// Action handler for OnBoolean
int TBooleanProperty::OnBoolean(MM::PropertyBase * pProp, MM::ActionType eAct)
{
   if (eAct == MM::BeforeGet)
   {
      pProp->Set(boolean_feature_->Get() ? g_StatusON : g_StatusOFF);
   }
   else if (eAct == MM::AfterSet)
   {
      string temp_s;
      pProp->Get(temp_s);
      string currentValue(boolean_feature_->Get() ? g_StatusON : g_StatusOFF);
      if (0 != temp_s.compare(currentValue))
      {
         //Need check poised for Snap as camera running...
         if (callback_->IsSSCPoised())
         {
            callback_->SSCLeavePoised();
            setFeature(temp_s);
            callback_->SSCEnterPoised();
         }
         else if (callback_->IsLiveModeActive()) //Live
         {
            callback_->PauseLiveAcquisition();
            setFeature(temp_s);
            callback_->CPCRestartLiveAcquisition();
         }
         else
         {
            callback_->CPCLog("Error - cannot set boolean feature during MDA");
         }
      }
   }

   return DEVICE_OK;
}
