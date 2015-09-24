/*
 * XY Stage device instance wrapper
 *
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

#include "XYStageInstance.h"


int XYStageInstance::SetPositionUm(double x, double y) { return GetImpl()->SetPositionUm(x, y); }
int XYStageInstance::SetRelativePositionUm(double dx, double dy) { return GetImpl()->SetRelativePositionUm(dx, dy); }
int XYStageInstance::SetAdapterOriginUm(double x, double y) { return GetImpl()->SetAdapterOriginUm(x, y); }
int XYStageInstance::GetPositionUm(double& x, double& y) { return GetImpl()->GetPositionUm(x, y); }
int XYStageInstance::GetLimitsUm(double& xMin, double& xMax, double& yMin, double& yMax) { return GetImpl()->GetLimitsUm(xMin, xMax, yMin, yMax); }
int XYStageInstance::Move(double vx, double vy) { return GetImpl()->Move(vx, vy); }
int XYStageInstance::SetPositionSteps(long x, long y) { return GetImpl()->SetPositionSteps(x, y); }
int XYStageInstance::GetPositionSteps(long& x, long& y) { return GetImpl()->GetPositionSteps(x, y); }
int XYStageInstance::SetRelativePositionSteps(long x, long y) { return GetImpl()->SetRelativePositionSteps(x, y); }
int XYStageInstance::Home() { return GetImpl()->Home(); }
int XYStageInstance::Stop() { return GetImpl()->Stop(); }
int XYStageInstance::SetOrigin() { return GetImpl()->SetOrigin(); }
int XYStageInstance::SetXOrigin() { return GetImpl()->SetXOrigin(); }
int XYStageInstance::SetYOrigin() { return GetImpl()->SetYOrigin(); }
int XYStageInstance::GetStepLimits(long& xMin, long& xMax, long& yMin, long& yMax) { return GetImpl()->GetStepLimits(xMin, xMax, yMin, yMax); }
double XYStageInstance::GetStepSizeXUm() { return GetImpl()->GetStepSizeXUm(); }
double XYStageInstance::GetStepSizeYUm() { return GetImpl()->GetStepSizeYUm(); }
int XYStageInstance::IsXYStageSequenceable(bool& isSequenceable) const { return GetImpl()->IsXYStageSequenceable(isSequenceable); }
int XYStageInstance::GetXYStageSequenceMaxLength(long& nrEvents) const { return GetImpl()->GetXYStageSequenceMaxLength(nrEvents); }
int XYStageInstance::StartXYStageSequence() { return GetImpl()->StartXYStageSequence(); }
int XYStageInstance::StopXYStageSequence() { return GetImpl()->StopXYStageSequence(); }
int XYStageInstance::ClearXYStageSequence() { return GetImpl()->ClearXYStageSequence(); }
int XYStageInstance::AddToXYStageSequence(double positionX, double positionY) { return GetImpl()->AddToXYStageSequence(positionX, positionY); }
int XYStageInstance::SendXYStageSequence() { return GetImpl()->SendXYStageSequence(); }
