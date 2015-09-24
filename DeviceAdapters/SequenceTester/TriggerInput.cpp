/*
 * Mock device adapter for testing of device sequencing
 *
 * Copyright (c) 2014 Regents of the University of California
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TriggerInput.h"
#include "SequenceTester.h"

#include <boost/bind.hpp>
#include <string>


void
TriggerInput::Initialize(InterDevice::Ptr device,
      LoggedSetting::Ptr sequencedSetting)
{
   device_ = device;
   sequencedSetting_ = sequencedSetting;

   triggerSourceDevice_ = StringSetting::New(device_->GetLogger(),
         device_.get(), settingNamePrefix_ + "TriggerSourceDevice");
   triggerSourceDevice_->GetPostSetSignal().connect(
         boost::bind(&TriggerInput::UpdateTriggerConnection, this));

   triggerSourcePort_ = StringSetting::New(device_->GetLogger(),
         device_.get(), settingNamePrefix_ + "TriggerSourcePort");
   triggerSourcePort_->GetPostSetSignal().connect(
         boost::bind(&TriggerInput::UpdateTriggerConnection, this));

   sequenceMaxLength_ = IntegerSetting::New(device_->GetLogger(),
         device_.get(), settingNamePrefix_ + "TriggerSequenceMaxLength",
         0, false);
   sequencedSetting_->SetSequenceMaxLengthSetting(sequenceMaxLength_);
}


void
TriggerInput::UpdateTriggerConnection()
{
   sequencedSetting_->DisconnectEdgeTriggerSource();

   const std::string sourceDevice = triggerSourceDevice_->Get();
   const std::string sourcePort = triggerSourcePort_->Get();
   if (sourceDevice.empty() || sourcePort.empty())
      return;

   InterDevice::Ptr source = device_->GetHub()->FindPeerDevice(sourceDevice);
   if (!source)
      return;

   EdgeTriggerSignal* signal = source->GetEdgeTriggerSource(sourcePort);
   if (!signal)
      return;

   sequencedSetting_->ConnectToEdgeTriggerSource(*signal);
}
