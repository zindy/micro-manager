/*
 * RND's WTR controller adapter
 *
 * AUTHOR:
 * Vikram Kopuri, based on Code by Nenad Amodaj Nico Stuurman and Jizhen Zhao
 *
 * Copyright (c)  Applied Scientific Instrumentation, Eugene OR
 * Copyright (c)  Robots and Design Co, Ltd.
 * Copyright (c)  Regents of the University of California
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
   //#include <windows.h>
   #define snprintf _snprintf 
#endif

#include "wptr.h"
#include <string>
#include <math.h>
#include "../../MMDevice/ModuleInterface.h"
#include "../../MMDevice/DeviceUtils.h"
#include <sstream>
#include <iostream>

using namespace std;

const char* g_WPTRobotName = "WPTRobot";



///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////
MODULE_API void InitializeModuleData()
{
   RegisterDevice(g_WPTRobotName, MM::GenericDevice, "WPTRobot");
}

MODULE_API MM::Device* CreateDevice(const char* deviceName)
{
   if (deviceName == 0)
      return 0;

  if (strcmp(deviceName, g_WPTRobotName) == 0)
   {
      return  new WPTRobot();
   }

   return 0;
}

MODULE_API void DeleteDevice(MM::Device* pDevice)
{
   delete pDevice;
}

// General utility function:
int ClearPort(MM::Device& device, MM::Core& core, std::string port)
{
   // Clear contents of serial port 
   const int bufSize = 255;
   unsigned char clear[bufSize];                                                        
   unsigned long read = bufSize;                                               
   int ret;                                                                    
   while ((int) read == bufSize)                                                     
   {                                                                           
      ret = core.ReadFromSerial(&device, port.c_str(), clear, bufSize, read); 
      if (ret != DEVICE_OK)                                                    
         return ret;                                                           
   }                                                                           
   return DEVICE_OK;                                                           
} 


///////////////////////////////////////////////////////////////////////////////
WPTRobot::WPTRobot() :
CGenericBase<WPTRobot>(),
   initialized_(false),
 // port_("Undefined"),
   stage_(1),
   slot_(1)
  
   
{
   InitializeDefaultErrorMessages();

   // create pre-initialization properties
   // ------------------------------------

   // Name
   CreateProperty(MM::g_Keyword_Name, g_WPTRobotName, MM::String, true);

   // Description
   CreateProperty(MM::g_Keyword_Description, "Wellplate Transfer Robot", MM::String, true);

   // Port
   CPropertyAction* pAct = new CPropertyAction (this, &WPTRobot::OnPort);
   CreateProperty(MM::g_Keyword_Port, "Undefined", MM::String, false, pAct, true);
 
}

WPTRobot::~WPTRobot()
{
   Shutdown();
}

void WPTRobot::GetName(char* Name) const
{
   CDeviceUtils::CopyLimitedString(Name, g_WPTRobotName);
}

int WPTRobot::Initialize()
{
	  // empty the Rx serial buffer before sending command
      ClearPort(*this, *GetCoreCallback(), port_.c_str()); 
	// Stage and Slot passed as properties
      CPropertyAction* pAct = new CPropertyAction (this, &WPTRobot::OnStage);
      CreateProperty("Stage", "1", MM::Integer, false, pAct);
   //   SetPropertyLimits("Stage", 1, 3); //settings limits must be edited again, if set up changed 

      pAct = new CPropertyAction (this, &WPTRobot::OnSlot);
      CreateProperty("Slot", "1", MM::Integer, false, pAct);
   //   SetPropertyLimits("Slot", 1, 10);
	// setting this property to different keywords leads to a command
	  pAct = new CPropertyAction (this, &WPTRobot::OnCommand);
      CreateProperty("Command", "Undefined", MM::String, false, pAct);
      

 

   int ret = UpdateStatus();
   if (ret != DEVICE_OK)
      return ret;

   initialized_ = true;
   return DEVICE_OK;
}

int WPTRobot::Shutdown()
{
   if (initialized_)
   {
      initialized_ = false;
   }
   return DEVICE_OK;
}

bool WPTRobot::Busy()
{
   // reply is only given after move is completed, so busy() not necessary
   return false;
 
}

int WPTRobot::OnPort(MM::PropertyBase* pProp, MM::ActionType eAct)
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
         return ERR_PORT_CHANGE_FORBIDDEN;
      }

      pProp->Get(port_);
   }

   return DEVICE_OK;
}

int WPTRobot::OnStage(MM::PropertyBase* pProp, MM::ActionType eAct)
{//Just reading and writing to the stage variable
   if (eAct == MM::BeforeGet)
   {
      pProp->Set(stage_);
   }
   else if (eAct == MM::AfterSet)
   {
	      pProp->Get(stage_);
   }
 
	return DEVICE_OK;

}

int WPTRobot::OnSlot(MM::PropertyBase* pProp, MM::ActionType eAct)
{//Just reading and writing to the slot variable
   if (eAct == MM::BeforeGet)
   {
      pProp->Set(slot_);
   }
   else if (eAct == MM::AfterSet)
   {
	      pProp->Get(slot_);
   }
 

  return DEVICE_OK;
}


int WPTRobot::OnCommand(MM::PropertyBase* pProp, MM::ActionType eAct)
{

	if (eAct == MM::BeforeGet)
   {
      pProp->Set(command_.c_str());
   }
   else if (eAct == MM::AfterSet)
   {// Read what keyword the user issued, and send the corresponding cmd
      pProp->Get(command_);

	  ostringstream os;
	  int ret;
	  string answer;	

	 //if its ORG cmd
	  if(command_.substr(0,3)=="ORG") // user issued ORG command
	  {
	  os<<"ORG";	
	  ret = SendSerialCommand(port_.c_str(), os.str().c_str(), "\r\n");
	   if (ret != DEVICE_OK) //checking if sendserialcmd was exected without errors
         return ret;
      

     
      ret = GetSerialAnswer(port_.c_str(), "\r\n", answer);//waiting for reply
	   if (ret != DEVICE_OK)//checking if GetSerialAnswer was exected without errors
         return ret;
      

		if(answer.length()!=3 )//checking if the answer is what we are expecting
			return ERR_UNRECOGNIZED_ANSWER;// if not just give an error and quit
		if(answer.substr(0, 3).compare("ORG") != 0)
			return ERR_UNRECOGNIZED_ANSWER;
	  }
	  else if(command_.substr(0,3)=="GET")//user issued GET
	  {
	  os<<"GET "<<stage_<<","<<slot_; // Crafting the cmd to be sent to serial port

	  ret = SendSerialCommand(port_.c_str(), os.str().c_str(), "\r\n");
	   if (ret != DEVICE_OK)
         return ret;
      
     
      ret = GetSerialAnswer(port_.c_str(), "\r\n", answer);
	   if (ret != DEVICE_OK)
         return ret;
     
		if(answer.length()!=3 )	
			return ERR_UNRECOGNIZED_ANSWER;
		if(answer.substr(0, 3).compare("GET") != 0)
			return ERR_UNRECOGNIZED_ANSWER;

	  }
	  else if(command_.substr(0,3)=="PUT")//user issued PUT
	  {
	  os<<"PUT "<<stage_<<","<<slot_;

	  ret = SendSerialCommand(port_.c_str(), os.str().c_str(), "\r\n");
       if (ret != DEVICE_OK)
         return ret;

     
      ret = GetSerialAnswer(port_.c_str(), "\r\n", answer);
	   if (ret != DEVICE_OK)
         return ret;
      
		if(answer.length()!=3)	
			return ERR_UNRECOGNIZED_ANSWER;
		if(answer.substr(0, 3).compare("PUT") != 0)
			return ERR_UNRECOGNIZED_ANSWER;

	 }
	  else if(command_.substr(0,3)=="AES")// used issued STOP,
	  {
	  os<<"AES";// AES is cmd for Emergency stop, stops the robot cold, issue DRT cmd to enable again.	
	  ret = SendSerialCommand(port_.c_str(), os.str().c_str(), "\r\n");
	   if (ret != DEVICE_OK)
         return ret;
      

     
      ret = GetSerialAnswer(port_.c_str(), "\r\n", answer);
	   if (ret != DEVICE_OK)
         return ret;
      

		if(answer.length()!=3 )
			return ERR_UNRECOGNIZED_ANSWER;
		if(answer.substr(0, 3).compare("AES") != 0)
			return ERR_UNRECOGNIZED_ANSWER;
	  }
	  else if(command_.substr(0,3)=="DRT")// used issued STOP,
	  {
	  os<<"DRT";//used to over ride errors
	  ret = SendSerialCommand(port_.c_str(), os.str().c_str(), "\r\n");
	   if (ret != DEVICE_OK)
         return ret;
      

     
      ret = GetSerialAnswer(port_.c_str(), "\r\n", answer);
	   if (ret != DEVICE_OK)
         return ret;
      

		if(answer.length()!=3 )
			return ERR_UNRECOGNIZED_ANSWER;
		if(answer.substr(0, 3).compare("DRT") != 0)
			return ERR_UNRECOGNIZED_ANSWER;
	  }


   }
   return DEVICE_OK;
}

