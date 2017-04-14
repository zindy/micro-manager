///////////////////////////////////////////////////////////////////////////////
// FILE:       SerialShutter.cpp
// PROJECT:    Micro-Manager
// SUBSYSTEM:  DeviceAdapters
//-----------------------------------------------------------------------------
// DESCRIPTION:   DTR / RTS Serial Port Shutter Device Adapter based on
//                Serial Port Device Adapter for free-form communication with serial device
//
// COPYRIGHT:     University of California San Francisco, 2010
// LICENSE:       This file is distributed under the BSD license.
//                License text is included with the source distribution.
//
//                This file is distributed in the hope that it will be useful,
//                but WITHOUT ANY WARRANTY; without even the implied warranty
//                of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//                IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//                CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//                INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES.
// AUTHOR:        Egor Zindy, Karl Hoover
//
// CVS:           $Id: SerialShutter.cpp $
//

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define snprintf _snprintf 
#else
//code lifted from http://stackoverflow.com/questions/27673344
#include <fcntl.h>       /* File Control Definitions           */
#include <termios.h>     /* POSIX Terminal Control Definitions */
#include <unistd.h>      /* UNIX Standard Definitions          */ 
#include <errno.h>       /* ERROR Number Definitions           */
#include <sys/ioctl.h>   /* ioctl()                            */
#endif

#include "SerialShutter.h"
#include "../../MMDevice/ModuleInterface.h"
#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/DeviceUtils.h"
#include "boost/lexical_cast.hpp"


using namespace std;

// global constants
const char* g_DeviceName = "SerialShutter";
#define ERR_PORT_CHANGE_FORBIDDEN    10004
#define ERR_DEVICE_NOT_FOUND         10005

const char* g_Keyword_Line= "Line";
const char* g_normalLogicString = "Normal";
const char* g_invertedLogicString = "Inverted";
const char* g_RTSString = "RTS";
const char* g_DTRString = "DTR";

///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////
MODULE_API void InitializeModuleData()
{
   RegisterDevice(g_DeviceName, MM::ShutterDevice, "USB-Serial-TTL converter as a shutter");
}

MODULE_API MM::Device* CreateDevice(const char* deviceName)
{
	if (deviceName == 0)
		return 0;

	if (strcmp(deviceName, g_DeviceName) == 0)
	{
		return new SerialShutter;
	}
	return 0;
}

MODULE_API void DeleteDevice(MM::Device* pDevice)
{
	delete pDevice;
}

///////////////////////////////////////////////////////////////////////////////
// SerialShutter implementation
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

SerialShutter::SerialShutter() :
   busy_(false),
   initialized_(false),
   state_(0)
{
	InitializeDefaultErrorMessages();

	// add custom error messages
	SetErrorText(ERR_PORT_NOT_FOUND, "No serial port by this name was found");

   // Port
   CPropertyAction* pAct = new CPropertyAction (this, &SerialShutter::OnPort);
   CreateProperty(MM::g_Keyword_Port, "Undefined", MM::String, false, pAct, true);

   // Line
   pAct = new CPropertyAction (this, &SerialShutter::OnLine);
   CreateProperty(g_Keyword_Line, g_RTSString, MM::String, false, pAct, true);
   AddAllowedValue(g_Keyword_Line, g_RTSString);
   AddAllowedValue(g_Keyword_Line, g_DTRString);

   // Logic (borrowed from Arduino.cpp
   pAct = new CPropertyAction(this, &SerialShutter::OnLogic);
   CreateProperty("Logic", g_invertedLogicString, MM::String, false, pAct, true);
   AddAllowedValue("Logic", g_invertedLogicString);
   AddAllowedValue("Logic", g_normalLogicString);
}

SerialShutter::~SerialShutter()
{
	Shutdown();
}

void SerialShutter::GetName(char* Name) const
{
	CDeviceUtils::CopyLimitedString(Name, g_DeviceName);
}


int SerialShutter::Initialize()
{
	// the device initialization will have opened the assigned serial ports...

	// set property list
	// -----------------

	// Name
	int nRet = CreateProperty(MM::g_Keyword_Name, g_DeviceName, MM::String, true);
	if (DEVICE_OK != nRet)
		return nRet;

	// Description
	nRet = CreateProperty(MM::g_Keyword_Description, "Use the RTS or DTR line on a USB-Serial-TTL converter as a shutter device", MM::String, true);

	if (DEVICE_OK != nRet)
		return nRet;

	// create an extended (i.e. array) of properties
   // get the serial device
   MM::Device* pS = GetCoreCallback()->GetDevice(this, port_.c_str());
   pS->Shutdown();
   /*
   if (pS == 0) 
   {
      return ERR_PORT_NOT_FOUND;
   }
   */

   // show port setting in browser
   CPropertyAction* pAct = new CPropertyAction (this, &SerialShutter::OnShowPort);
   CreateProperty("ShowPort", port_.c_str(), MM::String, true, pAct);

   // State
   // -----
   pAct = new CPropertyAction (this, &SerialShutter::OnState);
   int ret = CreateProperty(MM::g_Keyword_State, "0", MM::Integer, false, pAct);
   if (ret != DEVICE_OK)
      return ret;

   AddAllowedValue(MM::g_Keyword_State, "0");
   AddAllowedValue(MM::g_Keyword_State, "1");

#ifdef WIN32
   handle_ = CreateFileA( port_.c_str(),GENERIC_READ | GENERIC_WRITE, 
                        0, 
                        0, 
                        OPEN_EXISTING,
                        NULL,
                        0);

    if (handle_ == INVALID_HANDLE_VALUE)
        return DEVICE_NOT_CONNECTED;

#else
    fd_ = open( port_.c_str(), O_RDWR | O_NOCTTY ); //Opening the serial port

    if (fd_ == -1)
        return DEVICE_NOT_CONNECTED;
#endif

	initialized_ = true;
	return DEVICE_OK;
}

int SerialShutter::Shutdown()
{
	if (initialized_)
	{
		initialized_ = false;
	}

#ifdef WIN32
    CloseHandle(handle_);
#else
    close(fd_);
#endif

	return DEVICE_OK;
}

int SerialShutter::SetOpen(bool open)
{
	int pos;
   int ret;

   //remember the shutter state
   state_ = open;

	if (open)
		pos = 1;
	else
		pos = 0;

   if (invertedLogic_) pos = 1-pos;

#ifdef WIN32
   //code lifted from http://stackoverflow.com/questions/18539104
   DWORD dwFunc;

   if (line_.compare(g_DTRString)==0) {
      dwFunc = (pos == 1)?SETRTS:CLRRTS;
   }
   else { //if (line == DTR) {
      dwFunc = (pos == 1)?SETDTR:CLRDTR;
   }

   ret = EscapeCommFunction(handle_, dwFunc);
   ret = (ret == 0)?DEVICE_NOT_CONNECTED:DEVICE_OK;
#else
   //code lifted from http://stackoverflow.com/questions/27673344
   int status;
   ioctl(fd_,TIOCMGET,&status); /* GET the State of MODEM bits in Status */

   if (line_.compare(g_DTRString)==0) {
      if (pos == 1)
         status |= TIOCM_DTR;        // Set the DTR pin
      else
         status &= ~TIOCM_DTR;
   }
   else {
      if (pos == 1)
         status |= TIOCM_RTS;        // Set the RTS pin
      else
         status &= ~TIOCM_RTS;
   }

   ret = ioctl(fd_, TIOCMSET, &status);
   ret = (ret == -1)?DEVICE_NOT_CONNECTED:DEVICE_OK;
#endif

   return ret;
}

int SerialShutter::GetOpen(bool& open)
{
   //TODO is there a way to read the shutter state from the RTS/DTR lines? Is it worth doing?
   open = state_;
	return DEVICE_OK;
}

int SerialShutter::Fire(double /*deltaT*/)
{
   return DEVICE_UNSUPPORTED_COMMAND;
}

/**
 * Sends an open/close command through the serial port.
 */
int SerialShutter::SetShutterPosition(bool state)
{
   return SetOpen(state);
}

/**
 * Check the state of the shutter.
 */
int SerialShutter::GetShutterPosition(bool& state)
{
   return GetOpen(state);
}

///////////////////////////////////////////////////////////////////////////////
// Action handlers
///////////////////////////////////////////////////////////////////////////////


int SerialShutter::OnPort(MM::PropertyBase* pProp, MM::ActionType eAct)
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


int SerialShutter::OnLine(MM::PropertyBase* pProp, MM::ActionType eAct)
{
   if (eAct == MM::BeforeGet)
   {
      pProp->Set(line_.c_str());
   }
   else if (eAct == MM::AfterSet)
   {
      pProp->Get(line_);
   }

   return DEVICE_OK;
}


// Logic (borrowed from Arduino.cpp}
int SerialShutter::OnLogic(MM::PropertyBase* pProp, MM::ActionType pAct)
{
   if (pAct == MM::BeforeGet)
   {
      if (invertedLogic_)
         pProp->Set(g_invertedLogicString);
      else
         pProp->Set(g_normalLogicString);
   } else if (pAct == MM::AfterSet)
   {
      std::string logic;
      pProp->Get(logic);
      if (logic.compare(g_invertedLogicString)==0)
         invertedLogic_ = true;
      else invertedLogic_ = false;
   }
   return DEVICE_OK;
}

// let user see port setting in browswer
int SerialShutter::OnShowPort(MM::PropertyBase* pProp, MM::ActionType eAct)
{
   int ret = DEVICE_OK;
   if (eAct == MM::BeforeGet)
   {
      pProp->Set(port_.c_str() );
   }
   else if (eAct == MM::AfterSet)
   {
   }

   return ret;
}

int SerialShutter::OnState(MM::PropertyBase* pProp, MM::ActionType eAct)
{
   if (eAct == MM::BeforeGet)
   {
      bool open;
      int ret = GetShutterPosition(open);
      if (ret != DEVICE_OK)
         return ret;
      if (open)
         pProp->Set((long)1);
      else
         pProp->Set((long)0);
   }
   else if (eAct == MM::AfterSet)
   {
      long state;
      pProp->Get(state);
      if (state == 1)
         return SetShutterPosition(true);
      else if (state == 0)
         return SetShutterPosition(false);
      else
         return ERR_INVALID_SHUTTER_STATE;
   }
   return DEVICE_OK;
}

