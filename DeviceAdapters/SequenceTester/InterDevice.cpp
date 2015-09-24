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

#include "InterDevice.h"
#include "SequenceTester.h"


SettingLogger*
InterDevice::GetLogger()
{
   return GetHub()->GetLogger();
}


EdgeTriggerSignal*
InterDevice::GetEdgeTriggerSource(const std::string& port)
{
   boost::unordered_map<std::string, EdgeTriggerSignal*>::const_iterator
      found = edgeTriggersSources_.find(port);
   if (found == edgeTriggersSources_.end())
      return 0;
   return found->second;
}


void
InterDevice::RegisterEdgeTriggerSource(const std::string& port,
      EdgeTriggerSignal& signal)
{
   edgeTriggersSources_[port] = &signal;
}
