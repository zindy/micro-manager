/*
 * Vincent Uniblitz controller adapter
 *
 * AUTHOR:
 * Nico Stuurman, 02/27/2006
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

#ifndef _VINCENT_H_
#define _VINCENT_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include <string>
#include <map>

//////////////////////////////////////////////////////////////////////////////
// Error codes
//
#define ERR_UNKNOWN_POSITION         10002
#define ERR_INVALID_SPEED            10003
#define ERR_PORT_CHANGE_FORBIDDEN    10004
#define ERR_SET_POSITION_FAILED      10005
#define ERR_INVALID_STEP_SIZE        10006
#define ERR_LOW_LEVEL_MODE_FAILED    10007
#define ERR_INVALID_MODE             10008

class VincentD1 : public CShutterBase<VincentD1>
{
public:
   VincentD1();
   ~VincentD1();
  
   // Device API
   // ----------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();
   //MM::DeviceType GetType() const {return MM::GenericDevice;}

   // Shutter API
   int SetOpen(bool open = true);
   int GetOpen(bool& open);
   int Fire(double deltaT);

   // action interface
   // ----------------
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnCommand(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAddress(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnOpeningTime(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnClosingTime(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnShutterName(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   int ExecuteCommand(const std::string& cmd);

   bool initialized_;
   // MMCore name of serial port
   std::string port_;
   // Command exchange with MMCore
   std::string command_;
   // Last command sent to the controller
   std::string lastCommand_;
   // address of controller on serial chain (x, 0-7)
   std::string address_;
   // Time it takes after issuing Close command to close the shutter
   double closingTimeMs_;
   // Time it takes after issuing Open command to open the shutter
   double openingTimeMs_;
   // Are we operating shutter A or shutter B?
   std::string shutterName_;
   // Time that last command was sent to shutter
   MM::MMTime changedTime_;
};


class VincentD3 : public CShutterBase<VincentD3>
{
public:
   VincentD3();
   ~VincentD3();
  
   // Device API
   // ----------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();
   //MM::DeviceType GetType() const {return MM::GenericDevice;}

   // Shutter API
   int SetOpen(bool open = true);
   int GetOpen(bool& open);
   int Fire(double deltaT);

   // action interface
   // ----------------
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnCommand(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAddress(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnOpeningTime(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnClosingTime(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnShutterName(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   int ExecuteCommand(const std::string& cmd);

   bool initialized_;
   // MMCore name of serial port
   std::string port_;
   // Command exchange with MMCore
   std::string command_;
   // Last command sent to the controller
   std::string lastCommand_;
   // address of controller on serial chain (x, 0-7)
   std::string address_;
   // Time it takes after issuing Close command to close the shutter
   double closingTimeMs_;
   // Time it takes after issuing Open command to open the shutter
   double openingTimeMs_;
   // Ch1-3
   std::string shutterName_;
   // Time that last command was sent to shutter
   MM::MMTime changedTime_;
};
#endif //_VINCENT_H_
