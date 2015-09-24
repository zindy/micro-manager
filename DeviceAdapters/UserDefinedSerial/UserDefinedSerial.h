/*
 * Control devices using user-specified serial commands
 *
 * AUTHOR:
 * Mark Tsuchida
 *
 * Copyright (c) 2014 Regents of the University of California
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

#include "DeviceBase.h"
#include "DeviceUtils.h"

#include <boost/scoped_array.hpp>

#include <memory>
#include <string>
#include <vector>

class ResponseDetector;


/**
 * \brief Common base class template for concrete device classes.
 *
 * Uses the template template argument TBasicDevice so that it can inherit from
 * either CShutterBase CStateDeviceBase (or other).
 *
 * \tparam TBasicDevice the device base class
 * \tparam UConcreteDevice the concrete device class
 */
template <template <class> class TBasicDevice, class UConcreteDevice>
class UserDefSerialBase : public TBasicDevice<UConcreteDevice>
{
protected:
   typedef UserDefSerialBase Self;
   typedef TBasicDevice<UConcreteDevice> Super;
   typedef UConcreteDevice DeviceType;
   // An always-safe cast
   DeviceType* This() { return static_cast<DeviceType*>(this); }

public:
   UserDefSerialBase();
   virtual ~UserDefSerialBase() {}

   // MM::Device methods
   // Derived classes must call these base class versions if overriding
   virtual int Initialize();
   virtual int Shutdown();
   virtual bool Busy(); // Should not be overridden

private:
   // Register error messages for all derived devices (we use common codes
   // within this device adapter)
   void RegisterErrorMessages();
   // No point in making virtual (called from ctor)
   void CreatePreInitProperties();

protected:
   // Derived classes must call this base class version if overriding
   virtual int CreatePostInitProperties();
   virtual void StartBusy(); // Should not be overridden

protected:
   // Property action handlers
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnCommandSendMode(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnResponseDetectionMethod(MM::PropertyBase* pProp, MM::ActionType eAct);

protected:
   // Create a string property for a command or response string, mapped to
   // an instance variable
   int CreateByteStringProperty(const char* name,
         std::vector<char>& varRef, bool preInit = false);
   int CreateByteStringProperty(const std::string& name,
         std::vector<char>& varRef, bool preInit = false)
   { return CreateByteStringProperty(name.c_str(), varRef, preInit); }

   int SendRecv(const std::vector<char>& command,
         const std::vector<char>& expectedResponse);

   // Send a command and match response against several alternatives
   int SendQueryRecvAlternative(const std::vector<char>& command,
         const std::vector< std::vector<char> >& responseAlts,
         size_t& responseAltIndex);

private:
   int Send(const std::vector<char>& command);

private:
   std::string port_;
   bool initialized_;

   MM::MMTime lastActionTime_;

   bool binaryMode_;
   std::string asciiTerminator_;
   std::auto_ptr<ResponseDetector> responseDetector_;

   std::vector<char> initializeCommand_;
   std::vector<char> initializeResponse_;
   std::vector<char> shutdownCommand_;
   std::vector<char> shutdownResponse_;
};


class UserDefSerialGenericDevice :
   public UserDefSerialBase<CGenericBase, UserDefSerialGenericDevice>
{
protected:
   typedef UserDefSerialGenericDevice Self;
   typedef UserDefSerialBase< ::CGenericBase, Self > Super;

public:
   UserDefSerialGenericDevice();

   virtual void GetName(char* name) const;
};


class UserDefSerialShutter :
   public UserDefSerialBase<CShutterBase, UserDefSerialShutter>
{
protected:
   typedef UserDefSerialShutter Self;
   typedef UserDefSerialBase< ::CShutterBase, Self > Super;

public:
   UserDefSerialShutter();

   virtual void GetName(char* name) const;

   // MM::Shutter methods
   virtual int SetOpen(bool open);
   virtual int GetOpen(bool& open);
   virtual int Fire(double) { return DEVICE_UNSUPPORTED_COMMAND; }

   // Overrides
   virtual int Initialize();
   virtual int Shutdown();

private:
   void CreatePreInitProperties();

protected:
   // Override
   virtual int CreatePostInitProperties();

private:
   // Property action handlers
   int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   bool lastSetOpen_;

   std::vector<char> openCommand_;
   std::vector<char> openResponse_;
   std::vector<char> closeCommand_;
   std::vector<char> closeResponse_;
   std::vector<char> queryCommand_;
   std::vector<char> queryOpenResponse_;
   std::vector<char> queryCloseResponse_;
};


class UserDefSerialStateDevice :
   public UserDefSerialBase<CStateDeviceBase, UserDefSerialStateDevice>
{
protected:
   typedef UserDefSerialStateDevice Self;
   typedef UserDefSerialBase< ::CStateDeviceBase, Self > Super;

public:
   UserDefSerialStateDevice();

   virtual void GetName(char* name) const;

   // MM::State methods (Get/SetState() is implemented in CStateDeviceBase
   // based on the State property)
   virtual unsigned long GetNumberOfPositions() const;

   // Override
   virtual int Initialize();

private:
   void CreatePreInitProperties();

protected:
   // Override
   virtual int CreatePostInitProperties();

private:
   // Property action handlers
   int OnNumberOfPositions(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   size_t numPositions_;
   size_t currentPosition_;

   // Because we use references to the command and response strings to bind
   // them to the properties (see
   // UserDefSerialBase::CreateByteStringProperty()), the std::vector<char>
   // must be at a fixed location (between initialization and shutdown). Thus,
   // we use boost::scoped_array which, unlike std::vector, never reallocates
   // its storage.
   boost::scoped_array< std::vector<char> > positionCommands_;
   boost::scoped_array< std::vector<char> > positionResponses_;
   std::vector<char> queryCommand_;
   boost::scoped_array< std::vector<char> > queryResponses_;
};
