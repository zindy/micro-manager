/*
 * Thorlabs device adapters: BBD Controller
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, 2011
 *
 * Copyright (c) 2011 Thorlabs
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
   #include <windows.h>
   #define snprintf _snprintf 
#endif

#include "Thorlabs.h"
#include "XYStage.h"
#include "MotorZStage.h"
#include "PiezoZStage.h"
#include "IntegratedFilterWheel.h"
#include <ModuleInterface.h>
#include <MMDevice.h>
#include <cstdio>
#include <string>
#include <math.h>
#include <sstream>

///////////
// properties
///////////

const char* g_XYStageDeviceName = "XYStage";
const char* g_PiezoZStageDeviceName = "PiezoZStage";
const char* g_MotorZStageDeviceName = "MotorZStage";
const char* g_WheelDeviceName = "FilterWheel";

const char* g_SerialNumberProp = "SerialNumber";
const char* g_ModelNumberProp = "ModelNumber";
const char* g_SWVersionProp = "SoftwareVersion";

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Utility
///////////////////////////////////////////////////////////////////////////////

/**
 * Clears receive buffer of any content.
 * To be used before sending commands to make sure that we are not catching
 * residual error messages or previous unhandled responses.
 */
int ClearPort(MM::Device& device, MM::Core& core, std::string port)
{
   // Clear contents of serial port 
   const int bufSize = 255;
   unsigned char clear[bufSize];
   unsigned long read = bufSize;
   while ((int) read == bufSize)
   {
      int ret = core.ReadFromSerial(&device, port.c_str(), clear, bufSize, read);
      if (ret != DEVICE_OK)
         return ret;
   }
   return DEVICE_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////
MODULE_API void InitializeModuleData()
{
   RegisterDevice(g_XYStageDeviceName, MM::XYStageDevice, "Thorlabs BD102 XY Stage");
   RegisterDevice(g_PiezoZStageDeviceName, MM::StageDevice, "Thorlabs piezo Z Stage");
   RegisterDevice(g_MotorZStageDeviceName, MM::StageDevice, "Thorlabs Motor Z Stage");
   RegisterDevice(g_WheelDeviceName, MM::StateDevice, "Integrated filter wheel");
}

MODULE_API MM::Device* CreateDevice(const char* deviceName)
{
   if (deviceName == 0)
      return 0;

   if (strcmp(deviceName, g_XYStageDeviceName) == 0)
   {
      XYStage* xyStage = new XYStage();
      return xyStage;
   }
   if (strcmp(deviceName, g_PiezoZStageDeviceName) == 0)
   {
      PiezoZStage* stage = new PiezoZStage();
      return stage;
   }
   if (strcmp(deviceName, g_MotorZStageDeviceName) == 0)
   {
      MotorZStage* stage = new MotorZStage();
      return stage;
   }
   if (strcmp(deviceName, g_WheelDeviceName) == 0)
   {
      IntegratedFilterWheel* wheel = new IntegratedFilterWheel();
      return wheel;
   }

   return 0;
}

MODULE_API void DeleteDevice(MM::Device* pDevice)
{
   delete pDevice;
}

