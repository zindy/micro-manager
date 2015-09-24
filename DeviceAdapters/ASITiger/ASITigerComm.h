/*
 * ASI TIGER controller adapter
 *
 * AUTHOR:
 * Jon Daniels (jon@asiimaging.com) 09/2013
 *
 * Copyright (c)  Applied Scientific Instrumentation, Eugene OR
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

#ifndef _ASITigerComm_H_
#define _ASITigerComm_H_

#include "ASIHub.h"
#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"


///////////////////////////////////////////////////////
// ***** TIGER-specific *******************************
///////////////////////////////////////////////////////

// Because the comm card is also a device in the TG-1000 system it would
// be nice if it also went through the ASIPeripheralBase class like the
// other device adapters (and then ASIPeripheralBase and ASIBase could be
// merged).  However, this was impossible because the other peripheral
// adapters need to store a pointer to the hub device.  One solution would
// be to remove ASIHub from the inheritance heirarchy and leave it as a
// utility class, but some of its core commands require it to inherit from
// HubBase in DeviceBase.h, at which point it would have to be merged
// with TigerComm to work.  So that refactoring path was abandoned for now.
// - Jon (14-May-2014)
class CTigerCommHub : public ASIHub
{
public:
   CTigerCommHub();
   ~CTigerCommHub() { Shutdown(); }

   // Device API
   int Initialize();
   bool Busy();

   // Hub API
   MM::DeviceDetectionStatus DetectDevice();
   int DetectInstalledDevices();

private:
   int TalkToTiger();
};



#endif //_ASITigerComm_H_
