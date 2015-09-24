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


class StateInstance : public DeviceInstanceBase<MM::State>
{
public:
   StateInstance(CMMCore* core,
         boost::shared_ptr<LoadedDeviceAdapter> adapter,
         const std::string& name,
         MM::Device* pDevice,
         DeleteDeviceFunction deleteFunction,
         const std::string& label,
         mm::logging::Logger deviceLogger,
         mm::logging::Logger coreLogger) :
      DeviceInstanceBase<MM::State>(core, adapter, name, pDevice, deleteFunction, label, deviceLogger, coreLogger)
   {}

   int SetPosition(long pos);
   int SetPosition(const char* label);
   int GetPosition(long& pos) const;
   std::string GetPositionLabel() const; // Name differs from MM::Device
   std::string GetPositionLabel(long pos) const;
   int GetLabelPosition(const char* label, long& pos) const;
   int SetPositionLabel(long pos, const char* label);
   unsigned long GetNumberOfPositions() const;
   int SetGateOpen(bool open = true);
   int GetGateOpen(bool& open);
};
