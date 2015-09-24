/*
 * Signal I/O device instance wrapper
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

#include "SignalIOInstance.h"


int SignalIOInstance::SetGateOpen(bool open) { return GetImpl()->SetGateOpen(open); }
int SignalIOInstance::GetGateOpen(bool& open) { return GetImpl()->GetGateOpen(open); }
int SignalIOInstance::SetSignal(double volts) { return GetImpl()->SetSignal(volts); }
int SignalIOInstance::GetSignal(double& volts) { return GetImpl()->GetSignal(volts); }
int SignalIOInstance::GetLimits(double& minVolts, double& maxVolts) { return GetImpl()->GetLimits(minVolts, maxVolts); }
int SignalIOInstance::IsDASequenceable(bool& isSequenceable) const { return GetImpl()->IsDASequenceable(isSequenceable); }
int SignalIOInstance::GetDASequenceMaxLength(long& nrEvents) const { return GetImpl()->GetDASequenceMaxLength(nrEvents); }
int SignalIOInstance::StartDASequence() { return GetImpl()->StartDASequence(); }
int SignalIOInstance::StopDASequence() { return GetImpl()->StopDASequence(); }
int SignalIOInstance::ClearDASequence() { return GetImpl()->ClearDASequence(); }
int SignalIOInstance::AddToDASequence(double voltage) { return GetImpl()->AddToDASequence(voltage); }
int SignalIOInstance::SendDASequence() { return GetImpl()->SendDASequence(); }
