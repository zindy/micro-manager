/*
 * HID interface device adapter - UNIX version
 *
 * AUTHOR:
 * Nico Stuurman, May. 2013
 *
 * Copyright (c) 2007 Regents of the University of California
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

/*
 * NOTES:
 *            Uses hidapi 0.7
 *                 http://www.signal11.us/oss/hidapi 
 */

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include <string>
#include <stdexcept>
#include <map>

#ifdef WIN32
typedef int	u_int16_t;
#endif

//////////////////////////////////////////////////////////////////////////////
// Error codes
//
//#define ERR_UNKNOWN_LABEL 100
#define ERR_OPEN_FAILED 101
#define ERR_SETUP_FAILED 102
#define ERR_TRANSMIT_FAILED 104
#define ERR_RECEIVE_FAILED 105
#define ERR_BUFFER_OVERRUN 106
#define ERR_TERM_TIMEOUT 107
#define ERR_PORT_CHANGE_FORBIDDEN 109
#define ERR_PORT_DOES_NOT_EXIST 110
#define ERR_PORT_ALREADY_OPEN 111
#define ERR_PORT_DISAPPEARED 112
#define ERR_INTERNAL_ERROR 113
#define ERR_CLAIM_INTERFACE 114
#define ERR_CLOSING_DEVICE 115
#define ERR_WRITE_FAILED 116

class CHID;
class HIDDevice;

struct HIDDeviceInfo
{
   std::string name;
   unsigned short idVendor;
   unsigned short idProduct;

};

class HIDDeviceLister
{
   public:
      HIDDeviceLister();
      ~HIDDeviceLister();

      // returns the current list of ports
      void ListHIDDevices(std::vector<std::string> &availableDevices);
      // returns a cached list of devices
      void ListCachedHIDDevices(std::vector<std::string> &availableDevices);

   private:
      MM::MMTime GetCurrentMMTime();
      std::vector<std::string> storedAvailableHIDDevices_;
      void FindHIDDevices(std::vector<std::string> &availableDevices);
};

//////////////////////////////////////////////////////////////////////////////
// Implementation of the MMDevice and MMStateDevice interfaces
//

// Exception class for MDHIDDevice
class NotOpen : public std::logic_error
{   
public:
  NotOpen(const std::string& whatArg) :
      logic_error(whatArg) { }
}; 

// Exception class for MDHIDDevice
class ReadTimeout : public std::runtime_error
{
public:
  ReadTimeout() : runtime_error( "Read timeout" ) { }
};
class MDHIDDevice : public CSerialBase<MDHIDDevice>  
{
public:
   MDHIDDevice(std::string deviceName);
   ~MDHIDDevice();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy() {return busy_;}

   int SetCommand(const char* command, const char* term);
   int GetAnswer(char* answer, unsigned bufLength, const char* term);
   int Write(unsigned const char* buf, unsigned long bufLen);
   int Read(unsigned char* buf, unsigned long bufLen, unsigned long& charsRead);
   int Purge();

   MM::PortType GetPortType() const {return MM::HIDPort;}

   // action interface
   // ----------------
   int OnTimeout(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnDevice(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnVerbose(MM::PropertyBase* pProp, MM::ActionType eAct);

   int Open(const char* portName);
   int Close();
   void AddReference() {refCount_++;}
   void RemoveReference() {refCount_--;}
   bool OKToDelete() {return refCount_ < 1;}

private:
   int TakeOverDevice(int interfaceIndex);
   int WriteByte(const unsigned char dataByte);
   std::string ReadLine(const unsigned int msTimeOut, const char* lineTerminator) throw (NotOpen, ReadTimeout, std::runtime_error);

   std::string deviceName_;
   hid_device *handle_;
   int refCount_;
   bool busy_;
   bool open_;
   bool initialized_;
   double portTimeoutMs_;
   std::vector<std::string> availableDevices_;
   HIDDeviceLister* deviceLister;
   double answerTimeoutMs_;
   int deviceInputEndPoint_;
   int deviceOutputEndPoint_;
   bool deviceUsesBulkEndPoint_;
   int maxPacketSize_;

   int HandleError(int errorCode);

   char* overflowBuffer_;
   unsigned overflowBufferOffset_;
   unsigned overflowBufferLength_;

   bool verbose_; // whether or not to do verbose logging
};

class HIDManager
{
public:
   HIDManager();
   ~HIDManager();

   MM::Device* CreatePort(const char* portName);
   void DestroyPort(MM::Device* port);

private:
   std::vector<MDHIDDevice*> devices_;
};

#define _HID_h_
