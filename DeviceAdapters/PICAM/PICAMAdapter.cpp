/*
 * PICAM camera module
 *
 * AUTHOR:
 * Toshio Suzuki
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

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "ModuleInterface.h"
#include "PICAMAdapter.h"

#ifdef WIN64
#pragma warning(push)
#include "picam.h"
#pragma warning(pop)
#endif

#ifdef __APPLE__
#define __mac_os_x
#endif

#ifdef linux
#endif

#include <boost/lexical_cast.hpp>

#include <string>
#include <sstream>
#include <iomanip>


using namespace std;

// global constants
#define NUM_CAMERA_IDS 4
// Device names here are derived from the model name and serial number.
char g_DeviceName[NUM_CAMERA_IDS][128];
// Also support existing config files that use just the model name as device
// name.
char g_LegacyDeviceName[NUM_CAMERA_IDS][128];

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
   piint demoList[] = { PicamModel_Pixis1024B, PicamModel_Nirvana640, PicamModel_ProEM1024B, PicamModel_Pylonir102417  };
   const pichar *sn[] = { "1000000001", "1000000002", "1000000003" , "1000000004" };
   piint numCamsAvailable = 0;
   const PicamCameraID *camID;
   piint numDemos = 0;
   PicamCameraID *demoID = NULL;


   /* Initialize PICAM */
   // TODO Strictly speaking, the initialize/uninitialize here
   // should take into account the reference count used by the
   // camera device initialization.
   if (Picam_InitializeLibrary()==PicamError_None){
      Picam_GetAvailableCameraIDs( &camID, &numCamsAvailable );

      // Add demo Camera
      if( numCamsAvailable < NUM_CAMERA_IDS )
      {
         numDemos = NUM_CAMERA_IDS - numCamsAvailable;
         demoID = (PicamCameraID*) malloc( sizeof( PicamCameraID ) * numDemos );
      }
      piint count = 0;
      while( numCamsAvailable < NUM_CAMERA_IDS )
      {
         Picam_ConnectDemoCamera( (PicamModel)demoList[count], sn[count], &demoID[count] );
         ++numCamsAvailable;
         ++count;
      }

      // may get 4 cameras
      Picam_GetAvailableCameraIDs( &camID, &numCamsAvailable );

      const pichar* modelName;

      for (int i = 0; i < NUM_CAMERA_IDS; ++i)
      {
         Picam_GetEnumerationString(PicamEnumeratedType_Model, camID[i].model, &modelName);
         strcpy(g_DeviceName[i],
               (modelName + std::string(" - ") + camID[i].serial_number).c_str());
         strcpy(g_LegacyDeviceName[i], modelName);
         Picam_DestroyString(modelName);

         RegisterDevice(g_DeviceName[i], MM::CameraDevice,
               ("PICam camera " + boost::lexical_cast<std::string>(i)).c_str());
      }

      Picam_DestroyCameraIDs(camID);

      Picam_UninitializeLibrary();
   }
   else{
      strcpy(g_DeviceName[0], "Error1");
      strcpy(g_DeviceName[1], "Error2");
      strcpy(g_DeviceName[2], "Error3");
      strcpy(g_DeviceName[3], "Error4");
      RegisterDevice(g_DeviceName[0], MM::CameraDevice, "Universal PICAM interface - camera 1");
      RegisterDevice(g_DeviceName[1], MM::CameraDevice, "Universal PICAM interface - camera 2");
      RegisterDevice(g_DeviceName[2], MM::CameraDevice, "Universal PICAM interface - camera 3");
      RegisterDevice(g_DeviceName[3], MM::CameraDevice, "Universal PICAM interface - camera 4");
   }
}

MODULE_API void DeleteDevice(MM::Device* pDevice)
{
   delete pDevice;
}


MODULE_API MM::Device* CreateDevice(const char* deviceName)
{
   for (short i = 0; i < 4; ++i)
   {
      if (strcmp(deviceName, g_DeviceName[i]) == 0)
         return new Universal(i, deviceName);
   }
   for (short i = 0; i < 4; ++i)
   {
      if (strcmp(deviceName, g_LegacyDeviceName[i]) == 0)
         return new Universal(i, deviceName);
   }
   return 0;
}
