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

#include "DeviceInstance.h"

#include "../CoreUtils.h"
#include "../Error.h"
#include "../LoadableModules/LoadedDeviceAdapter.h"


// Common member function implementations for concrete DeviceInstance
// subclasses.
template <typename TMMDevice>
class DeviceInstanceBase : public DeviceInstance
{
public:
   typedef TMMDevice RawDeviceClass;

   // It would be nice to get rid of the need for raw pointers, but for now we
   // need it for the few CoreCallback methods that return a device pointer.
   RawDeviceClass* GetRawPtr() const { return GetImpl(); }

protected:
   DeviceInstanceBase(CMMCore* core,
         boost::shared_ptr<LoadedDeviceAdapter> adapter,
         const std::string& name,
         MM::Device* pDevice,
         DeleteDeviceFunction deleteFunction,
         const std::string& label,
         mm::logging::Logger deviceLogger,
         mm::logging::Logger coreLogger) :
      DeviceInstance(core, adapter, name, pDevice, deleteFunction,
            label, deviceLogger, coreLogger)
   {
      MM::DeviceType actualType = GetType();
      if (actualType != RawDeviceClass::Type)
         throw CMMError("Device " + ToQuotedString(name) +
               " of device adapter " + ToQuotedString(adapter->GetName()) +
               " was expected to be type " + ToString(RawDeviceClass::Type) +
               " but turned out to be type " + ToString(actualType));
   }

protected:
   RawDeviceClass* GetImpl() const /* final */ { return static_cast<RawDeviceClass*>(pImpl_); }
};
