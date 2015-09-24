/*
 * The implemenation for the common plugin functions
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, 08/08/2005
 *
 * Copyright (c) 2006 Regents of the University of California
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

/*
 * NOTES:
 *            Change the implementation of module interface methods in
 *                 this file with caution, since the Micro-Manager plugin
 *                 mechanism relies on specific functionality as implemented
 *                 here.
 */

#include "ModuleInterface.h"

#include <algorithm>
#include <string>
#include <vector>


namespace {

struct DeviceInfo
{
   std::string name_;
   MM::DeviceType type_;
   std::string description_;

   DeviceInfo(const char* name, MM::DeviceType type, const char* description) :
      name_(name),
      type_(type),
      description_(description)
   {}
};

// Predicate for searching by name
class DeviceNameMatches
{
   std::string name_;
public:
   explicit DeviceNameMatches(const std::string& deviceName) : name_(deviceName) {}
   bool operator()(const DeviceInfo& info) { return info.name_ == name_; }
};

} // anonymous namespace


// Registered devices in this module (device adapter library)
static std::vector<DeviceInfo> g_registeredDevices;


MODULE_API long GetModuleVersion()
{
   return MODULE_INTERFACE_VERSION;   
}

MODULE_API long GetDeviceInterfaceVersion()
{
   return DEVICE_INTERFACE_VERSION;   
}

MODULE_API unsigned GetNumberOfDevices()
{
   return static_cast<unsigned>(g_registeredDevices.size());
}

MODULE_API bool GetDeviceName(unsigned deviceIndex, char* name, unsigned bufLen)
{
   if (deviceIndex >= g_registeredDevices.size())
      return false;

   const std::string& deviceName = g_registeredDevices[deviceIndex].name_;

   if (deviceName.size() >= bufLen)
      return false; // buffer too small, can't truncate the name

   strcpy(name, deviceName.c_str());
   return true;
}

MODULE_API bool GetDeviceType(const char* deviceName, int* type)
{
   std::vector<DeviceInfo>::const_iterator it =
      std::find_if(g_registeredDevices.begin(), g_registeredDevices.end(),
            DeviceNameMatches(deviceName));
   if (it == g_registeredDevices.end())
   {
      *type = MM::UnknownType;
      return false;
   }

   // Prefer int over enum across DLL boundary so that the module ABI does not
   // change (somewhat pedantic, but let's be safe).
   *type = static_cast<int>(it->type_);

   return true;
}

MODULE_API bool GetDeviceDescription(const char* deviceName, char* description, unsigned bufLen)
{
   std::vector<DeviceInfo>::const_iterator it =
      std::find_if(g_registeredDevices.begin(), g_registeredDevices.end(),
            DeviceNameMatches(deviceName));
   if (it == g_registeredDevices.end())
      return false;

   strncpy(description, it->description_.c_str(), bufLen - 1);

   return true;
}

void RegisterDevice(const char* deviceName, MM::DeviceType deviceType, const char* deviceDescription)
{
   if (!deviceName)
      return;

   if (!deviceDescription)
      // This is a bug; let the programmer know by displaying an ugly string
      deviceDescription = "(Null description)";

   if (std::find_if(g_registeredDevices.begin(), g_registeredDevices.end(),
            DeviceNameMatches(deviceName)) != g_registeredDevices.end())
      // Device with this name already registered
      return;

   g_registeredDevices.push_back(DeviceInfo(deviceName, deviceType, deviceDescription));
}
