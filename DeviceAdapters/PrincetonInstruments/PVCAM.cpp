/*
 * PVCAM camera module
 *
 * AUTHOR:
 * Nico Stuurman, Nenad Amodaj nenad@amodaj.com, 09/13/2005
 *
 * Copyright (c) 2006 Regents of the University of California
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

/*
 * NOTES:
 *  NOTE;          This file is obsolete. For extensions and new development
 *                 use PVCAMUniversal.cpp. N.A. 01/17/2007
 */

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma warning(disable : 4996) // disable warning for deperecated CRT functions on Windows 
#endif

#include "../../MMDevice/ModuleInterface.h"
#include "PVCAMInt.h"

#ifdef WIN32
#include "master.h"
#include "pvcam.h"
#endif

#ifdef __APPLE__
#define __mac_os_x
#include <PVCAM/master.h>
#include <PVCAM/pvcam.h>
#endif

#ifdef linux
#include <pvcam/master.h>
#include <pvcam/pvcam.h>
#endif

#include <string>
#include <sstream>
#include <iomanip>


using namespace std;

// global constants
const char* g_DeviceUniversal_1 = "Camera-1";
const char* g_DeviceUniversal_2 = "Camera-2";

const char* g_PixelType_8bit = "8bit";
const char* g_PixelType_10bit = "10bit";
const char* g_PixelType_12bit = "12bit";
const char* g_PixelType_14bit = "14bit";
const char* g_PixelType_16bit = "16bit";
const char* g_ReadoutRate = "ReadoutRate";
const char* g_ReadoutRate_Slow = "Slow";
const char* g_ReadoutRate_Fast = "Fast";
const char* g_ReadoutPort = "Port";
const char* g_ReadoutPort_Normal = "Normal";
const char* g_ReadoutPort_Multiplier = "EM";
const char* g_ReadoutPort_LowNoise = "LowNoise";
const char* g_ReadoutPort_HighCap = "HighCap";


///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////
MODULE_API void InitializeModuleData()
{
   RegisterDevice(g_DeviceUniversal_1, MM::CameraDevice, "Princeton Instruments interface - camera slot 1");
   RegisterDevice(g_DeviceUniversal_2, MM::CameraDevice, "Princeton Instruments interface - camera slot 2");
}

MODULE_API void DeleteDevice(MM::Device* pDevice)
{
   delete pDevice;
}

MODULE_API MM::Device* CreateDevice(const char* deviceName)
{
   if (deviceName == 0)
      return 0;
   
   if (strcmp(deviceName, g_DeviceUniversal_1) == 0)
      return new Universal(0);
   else if (strcmp(deviceName, g_DeviceUniversal_2) == 0)
      return new Universal(1);
   
   return 0;
}
