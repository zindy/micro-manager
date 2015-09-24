/*
 * This is the DLL interface file. It provides an interface
 * with the OS when the DLL is loaded and unloaded and an
 * interface to Micro-Manager to discover the available
 * device adapters and methods to create and destroy instances
 * of those adapters.
 *
 * AUTHOR:
 * Terry L. Sprout, Terry.Sprout@Agile-Automation.com
 *
 * Copyright (c) 2014 AgileAutomation, Inc, All rights reserved
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

#include "stdafx.h"
#include "CameraAdapter.h"

// The following data segment is a shared segment for all instances of this DLL.
// It contains a global instance counter that tracks the number of instances
// currently loaded. Some devices only allow a single instance since they control
// hardware that can only be controlled from a single client. The problem is
// a DLL can only recognize when multiple instances of the device are created
// within that instance of the DLL. This instance counter allows other instances
// of the DLL to know when there are instances of the DLL already loaded. The
// current rule is only one instance of this DLL can be used at a time. No devices
// will be created from a second instance of this DLL.
#pragma data_seg( ".sDalShared" )
extern LONG gs_nInstanceCount = -1;
#pragma data_seg()

// External names used by the rest of the system
// to load particular device from the "CameraAdapter.dll" library
static LPCTSTR sc_pszCameraDeviceName = "SPI Cameras";

extern "C" int APIENTRY
DllMain(HINSTANCE /*hInstance*/, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
      // Increment the instance count for this DLL
      InterlockedIncrement( &gs_nInstanceCount );
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
      // We do not want to release the PIL until this DLL is being unloaded.
      // The release function does nothing if the PIL was never connected.
      pilReleaseLib();

      // Decrement the instance count for this DLL
      InterlockedDecrement( &gs_nInstanceCount );
	}
	return 1;   // ok
}

///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////

MODULE_API void InitializeModuleData()
{
   RegisterDevice( sc_pszCameraDeviceName, MM::CameraDevice, "Stanford Photonics cameras" );
}

MODULE_API MM::Device* CreateDevice(const char* deviceName)
{
   if (deviceName == 0)
      return 0;

   // decide which device class to create based on the deviceName parameter
   if (strcmp(deviceName, sc_pszCameraDeviceName) == 0)
   {
      // create camera
      return new CCameraAdapter( sc_pszCameraDeviceName );
   }

   // ...supplied name not recognized
   return 0;
}

MODULE_API void DeleteDevice(MM::Device* pDevice)
{
   delete pDevice;
}
