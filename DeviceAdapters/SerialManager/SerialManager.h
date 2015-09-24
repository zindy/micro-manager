/*
 * serial port device adapter
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

#pragma once

// Prevent windows.h (through DeviceBase.h) from includeing winsock.h
// before boost/asio.h (which results in an #error).
#define WIN32_LEAN_AND_MEAN

#include "DeviceBase.h"

#ifdef __APPLE__
// OS X 10.5 kqueue does not support serial
// See https://svn.boost.org/trac/boost/ticket/2565 and
// http://sourceforge.net/p/asio/mailman/message/24889328/
#define BOOST_ASIO_DISABLE_KQUEUE
// (This must come before all boost/asio includes, including indirect ones.)

#include <IOKit/serial/ioss.h>
#endif // __APPLE__

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/thread.hpp>

#include <iostream>
#include <map>
#include <string>
#include <vector>

class AsioClient;


//////////////////////////////////////////////////////////////////////////////
// Error codes
//
//#define ERR_UNKNOWN_LABEL 100
#define ERR_OPEN_FAILED 101
#define ERR_SETUP_FAILED 102
#define ERR_HANDSHAKE_SETUP_FAILED 103
#define ERR_TRANSMIT_FAILED 104
#define ERR_RECEIVE_FAILED 105
#define ERR_BUFFER_OVERRUN 106
#define ERR_TERM_TIMEOUT 107
#define ERR_PURGE_FAILED 108
#define ERR_PORT_CHANGE_FORBIDDEN 109
#define ERR_PORT_BLACKLISTED 110
#define ERR_PORT_NOTINITIALIZED 111


//////////////////////////////////////////////////////////////////////////////
// Implementation of the MMDevice and MMStateDevice interfaces
//

class SerialPort : public CSerialBase<SerialPort>  
{
public:
   SerialPort(const char* portName);
   ~SerialPort();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy() {return busy_;}

   int SetCommand(const char* command, const char* term);
   int GetAnswer(char* answer, unsigned bufLength, const char* term);
   int Write(const unsigned char* buf, unsigned long bufLen);
   int Read(unsigned char* buf, unsigned long bufLen, unsigned long& charsRead);
   MM::PortType GetPortType() const {return MM::SerialPort;}    
   int Purge();

   std::string Name(void) const;

   // This overrides a protected nonvirtual function and makes it public.
   void LogMessage(const char *const p, bool debugOnly = false)
   {
      if (this->IsCallbackRegistered())
         CSerialBase<SerialPort>::LogMessage(std::string(p), debugOnly);
      else
         std::cerr << p << std::endl;;
   }


   // action interface
   // ----------------
   int OnStopBits(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnParity(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnHandshaking(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnBaud(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnTimeout(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnDelayBetweenCharsMs(MM::PropertyBase* pProp, MM::ActionType eAct);

   int OnVerbose(MM::PropertyBase* pProp, MM::ActionType eAct);

   void AddReference() {refCount_++;}
   void RemoveReference() {refCount_--;}
   bool OKToDelete() {return refCount_ < 1;}


   bool Initialized(void) { return initialized_; }


private:
   std::string portName_;

   bool initialized_;
   bool busy_;

   // thread locking for the port 
   MMThreadLock portLock_;

   double answerTimeoutMs_;
   int refCount_;
   double transmitCharWaitMs_;
   std::map<std::string, int> baudList_;

   std::string stopBits_;
   std::string parity_;

   // create these guys in the order of declaration
   boost::asio::io_service* pService_;
   AsioClient* pPort_;
   // the worker thread
   boost::thread* pThread_;
   bool verbose_; // if false, turn off LogBinaryMessage even in Debug Log

#ifdef _WIN32
   int OpenWin32SerialPort(const std::string& portName, HANDLE& portHandle);
#endif
   void LogAsciiCommunication(const char* prefix, bool isInput, const std::string& content);
   void LogBinaryCommunication(const char* prefix, bool isInput, const unsigned char* content, std::size_t length);
};

class SerialManager
{
public:
   SerialManager() {}
   ~SerialManager();

   MM::Device* CreatePort(const char* portName);
   void DestroyPort(MM::Device* port);

private:
   std::vector<SerialPort*> ports_;
};

class SerialPortLister
{
   public:                                                                   
      // returns list of serial ports that can be opened
      static void ListPorts(std::vector<std::string> &availablePorts);
      static bool portAccessible(const char*  portName);                     
};
