/*
 * AUTHOR:
 * Mark Tsuchida, May 2014
 *
 * Copyright (c) 2014 Regents of the University of California
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

#include "MMDevice.h"

namespace MM {

// Definitions for static const data members.
//
// Note: Do not try to move these initializers to the header. The C++ standard
// allows initializing a static const enum data member inline (inside the class
// definition, where the member is _declared_), but still requires a
// _definition_ (in which case, the definition should not have an initializer).
// However, Microsoft VC++ has a nonstandard extension that allows you to leave
// out the definition altogether, if an initializer is supplied at the
// declaration. Because of that nonstandard behavior, VC++ issues a warning
// (LNK4006) if the initializer is supplied with the declaration _and_ a
// definition is (correctly) provided. So, to compile correctly with a
// standards-confomant compiler _and_ avoid warnings from VC++, we need to
// leave the initializers out of the declarations, and supply them here with
// the definitions. See:
// http://connect.microsoft.com/VisualStudio/feedback/details/802091/lnk4006-reported-for-static-const-members-that-is-initialized-in-the-class-definition

const DeviceType Generic::Type = GenericDevice;
const DeviceType Camera::Type = CameraDevice;
const DeviceType Shutter::Type = ShutterDevice;
const DeviceType Stage::Type = StageDevice;
const DeviceType XYStage::Type = XYStageDevice;
const DeviceType State::Type = StateDevice;
const DeviceType Serial::Type = SerialDevice;
const DeviceType AutoFocus::Type = AutoFocusDevice;
const DeviceType ImageProcessor::Type = ImageProcessorDevice;
const DeviceType SignalIO::Type = SignalIODevice;
const DeviceType Magnifier::Type = MagnifierDevice;
const DeviceType SLM::Type = SLMDevice;
const DeviceType Galvo::Type = GalvoDevice;
const DeviceType Hub::Type = HubDevice;

} // namespace MM
