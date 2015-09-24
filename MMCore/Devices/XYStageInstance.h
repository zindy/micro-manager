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


class XYStageInstance : public DeviceInstanceBase<MM::XYStage>
{
public:
   XYStageInstance(CMMCore* core,
         boost::shared_ptr<LoadedDeviceAdapter> adapter,
         const std::string& name,
         MM::Device* pDevice,
         DeleteDeviceFunction deleteFunction,
         const std::string& label,
         mm::logging::Logger deviceLogger,
         mm::logging::Logger coreLogger) :
      DeviceInstanceBase<MM::XYStage>(core, adapter, name, pDevice, deleteFunction, label, deviceLogger, coreLogger)
   {}

   int SetPositionUm(double x, double y);
   int SetRelativePositionUm(double dx, double dy);
   int SetAdapterOriginUm(double x, double y);
   int GetPositionUm(double& x, double& y);
   int GetLimitsUm(double& xMin, double& xMax, double& yMin, double& yMax);
   int Move(double vx, double vy);
   int SetPositionSteps(long x, long y);
   int GetPositionSteps(long& x, long& y);
   int SetRelativePositionSteps(long x, long y);
   int Home();
   int Stop();
   int SetOrigin();
   int SetXOrigin();
   int SetYOrigin();
   int GetStepLimits(long& xMin, long& xMax, long& yMin, long& yMax);
   double GetStepSizeXUm();
   double GetStepSizeYUm();
   int IsXYStageSequenceable(bool& isSequenceable) const;
   int GetXYStageSequenceMaxLength(long& nrEvents) const;
   int StartXYStageSequence();
   int StopXYStageSequence();
   int ClearXYStageSequence();
   int AddToXYStageSequence(double positionX, double positionY);
   int SendXYStageSequence();
};
