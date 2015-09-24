/*
 * This is the root class for Micro-Manager device adapters for
 * the X-Cite Illuminators
 *
 * AUTHOR:
 * Mark Allen Neil, markallenneil@yahoo.com, Dec-2010
 *
 * Copyright (c) 2010 Mission Bay Imaging
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

#include "XCite120PC.h"
#include "XCiteExacte.h"
#include "../../MMDevice/ModuleInterface.h"

#include <string>
#include <math.h>
#include <sstream>
#include <iostream>

using namespace std;

// External names used used by the rest of the system to load devices
const char* g_XCite120PC_Name = "XCite-120PC";
const char* g_XCiteExacte_Name = "XCite-Exacte";

// Device descriptions
const char* g_XCite120PC_Desc = "X-Cite 120PC";
const char* g_XCiteExacte_Desc = "X-Cite Exacte";

// Other string constants
const char* g_Undefined = "Undefined";


///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////
MODULE_API void InitializeModuleData()
{
   RegisterDevice(g_XCite120PC_Name, MM::ShutterDevice, g_XCite120PC_Desc);
   RegisterDevice(g_XCiteExacte_Name, MM::ShutterDevice, g_XCiteExacte_Desc);
}

MODULE_API MM::Device* CreateDevice(const char* deviceName)
{
   if (deviceName == 0)
      return 0;
   else if (strcmp(deviceName, g_XCite120PC_Name) == 0)
      return new XCite120PC(g_XCite120PC_Name);
   else if (strcmp(deviceName, g_XCiteExacte_Name) == 0)
      return new XCiteExacte(g_XCiteExacte_Name);
   else   
      return 0;
}

MODULE_API void DeleteDevice(MM::Device* pDevice)
{
   delete pDevice;
}
