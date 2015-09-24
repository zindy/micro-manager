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


class SLMInstance : public DeviceInstanceBase<MM::SLM>
{
public:
   SLMInstance(CMMCore* core,
         boost::shared_ptr<LoadedDeviceAdapter> adapter,
         const std::string& name,
         MM::Device* pDevice,
         DeleteDeviceFunction deleteFunction,
         const std::string& label,
         mm::logging::Logger deviceLogger,
         mm::logging::Logger coreLogger) :
      DeviceInstanceBase<MM::SLM>(core, adapter, name, pDevice, deleteFunction, label, deviceLogger, coreLogger)
   {}

   int SetImage(unsigned char * pixels);
   int SetImage(unsigned int * pixels);
   int DisplayImage();
   int SetPixelsTo(unsigned char intensity);
   int SetPixelsTo(unsigned char red, unsigned char green, unsigned char blue);
   int SetExposure(double interval_ms);
   double GetExposure();
   unsigned GetWidth();
   unsigned GetHeight();
   unsigned GetNumberOfComponents();
   unsigned GetBytesPerPixel();
   int IsSLMSequenceable(bool& isSequenceable);
   int GetSLMSequenceMaxLength(long& nrEvents);
   int StartSLMSequence();
   int StopSLMSequence();
   int ClearSLMSequence();
   int AddToSLMSequence(const unsigned char * pixels);
   int AddToSLMSequence(const unsigned int * pixels);
   int SendSLMSequence();
};
