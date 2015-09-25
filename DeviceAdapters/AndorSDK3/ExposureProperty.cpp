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

#include "FloatProperty.h"
#include "ExposureProperty.h"
#include "CallBackManager.h"
#include <cmath>
#include <iomanip>

using namespace andor;
using namespace std;

TExposureProperty::TExposureProperty(const string & MM_name, IFloat * float_feature, ICallBackManager* callback,
                                bool readOnly, bool needsCallBack)
: MM_name_(MM_name),
  float_feature_(float_feature),
  callback_(callback),
  callbackRegistered_(needsCallBack)
{
   if (float_feature->IsImplemented())
   {
      CPropertyAction * pAct = new CPropertyAction (this, &TExposureProperty::OnFloat);
      callback->CPCCreateProperty(MM_name.c_str(), "", MM::Float, readOnly, pAct);

      try 
      {
         if (needsCallBack)
         {
            float_feature_->Attach(this);
         }
      }
      catch (exception & e)
      {
         // SDK3 Callback not implemented for this feature
         callback->CPCLog(e.what());
      }
   }
   else
   {
      callbackRegistered_ = false;
   }
}

TExposureProperty::~TExposureProperty()
{
   if (callbackRegistered_)
   {
      try 
      {
         float_feature_->Detach(this);
      }
      catch (exception & e)
      {
         // SDK3 Callback not implemented for this feature
         callback_->CPCLog(e.what());
      }
   }
   //Clean up memory, created as passed in
   callback_->GetCameraDevice()->Release(float_feature_);
}

void TExposureProperty::Update(ISubject * Subject)
{
   //if NOT Poised,... (Snapshot sets this first, then changes trigger silently
   // so once updates get applied, and repoise, snapshot sets true, so no erroneous updates get applied
   if ( !callback_->IsSSCPoised() )
   {
      IFloat * featureSubject = dynamic_cast<IFloat *>(Subject);
      TAndorFloatCache * cache = dynamic_cast<TAndorFloatCache *>(float_feature_);
      if (cache && featureSubject)
      {
         cache->SetCache(featureSubject->Get());
      }
   }
}

void TExposureProperty::setFeatureWithinLimits(double new_value)
{
   try
   {
      if (new_value < float_feature_->Min())
      {
         new_value = float_feature_->Min();
      }
      else if (new_value > float_feature_->Max())
      {
         new_value = float_feature_->Max();
      }
      float_feature_->Set(new_value);
   }
   catch (exception & e)
   {
      callback_->CPCLog(e.what());
   }
}

bool TExposureProperty::valueIsWithinLimits(double new_value)
{
   try
   {
      return new_value > float_feature_->Min() && new_value < float_feature_->Max();
   }
   catch (exception & e)
   {
      callback_->CPCLog(e.what());
      return false;
   }
}



inline bool almostEqual(double val1, double val2, int precisionFactor)
{
   const double base = 10.0;
   double precisionError = 1.0 / pow(base, precisionFactor);
   // Check if val1 and val2 are within precision decimal places
   return ( val1 > (val2 - precisionError) && val1 < (val2 + precisionError)) ? true : false;
}

int TExposureProperty::OnFloat(MM::PropertyBase * pProp, MM::ActionType eAct)
{
   if (eAct == MM::BeforeGet)
   {
      TAndorFloatCache * cache = dynamic_cast<TAndorFloatCache *>(float_feature_);
      if (cache)
      {
         pProp->Set(cache->Get(callback_->IsSSCPoised()));
      }
      else
      {
         pProp->Set(float_feature_->Get());
      }

   }
   else if (eAct == MM::AfterSet)
   {
      double new_value = 0.0, current_value = float_feature_->Get();
      pProp->Get(new_value);
      TAndorFloatCache * cache = dynamic_cast<TAndorFloatCache *>(float_feature_);
      if (cache)
      {
         current_value = cache->Get(callback_->IsSSCPoised());
      }
      if (!almostEqual(new_value, current_value, DEC_PLACES_ERROR))
      {
         if(valueIsWithinLimits(new_value))
         {
           setFeatureWithinLimits(new_value);
         }
         else
         {
            bool wasPoised = callback_->IsSSCPoised();

            if(wasPoised)
              callback_->SSCLeavePoised();
            
            setFeatureWithinLimits(new_value);
            
            if(wasPoised)
              callback_->SSCEnterPoised();
         }
      }
   }

   return DEVICE_OK;
}


