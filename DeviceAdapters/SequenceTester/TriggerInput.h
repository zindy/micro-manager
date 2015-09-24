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

#pragma once

#include "InterDevice.h"
#include "LoggedSetting.h"

#include <string>


// Common implementation for devices that receive trigger input
class TriggerInput
{
   const std::string settingNamePrefix_;

   InterDevice::Ptr device_;
   LoggedSetting::Ptr sequencedSetting_;

   StringSetting::Ptr triggerSourceDevice_;
   StringSetting::Ptr triggerSourcePort_;
   IntegerSetting::Ptr sequenceMaxLength_;

public:
   TriggerInput(const std::string& settingNamePrefix = "") :
      settingNamePrefix_(settingNamePrefix)
   {}

   // We may in the future need a mechanism to sequence more than one setting
   // in parallel with the same trigger input. For now, just allow one.
   void Initialize(InterDevice::Ptr device,
         LoggedSetting::Ptr sequencedSetting);

   StringSetting::Ptr GetSourceDeviceSetting()
   { return triggerSourceDevice_; }
   StringSetting::Ptr GetSourcePortSetting()
   { return triggerSourcePort_; }
   IntegerSetting::Ptr GetSequenceMaxLengthSetting()
   { return sequenceMaxLength_; }

private:
   void UpdateTriggerConnection();
};
