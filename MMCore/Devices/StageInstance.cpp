/*
 * Stage device instance wrapper
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

#include "StageInstance.h"


int StageInstance::SetPositionUm(double pos) { return GetImpl()->SetPositionUm(pos); }
int StageInstance::SetRelativePositionUm(double d) { return GetImpl()->SetRelativePositionUm(d); }
int StageInstance::Move(double velocity) { return GetImpl()->Move(velocity); }
int StageInstance::Stop() { return GetImpl()->Stop(); }
int StageInstance::Home() { return GetImpl()->Home(); }
int StageInstance::SetAdapterOriginUm(double d) { return GetImpl()->SetAdapterOriginUm(d); }
int StageInstance::GetPositionUm(double& pos) { return GetImpl()->GetPositionUm(pos); }
int StageInstance::SetPositionSteps(long steps) { return GetImpl()->SetPositionSteps(steps); }
int StageInstance::GetPositionSteps(long& steps) { return GetImpl()->GetPositionSteps(steps); }
int StageInstance::SetOrigin() { return GetImpl()->SetOrigin(); }
int StageInstance::GetLimits(double& lower, double& upper) { return GetImpl()->GetLimits(lower, upper); }

MM::FocusDirection
StageInstance::GetFocusDirection()
{
   // Default to what the device adapter says.
   if (!focusDirectionHasBeenSet_)
   {
      MM::FocusDirection direction;
      int err = GetImpl()->GetFocusDirection(direction);
      ThrowIfError(err, "Cannot get focus direction");

      focusDirection_ = direction;
      focusDirectionHasBeenSet_ = true;
   }
   return focusDirection_;
}

void
StageInstance::SetFocusDirection(MM::FocusDirection direction)
{
   focusDirection_ = direction;
   focusDirectionHasBeenSet_ = true;
}

int StageInstance::IsStageSequenceable(bool& isSequenceable) const { return GetImpl()->IsStageSequenceable(isSequenceable); }
bool StageInstance::IsContinuousFocusDrive() const { return GetImpl()->IsContinuousFocusDrive(); }
int StageInstance::GetStageSequenceMaxLength(long& nrEvents) const { return GetImpl()->GetStageSequenceMaxLength(nrEvents); }
int StageInstance::StartStageSequence() { return GetImpl()->StartStageSequence(); }
int StageInstance::StopStageSequence() { return GetImpl()->StopStageSequence(); }
int StageInstance::ClearStageSequence() { return GetImpl()->ClearStageSequence(); }
int StageInstance::AddToStageSequence(double position) { return GetImpl()->AddToStageSequence(position); }
int StageInstance::SendStageSequence() { return GetImpl()->SendStageSequence(); }
