/*
 * Serial Port Device Adapter for communication with a serial
 * port connected to a device without a specialized device adapter
 *
 * AUTHOR:
 * Karl Hoover
 *
 * Copyright (c) 2010 Regents of the University of California
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

#ifndef _FREESERIALPORT_H_
#define _FREESERIALPORT_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include <string>
#include <map>

//////////////////////////////////////////////////////////////////////////////
// Error codes
//
#define ERR_PORT_NOT_FOUND     1000

//////////////////////////////////////////////////////////////////////////////
// Implementation of the MMDevice interface
//

class FreeSerialPort : public CGenericBase<FreeSerialPort>  
{
public:
   FreeSerialPort();
   ~FreeSerialPort();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy() {return busy_;}
   
   // action interface
   // ----------------
   // serial port device - this is a pre-initialization setting
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);

   // serial port settings - this adapter is unique in that it allows these to be changed on-the-fly
   int OnCommunicationSetting(MM::PropertyBase* pProp, MM::ActionType eAct, long);

   // i/o buffers
   int OnCommand(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnCommandTerminator(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnResponse(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnResponseTerminator(MM::PropertyBase* pProp, MM::ActionType eAct);

   // reflect the pre-initialization property so user can see it
   int OnShowPort(MM::PropertyBase* pProp, MM::ActionType eAct);


private:
   FreeSerialPort& operator=(FreeSerialPort& /*rhs*/) {assert(false); return *this;}

   std::vector < std::pair<std::string, std::string>  > communicationSettings_;

   std::string port_;
   std::string command_; // string to send
   std::string commandTerminator_; // terminator to append to to sent string
   std::string response_;
   std::string responseTerminator_; // response terminator

   bool busy_;
   bool initialized_;

   std::string TokenizeControlCharacters(const std::string ) const;
   std::string DetokenizeControlCharacters(const std::string ) const;

};


#endif //_FREESERIALPORT_H_
