/*
 * AUTHOR:
 * Mark Tsuchida
 *
 * Copyright (c) 2014 Regents of the University of California,
 * All Rights reserved
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

#include "DeviceInstanceBase.h"


class HubInstance : public DeviceInstanceBase<MM::Hub>
{
   bool hasDetectedInstalledDevices_;
   int detectInstalledDevicesStatus_; // Valid when hasDetectedInstalledDevices_ == true

public:
   HubInstance(CMMCore* core,
         boost::shared_ptr<LoadedDeviceAdapter> adapter,
         const std::string& name,
         MM::Device* pDevice,
         DeleteDeviceFunction deleteFunction,
         const std::string& label,
         mm::logging::Logger deviceLogger,
         mm::logging::Logger coreLogger) :
      DeviceInstanceBase<MM::Hub>(core, adapter, name, pDevice, deleteFunction, label, deviceLogger, coreLogger),
      hasDetectedInstalledDevices_(false)
   {}

   std::vector<std::string> GetInstalledPeripheralNames();

   // XXX Dubious: This gets the descriptions from the device objects rather
   // than the module interface. And currently the result of this method is not
   // ultimately used. Perhaps should remove from Core API.
   std::string GetInstalledPeripheralDescription(const std::string& peripheralName);

private:
   std::vector<MM::Device*> GetInstalledPeripherals();

   // 1:1 wrappers for MM::Hub interface
   void DetectInstalledDevices();
   void ClearInstalledDevices(); // Not implemented (TODO should be removed from device interface)
   unsigned GetNumberOfInstalledDevices();
   MM::Device* GetInstalledDevice(int devIdx);
};
