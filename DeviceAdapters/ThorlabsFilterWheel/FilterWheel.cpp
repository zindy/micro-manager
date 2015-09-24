/*
 * AUTHOR:
 * Brian Ashcroft, ashcroft@physics.leidenuniv.nl 03/14/2009
 * Nenad Amodaj, nenad@amodaj.com, 06/08/2005
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
   #define snprintf _snprintf 
#endif

#include "FilterWheel.h"
#include <cstdio>
#include <string>
#include <math.h>
#include "../../MMDevice/ModuleInterface.h"
#include <sstream>
using namespace std;

// External names used used by the rest of the system
// to load particular device from the "DemoCamera.dll" library

const char* g_WheelDeviceName = "Thorlabs Filter Wheel";


///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////

MODULE_API void InitializeModuleData()
{
   RegisterDevice(g_WheelDeviceName, MM::StateDevice, "Thorlabs filter wheel");
}

MODULE_API MM::Device* CreateDevice(const char* deviceName)
{
   if (deviceName == 0)
      return 0;

 
   if (strcmp(deviceName, g_WheelDeviceName) == 0)
   {
      // create filter wheel
      return new CDemoFilterWheel();
   }


   // ...supplied name not recognized
   return 0;
}

MODULE_API void DeleteDevice(MM::Device* pDevice)
{
   delete pDevice;
}



///////////////////////////////////////////////////////////////////////////////
// CDemoFilterWheel implementation
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

CDemoFilterWheel::CDemoFilterWheel() : 
   numPos_(6), 
   initialized_(false), 
   changedTime_(0.0),
   position_(0),
   port_("")
{
   InitializeDefaultErrorMessages();

   //Com port
   CPropertyAction* pAct = new CPropertyAction (this, &CDemoFilterWheel::OnCOMPort);
   CreateProperty(MM::g_Keyword_Port, "", MM::String, false, pAct, true);

   EnableDelay(); // signals that the dealy setting will be used
}

CDemoFilterWheel::~CDemoFilterWheel()
{
   Shutdown();
}

void CDemoFilterWheel::GetName(char* Name) const
{
   CDeviceUtils::CopyLimitedString(Name, g_WheelDeviceName);
}


int CDemoFilterWheel::Initialize()
{
   if (initialized_)
      return DEVICE_OK;

   // set property list
   // -----------------
   


   // Name
   int ret = CreateProperty(MM::g_Keyword_Name, g_WheelDeviceName, MM::String, true);
   if (DEVICE_OK != ret)
      return ret;

   // Description
   ret = CreateProperty(MM::g_Keyword_Description, "Thorlabs filter wheel driver", MM::String, true);
   if (DEVICE_OK != ret)
      return ret;

   // Set timer for the Busy signal, or we'll get a time-out the first time we check the state of the shutter, for good measure, go back 'delay' time into the past
   changedTime_ = GetCurrentMMTime();   

   // create default positions and labels
   const int bufSize = 1024;
   char buf[bufSize];
   for (long i=0; i<numPos_; i++)
   {
      snprintf(buf, bufSize, "Filter-%ld", i + 1);
      SetPositionLabel(i, buf);
   }

   // State
   // -----
   CPropertyAction* pAct = new CPropertyAction (this, &CDemoFilterWheel::OnState);
   ret = CreateProperty(MM::g_Keyword_State, "0", MM::Integer, false, pAct);
   if (ret != DEVICE_OK)
      return ret;

   // Label
   // -----
   pAct = new CPropertyAction (this, &CStateBase::OnLabel);
   ret = CreateProperty(MM::g_Keyword_Label, "", MM::String, false, pAct);
   if (ret != DEVICE_OK)
      return ret;


   ret = UpdateStatus();
   if (ret != DEVICE_OK)
      return ret;

   initialized_ = true;

   return DEVICE_OK;
}

bool CDemoFilterWheel::Busy()
{
   MM::MMTime interval = GetCurrentMMTime() - changedTime_;
   MM::MMTime delay(GetDelayMs()*1000.0);
   if (interval < delay)
      return true;
   else
      return false;
}


int CDemoFilterWheel::Shutdown()
{
   if (initialized_)
   {
      initialized_ = false;
   }
   return DEVICE_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Action handlers
///////////////////////////////////////////////////////////////////////////////

int CDemoFilterWheel::OnState(MM::PropertyBase* pProp, MM::ActionType eAct)
{
   if (eAct == MM::BeforeGet)
   {
      printf("Getting position of Wheel device\n");
      pProp->Set(position_);
      // nothing to do, let the caller to use cached property
   }
   else if (eAct == MM::AfterSet)
   {
      // Set timer for the Busy signal
      changedTime_ = GetCurrentMMTime();

      long pos;
      pProp->Get(pos);
      //char* deviceName;
      //GetName(deviceName);
      printf("Moving to position %ld\n", position_);
      if (pos >= numPos_ || pos < 0)
      {
         pProp->Set(position_); // revert
         return ERR_UNKNOWN_POSITION;
      }
	  const int bufSize = 20;
      char buf[bufSize];
      snprintf(buf, bufSize, "pos=%ld", pos + 1);
	   
	  SendSerialCommand(port_.c_str(),buf,"\r");
      position_ = pos;
   }

   return DEVICE_OK;
}

int CDemoFilterWheel::OnCOMPort(MM::PropertyBase* pProp, MM::ActionType eAct)
{
   if (eAct == MM::BeforeGet)
   {
      pProp->Set(port_.c_str());
   }
   else if (eAct == MM::AfterSet)
   {
      if (initialized_)
      {
         // revert
         pProp->Set(port_.c_str());
      }

      pProp->Get(port_);                                                     
   }                                                                         
                                                                             
   return DEVICE_OK;                                                         
}  

void CDemoFilterWheel::InitializeFilterWheel()
{   
	
	SendSerialCommand(port_.c_str(),"sensors=0","\r");
	SendSerialCommand(port_.c_str(),"pos?","\r");
    std::string ans="";
	GetSerialAnswer (port_.c_str(), "\n",  ans);
    int i=0;
    from_string<int>(i, (ans).c_str(), std::dec);
    position_=i;

}
