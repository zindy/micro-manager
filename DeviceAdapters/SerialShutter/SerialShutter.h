///////////////////////////////////////////////////////////////////////////////
// FILE:          SerialShutter.h
// PROJECT:       Micro-Manager
// SUBSYSTEM:     DeviceAdapters
//-----------------------------------------------------------------------------
// DESCRIPTION:   Serial Port Device Adapter for communication with a serial
//                port connected to a device without a specialized device adapter
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
// AUTHOR:        Karl Hoover
//
// CVS:           $Id: SerialShutter.h 4054 2010-02-25 21:59:20Z karlh $
//

#ifndef _FREESERIALPORT_H_
#define _FREESERIALPORT_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include <string>
#include <map>

#ifdef WIN32
#include <windows.h>
#endif

//////////////////////////////////////////////////////////////////////////////
// Error codes
//
#define ERR_PORT_NOT_FOUND     1000
#define ERR_INVALID_SHUTTER_STATE    10011
#define ERR_INVALID_SHUTTER_NUMBER   10012


//////////////////////////////////////////////////////////////////////////////
// Implementation of the MMDevice interface
//

class SerialShutter : public CShutterBase<SerialShutter>  
{
public:
   SerialShutter();
   ~SerialShutter();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   // Shutter API
   int SetOpen(bool open = true);
   int GetOpen(bool& open);
   int Fire(double deltaT);

   void GetName(char* pszName) const;
   bool Busy() {return busy_;}
   
   // action interface
   // ----------------
   // serial port device - this is a pre-initialization setting
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnLine(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnLogic(MM::PropertyBase* pPropt, MM::ActionType eAct);

   // reflect the pre-initialization property so user can see it
   int OnShowPort(MM::PropertyBase* pProp, MM::ActionType eAct);


private:
   SerialShutter& operator=(SerialShutter& /*rhs*/) {assert(false); return *this;}

   int SetShutterPosition(bool state);
   int GetShutterPosition(bool& state);


   std::string port_;
   std::string line_;
   bool invertedLogic_;

   bool busy_;
   bool initialized_;
   bool state_;

   //File creation returns either a HANDLE or an int...
#ifdef WIN32
   HANDLE handle_;
#else
   int fd_;
#endif

};


#endif //_FREESERIALPORT_H_
