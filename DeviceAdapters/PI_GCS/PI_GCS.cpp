/*
 * PI GCS Controller Driver
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, 08/28/2006
 * Steffen Rau, s.rau@pi.ws, 10/03/2008
 *
 * Copyright (c) 2006 Regents of the University of California
 * Copyright (c) 2008 Physik Instrumente (PI) GmbH & Co. KG
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

#include "PI_GCS.h"
#include <string>
#include <math.h>
#include "../../MMDevice/ModuleInterface.h"
#include <sstream>

const char* g_PI_ZStageDeviceName = "PIZStage";
const char* g_PI_ZStageAxisName = "Axis";
const char* g_PI_ZStageAxisLimitUm = "Limit_um";

const char* g_Yes = "Yes";
const char* g_No = "No";

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////
MODULE_API void InitializeModuleData()
{
   RegisterDevice(g_PI_ZStageDeviceName, MM::StageDevice, "PI GCS Z-stage");
}

MODULE_API MM::Device* CreateDevice(const char* deviceName)
{
   if (deviceName == 0)
      return 0;

   if (strcmp(deviceName, g_PI_ZStageDeviceName) == 0)
   {
      PIZStage* s = new PIZStage();
      return s;
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
   const unsigned int bufSize = 255;
   unsigned char clear[bufSize];                      
   unsigned long read = bufSize;
   int ret;                                                                   
   while (read == bufSize)                                                   
   {                                                                     
      ret = core.ReadFromSerial(&device, port.c_str(), clear, bufSize, read);
      if (ret != DEVICE_OK)                               
         return ret;                                               
   }
   return DEVICE_OK;                                                           
} 
 

///////////////////////////////////////////////////////////////////////////////
// PIZStage

PIZStage::PIZStage() :
   port_("Undefined"),
   stepSizeUm_(0.1),
   initialized_(false),
   answerTimeoutMs_(1000),
   axisLimitUm_(500.0)
{
   InitializeDefaultErrorMessages();

   // create pre-initialization properties
   // ------------------------------------

   // Name
   CreateProperty(MM::g_Keyword_Name, g_PI_ZStageDeviceName, MM::String, true);

   // Description
   CreateProperty(MM::g_Keyword_Description, "Physik Instrumente (PI) GCS Adapter", MM::String, true);

   // Port
   CPropertyAction* pAct = new CPropertyAction (this, &PIZStage::OnPort);
   CreateProperty(MM::g_Keyword_Port, "Undefined", MM::String, false, pAct, true);

   // Axis name
   pAct = new CPropertyAction (this, &PIZStage::OnAxisName);
   CreateProperty(g_PI_ZStageAxisName, "A", MM::String, false, pAct, true);

   // axis limit in um
   pAct = new CPropertyAction (this, &PIZStage::OnAxisLimit);
   CreateProperty(g_PI_ZStageAxisLimitUm, "500.0", MM::Float, false, pAct, true);
}

PIZStage::~PIZStage()
{
   Shutdown();
}

void PIZStage::GetName(char* Name) const
{
   CDeviceUtils::CopyLimitedString(Name, g_PI_ZStageDeviceName);
}

int PIZStage::Initialize()
{
	// test ismoving
	checkIsMoving_ = true;
	Busy();
   // switch on servo, otherwise "MOV" will fail
   ostringstream command;
   command << "SVO " << axisName_<<" 1";
   int ret = SendSerialCommand(port_.c_str(), command.str().c_str(), "\n");
   if (ret != DEVICE_OK)
      return ret;

   CDeviceUtils::SleepMs(10);
   ret = GetPositionSteps(curSteps_);
   if (ret != DEVICE_OK)
      return ret;

   // StepSize
   CPropertyAction* pAct = new CPropertyAction (this, &PIZStage::OnStepSizeUm);
   CreateProperty("StepSizeUm", "0.01", MM::Float, false, pAct);
   stepSizeUm_ = 0.01;

   // axis limits
   pAct = new CPropertyAction (this, &PIZStage::OnPosition);
   CreateProperty(MM::g_Keyword_Position, "0.0", MM::Float, false, pAct);
   SetPropertyLimits(MM::g_Keyword_Position, 0, axisLimitUm_);

   ret = UpdateStatus();
   if (ret != DEVICE_OK)
      return ret;

   initialized_ = true;
   return DEVICE_OK;
}

int PIZStage::Shutdown()
{
   if (initialized_)
   {
      initialized_ = false;
   }
   return DEVICE_OK;
}

bool PIZStage::Busy()
{
   if (!checkIsMoving_)
      return false;

   unsigned char c = (unsigned char) 5;
   // send command
   int ret = WriteToComPort(port_.c_str(), &c, 1);
   if (ret != DEVICE_OK)
      return false;

   // block/wait for acknowledge, or until we time out;
   string answer;
   ret = GetSerialAnswer(port_.c_str(), "\n", answer);
   if (ret != DEVICE_OK)
   {
      // "#5" failed, maybe controller does not support this
      // clear error with two "ERR?"
      GetError();
      GetError();
      checkIsMoving_ = false;
      return false;
   }

   long isMoving;
   if (!GetValue(answer, isMoving))
      return false;

   return (isMoving != 0);
}

int PIZStage::SetPositionSteps(long steps)
{
   double pos = steps * stepSizeUm_;
   return SetPositionUm(pos);
}

int PIZStage::GetPositionSteps(long& steps)
{
   double pos;
   int ret = GetPositionUm(pos);
   if (ret != DEVICE_OK)
      return ret;
   steps = (long) ((pos / stepSizeUm_) + 0.5);
   return DEVICE_OK;
}
  
int PIZStage::SetPositionUm(double pos)
{
   ostringstream command;
   command << "MOV " << axisName_<< " " << pos;

   // send command
   int ret = SendSerialCommand(port_.c_str(), command.str().c_str(), "\n");
   if (ret != DEVICE_OK)
      return ret;

   CDeviceUtils::SleepMs(20);
   // block/wait for acknowledge, or until we time out;
   return GetError();
}

int PIZStage::GetError()
{
   int ret = SendSerialCommand(port_.c_str(), "ERR?", "\n");
   if (ret != DEVICE_OK)
      return ret;
   string answer;
   ret = GetSerialAnswer(port_.c_str(), "\n", answer);
   if (ret != DEVICE_OK)
      return ret;

   int errNo = atoi(answer.c_str());
   if (errNo == 0)
	   return DEVICE_OK;

   return ERR_OFFSET + errNo;   
}

int PIZStage::GetPositionUm(double& pos)
{
   ostringstream command;
   command << "POS? " << axisName_;

   // send command
   int ret = SendSerialCommand(port_.c_str(), command.str().c_str(), "\n");
   if (ret != DEVICE_OK)
      return ret;

   // block/wait for acknowledge, or until we time out;
   string answer;
   ret = GetSerialAnswer(port_.c_str(), "\n", answer);
   if (ret != DEVICE_OK)
      return ret;

   if (!GetValue(answer, pos))
      return ERR_UNRECOGNIZED_ANSWER;

   return DEVICE_OK;
}

int PIZStage::SetOrigin()
{
   return DEVICE_UNSUPPORTED_COMMAND;
}

int PIZStage::GetLimits(double&, double&)
{
   return DEVICE_UNSUPPORTED_COMMAND;
}

///////////////////////////////////////////////////////////////////////////////
// Action handlers
///////////////////////////////////////////////////////////////////////////////

int PIZStage::OnPort(MM::PropertyBase* pProp, MM::ActionType eAct)
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

int PIZStage::OnAxisName(MM::PropertyBase* pProp, MM::ActionType eAct)
{
   if (eAct == MM::BeforeGet)
   {
      pProp->Set(axisName_.c_str());
   }
   else if (eAct == MM::AfterSet)
   {
      pProp->Get(axisName_);
   }

   return DEVICE_OK;
}

int PIZStage::OnStepSizeUm(MM::PropertyBase* pProp, MM::ActionType eAct)
{
   if (eAct == MM::BeforeGet)
   {
      pProp->Set(stepSizeUm_);
   }
   else if (eAct == MM::AfterSet)
   {
      pProp->Get(stepSizeUm_);
   }

   return DEVICE_OK;
}

int PIZStage::OnAxisLimit(MM::PropertyBase* pProp, MM::ActionType eAct)
{
   if (eAct == MM::BeforeGet)
   {
      pProp->Set(axisLimitUm_);
   }
   else if (eAct == MM::AfterSet)
   {
      pProp->Get(axisLimitUm_);
   }

   return DEVICE_OK;
}

int PIZStage::OnPosition(MM::PropertyBase* pProp, MM::ActionType eAct)
{
   if (eAct == MM::BeforeGet)
   {
      double pos;
      int ret = GetPositionUm(pos);
      if (ret != DEVICE_OK)
         return ret;

      pProp->Set(pos);
   }
   else if (eAct == MM::AfterSet)
   {
      double pos;
      pProp->Get(pos);
      int ret = SetPositionUm(pos);
      if (ret != DEVICE_OK)
         return ret;

   }

   return DEVICE_OK;
}


bool PIZStage::GetValue(string& sMessage, double& dval)
{
   if (!ExtractValue(sMessage))
      return false;
   
   char *pend;
   const char* szMessage = sMessage.c_str();
   double dValue = strtod(szMessage, &pend);
   
   // return true only if scan was stopped by spaces, linefeed or the terminating NUL and if the
   // string was not empty to start with
   if (pend != szMessage)
   {
      while( *pend!='\0' && (*pend==' '||*pend=='\n')) pend++;
      if (*pend=='\0')
      {
         dval = dValue;
         return true;
      }
   }
   return false;
}

bool PIZStage::GetValue(string& sMessage, long& lval)
{
   if (!ExtractValue(sMessage))
      return false;

   char *pend;
   const char* szMessage = sMessage.c_str();
   long lValue = strtol(szMessage, &pend, 0);
   
   // return true only if scan was stopped by spaces, linefeed or the terminating NUL and if the
   // string was not empty to start with
   if (pend != szMessage)
   {
      while( *pend!='\0' && (*pend==' '||*pend=='\n')) pend++;
      if (*pend=='\0')
      {
         lval = lValue;
         return true;
      }
   }
   return false;
}

bool PIZStage::ExtractValue(std::string& sMessage)
{
   // value is after last '=', if any '=' is found
   size_t p = sMessage.find_last_of('=');
   if ( p != std::string::npos )
       sMessage.erase(0,p+1);
   
   // trim whitspaces from right ...
   p = sMessage.find_last_not_of(" \t\r\n");
   if (p != std::string::npos)
       sMessage.erase(++p);
   
   // ... and left
   p = sMessage.find_first_not_of(" \n\t\r");
   if (p == std::string::npos)
      return false;
   
   sMessage.erase(0,p);
   return true;
}
