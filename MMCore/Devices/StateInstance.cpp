/*
 * State device instance wrapper
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

#include "StateInstance.h"


int StateInstance::SetPosition(long pos) { return GetImpl()->SetPosition(pos); }
int StateInstance::SetPosition(const char* label) { return GetImpl()->SetPosition(label); }
int StateInstance::GetPosition(long& pos) const { return GetImpl()->GetPosition(pos); }

std::string StateInstance::GetPositionLabel() const
{
   DeviceStringBuffer labelBuf(this, "GetPosition");
   int err = GetImpl()->GetPosition(labelBuf.GetBuffer());
   ThrowIfError(err, "Cannot get current position label");
   return labelBuf.Get();
}

std::string StateInstance::GetPositionLabel(long pos) const
{
   DeviceStringBuffer labelBuf(this, "GetPositionLabel");
   int err = GetImpl()->GetPositionLabel(pos, labelBuf.GetBuffer());
   ThrowIfError(err, "Cannot get position label at index " + ToString(pos));
   return labelBuf.Get();
}

int StateInstance::GetLabelPosition(const char* label, long& pos) const { return GetImpl()->GetLabelPosition(label, pos); }
int StateInstance::SetPositionLabel(long pos, const char* label) { return GetImpl()->SetPositionLabel(pos, label); }
unsigned long StateInstance::GetNumberOfPositions() const { return GetImpl()->GetNumberOfPositions(); }
int StateInstance::SetGateOpen(bool open) { return GetImpl()->SetGateOpen(open); }
int StateInstance::GetGateOpen(bool& open) { return GetImpl()->GetGateOpen(open); }
