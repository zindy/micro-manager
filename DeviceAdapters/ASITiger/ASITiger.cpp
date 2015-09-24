/*
 * ASI Tiger MODULE_API items and ASIUtility class
 * Note this is for the "Tiger" MM set of adapters, which should
 * work for more than just the TG-1000 "Tiger" controller
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

#ifdef WIN32
#define snprintf _snprintf 
#pragma warning(disable: 4355)
#endif

#include "ASITiger.h"
#include "ASITigerComm.h"
#include "ASIXYStage.h"
#include "ASIZStage.h"
#include "ASIClocked.h"
#include "ASIFWheel.h"
#include "ASIScanner.h"
#include "ASIPiezo.h"
#include "ASICRISP.h"
#include "ASILED.h"
#include "ASIPLogic.h"
#include <cstdio>
#include <string>
#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// TODO add in support for other devices, each time modifying these places
//    name constant declarations in the corresponding .h file
//    MODULE_API MM::Device* CreateDevice(const char* deviceName) in this file
//    DetectInstalledDevices in TigerComm (or other hub)


///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////

MODULE_API void InitializeModuleData()
{
   RegisterDevice(g_TigerCommHubName, MM::HubDevice, g_TigerCommHubDescription);
}


MODULE_API MM::Device* CreateDevice(const char* deviceName)
{
   string deviceStr = deviceName;
   if (deviceName == 0)
      return 0;
   else if (strcmp(deviceName, g_TigerCommHubName) == 0)
      return new CTigerCommHub;
   else if (deviceStr.compare(0, strlen(g_XYStageDeviceName), (string)g_XYStageDeviceName) == 0)
         return new CXYStage(deviceName);
   else if (deviceStr.compare(0, strlen(g_ZStageDeviceName), (string)g_ZStageDeviceName) == 0)
         return new CZStage(deviceName);
   else if (deviceStr.compare(0, strlen(g_FSliderDeviceName), (string)g_FSliderDeviceName) == 0)
      return new CFSlider(deviceName);
   else if (deviceStr.compare(0, strlen(g_TurretDeviceName), (string)g_TurretDeviceName) == 0)
      return new CTurret(deviceName);
   else if (deviceStr.compare(0, strlen(g_FWheelDeviceName), (string)g_FWheelDeviceName) == 0)
      return new CFWheel(deviceName);
   else if (deviceStr.compare(0, strlen(g_ScannerDeviceName), (string)g_ScannerDeviceName) == 0)
      return new CScanner(deviceName);
   else if (deviceStr.compare(0, strlen(g_MMirrorDeviceName), (string)g_MMirrorDeviceName) == 0)
         return new CScanner(deviceName);  // this for compatibility with old config files
   else if (deviceStr.compare(0, strlen(g_PiezoDeviceName), (string)g_PiezoDeviceName) == 0)
      return new CPiezo(deviceName);
   else if (deviceStr.compare(0, strlen(g_CRISPDeviceName), (string)g_CRISPDeviceName) == 0)
      return new CCRISP(deviceName);
   else if (deviceStr.compare(0, strlen(g_LEDDeviceName), (string)g_LEDDeviceName) == 0)
      return new CLED(deviceName);
   else if (deviceStr.compare(0, strlen(g_PLogicDeviceName), (string)g_PLogicDeviceName) == 0)
      return new CPLogic(deviceName);
   else
      return 0;
}

MODULE_API void DeleteDevice(MM::Device* pDevice)
{
   delete pDevice;
}

