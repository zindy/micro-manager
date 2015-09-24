/*
 * Device adapter module
 *
 * AUTHOR:
 * Mark Tsuchida
 *
 * Copyright (c) 2013-2014 Regents of the University of California
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

#include "LoadedModule.h"

#include "../../MMDevice/DeviceThreads.h"
#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/ModuleInterface.h"
#include "../Logging/Logger.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

class CMMCore;


class DeviceInstance;


class LoadedDeviceAdapter /* final */ :
	boost::noncopyable,
	public boost::enable_shared_from_this<LoadedDeviceAdapter>
{
public:
   LoadedDeviceAdapter(const std::string& name, const std::string& filename);

   // TODO Unload() should mark the instance invalid (or require instance
   // deletion to unload)
   void Unload() { module_->Unload(); } // For developer use only

   std::string GetName() const { return name_; }

   // The "module lock", used to synchronize _most_ access to the device
   // adapter.
   MMThreadLock* GetLock();

   std::vector<std::string> GetAvailableDeviceNames() const;
   std::string GetDeviceDescription(const std::string& deviceName) const;
   MM::DeviceType GetAdvertisedDeviceType(const std::string& deviceName) const;

   boost::shared_ptr<DeviceInstance> LoadDevice(CMMCore* core,
         const std::string& name, const std::string& label,
         mm::logging::Logger deviceLogger,
         mm::logging::Logger coreLogger);

private:
   /**
    * \brief Utility class for getting fixed-length strings from the module
    */
   class ModuleStringBuffer : boost::noncopyable
   {
      char buf_[MM::MaxStrLength + 1];
      const LoadedDeviceAdapter* module_;
      const std::string& funcName_;

   public:
      ModuleStringBuffer(const LoadedDeviceAdapter* module,
            const std::string& functionName) :
         module_(module), funcName_(functionName)
      { memset(buf_, 0, sizeof(buf_)); }

      char* GetBuffer() { return buf_; }
      size_t GetMaxStrLen() { return sizeof(buf_) - 1; }
      std::string Get() const { Check(); return buf_; }
      bool IsEmpty() const { Check(); return (buf_[0] == '\0'); }

   private:
      void Check() const
      { if (buf_[sizeof(buf_) - 1] != '\0') ThrowBufferOverflowError(); }
      void ThrowBufferOverflowError() const;
   };

   void CheckInterfaceVersion() const;

   // Wrappers around raw module interface functions
   void InitializeModuleData();
   long GetModuleVersion() const;
   long GetDeviceInterfaceVersion() const;
   unsigned GetNumberOfDevices() const;
   bool GetDeviceName(unsigned index, char* buf, unsigned bufLen) const;
   bool GetDeviceDescription(const char* deviceName,
         char* buf, unsigned bufLen) const;
   bool GetDeviceType(const char* deviceName, int* type) const;
   MM::Device* CreateDevice(const char* deviceName);
   void DeleteDevice(MM::Device* device);

   const std::string name_;
   boost::shared_ptr<LoadedModule> module_;

   MMThreadLock lock_;

   // Cached function pointers
   mutable fnInitializeModuleData InitializeModuleData_;
   mutable fnCreateDevice CreateDevice_;
   mutable fnDeleteDevice DeleteDevice_;
   mutable fnGetModuleVersion GetModuleVersion_;
   mutable fnGetDeviceInterfaceVersion GetDeviceInterfaceVersion_;
   mutable fnGetNumberOfDevices GetNumberOfDevices_;
   mutable fnGetDeviceName GetDeviceName_;
   mutable fnGetDeviceType GetDeviceType_;
   mutable fnGetDeviceDescription GetDeviceDescription_;
};
