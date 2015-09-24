/*
 * Andor/Spectral Diskovery Device adapter
 *
 * AUTHOR:
 * Nico Stuurman, 06/31/2015
 *
 * Copyright (c) 2015 Regents of the University of California
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

#ifdef WIN32
   #include <windows.h>
   #define snprintf _snprintf 
#endif


#include "DiskoveryModel.h"
#include "Diskovery.h"

// Motor Running
void DiskoveryModel::SetMotorRunningSD(const bool p)
{
   MMThreadGuard g(lock_);
   motorRunningSD_ = p;
   std::string s = static_cast<std::ostringstream*>( &(std::ostringstream() << p) )->str();
   core_.OnPropertyChanged(hubDevice_, motorRunningProp_, s.c_str());
}

// Preset SD
void DiskoveryModel::SetPresetSD(const uint16_t p)
{
   MMThreadGuard g(lock_);
   presetSD_ = p;
   if (sdDevice_ != 0)
   {
      sdDevice_->OnStateChanged(p - 1);
   }
}

void DiskoveryModel::SetPresetWF(const uint16_t p)
{
   MMThreadGuard g(lock_);
   presetWF_ = p;
   if (wfDevice_ != 0)
   {
      wfDevice_->OnStateChanged(p - 1);
   }                                                                   
}

// Preset Iris                                                         
void DiskoveryModel::SetPresetIris(const uint16_t p)                         
{
   MMThreadGuard g(lock_);
   presetIris_ = p;
   if (irisDevice_ != 0)
   {
      irisDevice_->OnStateChanged(p - 1);
   }                                                                   
}

// Preset TIRF                                                         
void DiskoveryModel::SetPresetTIRF(const uint16_t p)                         
{
   MMThreadGuard g(lock_);
   presetPX_ = p;
   if (tirfDevice_ != 0)
   {
      tirfDevice_->OnStateChanged(p);
   }
}

// Preset Filter W
void DiskoveryModel::SetPresetFilterW(const uint16_t p) 
{  
   MMThreadGuard g(lock_);
   presetFilterW_ = p;
   if (filterWDevice_ != 0)
   {
      filterWDevice_->OnStateChanged(p - 1);
   }                                                                   
}

// Preset Filter T                                                     
void DiskoveryModel::SetPresetFilterT(const uint16_t p) 
{ 
   MMThreadGuard g(lock_);
   presetFilterT_ = p;                                       
   if (filterTDevice_ != 0)
   {
      filterTDevice_->OnStateChanged(p - 1);
   }                                                                   
}

// TIRF slider Rot
void DiskoveryModel::SetPositionRot(const int32_t p)
{
   MMThreadGuard g(lock_);
   tirfRotPos_ = p;
   if (tirfDevice_ != 0)
   {
      tirfDevice_->SignalPropChanged("PositionRot", CDeviceUtils::ConvertToString((int) p));
   }
}

// TIRF slider Lin
void DiskoveryModel::SetPositionLin(const int32_t p)
{
   MMThreadGuard g(lock_);
   tirfLinPos_ = p;
   if (tirfDevice_ != 0)
   {
      tirfDevice_->SignalPropChanged("PositionLin", CDeviceUtils::ConvertToString((int) p));
   }
}

// Extract the number of the Iris label and use it as objective magnification
uint16_t DiskoveryModel::GetOM()
{
   uint16_t pos = GetPresetIris();
   const char* label = GetButtonIrisLabel(pos);
   std::string strLabel(label);
   std::stringstream ss(strLabel.substr(0, strLabel.size() -1));
   uint16_t val;
   ss >> val;
   return val;
}

