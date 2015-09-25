/*
 * Copyright (c) 2008, Mad City Labs Inc.
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
File:		MCL_MicroDrive.cpp
Copyright:	Mad City Labs Inc., 2008
License:	Distributed under the BSD license.
*/

#include "MCL_MicroDrive_ZStage.h"
#include "MicroDriveXYStage.h"
#include "../../MMDevice/ModuleInterface.h"
#include "heap.h"
#include "handle_list_if.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
 
extern HANDLE gHeap;

BOOL APIENTRY DllMain( HANDLE /*hModule*/, 
                      DWORD  ul_reason_for_call, 
                      LPVOID /*lpReserved*/)
{
   	switch (ul_reason_for_call)  
   	{
   		case DLL_PROCESS_ATTACH:
			if(!GlobalHeapInit())
				return false;

			if(!MCL_InitLibrary(::gHeap))
				return false;

			if(!HandleListCreate())
				return false;

			break;
  		case DLL_THREAD_ATTACH:
			break;
   		case DLL_THREAD_DETACH:
			break;
   		case DLL_PROCESS_DETACH:

			HandleListDestroy();

			MCL_ReleaseLibrary();

			GlobalHeapDestroy();

   			break;
   	}
    
	return TRUE;
}

MODULE_API void InitializeModuleData()
{
   RegisterDevice(g_XYStageDeviceName, MM::XYStageDevice, "XY positioning");
   RegisterDevice(g_StageDeviceName, MM::StageDevice, "Z positioning");
}

MODULE_API MM::Device* CreateDevice(const char* deviceName)
{
   if (deviceName == NULL)
      return 0;

   if(strcmp(deviceName, g_StageDeviceName) == 0)
	   return new MCL_MicroDrive_ZStage();

   if (strcmp(deviceName, g_XYStageDeviceName) == 0) 
	   return new MicroDriveXYStage();
   
   // ...supplied name not recognized
   return 0;
}

MODULE_API void DeleteDevice(MM::Device* pDevice)
{
   delete pDevice;
}
