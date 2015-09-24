/*
 * Autofocus device instance wrapper
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

#include "AutoFocusInstance.h"


int AutoFocusInstance::SetContinuousFocusing(bool state) { return GetImpl()->SetContinuousFocusing(state); }
int AutoFocusInstance::GetContinuousFocusing(bool& state) { return GetImpl()->GetContinuousFocusing(state); }
bool AutoFocusInstance::IsContinuousFocusLocked() { return GetImpl()->IsContinuousFocusLocked(); }
int AutoFocusInstance::FullFocus() { return GetImpl()->FullFocus(); }
int AutoFocusInstance::IncrementalFocus() { return GetImpl()->IncrementalFocus(); }
int AutoFocusInstance::GetLastFocusScore(double& score) { return GetImpl()->GetLastFocusScore(score); }
int AutoFocusInstance::GetCurrentFocusScore(double& score) { return GetImpl()->GetCurrentFocusScore(score); }
int AutoFocusInstance::AutoSetParameters() { return GetImpl()->AutoSetParameters(); }
int AutoFocusInstance::GetOffset(double &offset) { return GetImpl()->GetOffset(offset); }
int AutoFocusInstance::SetOffset(double offset) { return GetImpl()->SetOffset(offset); }
