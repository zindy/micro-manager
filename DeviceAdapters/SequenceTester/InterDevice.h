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

#include "LoggedSetting.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <string>

class SettingLogger;
class TesterHub;


// Common mixin interface for inter-device communication. Provides an interface
// for communication among devices belonging to the same hub, orthogonal to the
// Micro-Manager device interface. This way, we can use
// boost::shared_ptr<InterDevice> to reference any peer device.
class InterDevice : public boost::enable_shared_from_this<InterDevice>
{
public:
   typedef boost::shared_ptr<InterDevice> Ptr;

   InterDevice(const std::string& name) : name_(name) {}
   virtual ~InterDevice() {}
   virtual void SetHub(boost::shared_ptr<TesterHub> hub) { hub_ = hub; }

   virtual std::string GetDeviceName() const { return name_; }
   virtual boost::shared_ptr<TesterHub> GetHub() { return hub_; }
   virtual boost::shared_ptr<const TesterHub> GetHub() const { return hub_; }
   virtual SettingLogger* GetLogger();
   virtual CountDownSetting::Ptr GetBusySetting() = 0;

   virtual EdgeTriggerSignal* GetEdgeTriggerSource(const std::string& port);

protected:
   void RegisterEdgeTriggerSource(const std::string& port,
         EdgeTriggerSignal& signal);

private:
   const std::string name_;
   boost::shared_ptr<TesterHub> hub_;
   boost::unordered_map<std::string, EdgeTriggerSignal*> edgeTriggersSources_;
};
