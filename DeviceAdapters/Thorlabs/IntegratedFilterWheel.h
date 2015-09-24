/*
 * Thorlabs device adapters: BBD102 Controller
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, 2011
 * http://nenad.amodaj.com
 *
 * Copyright (c) 2011 Thorlabs Inc
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

#ifndef _INTEGRATEDFILTERWHEEL_H_
#define _INTEGRATEDFILTERWHEEL_H_

#include <MMDevice.h>
#include <DeviceBase.h>

//////////////////////////////////////////////////////////////////////////////
// IntegratedFilterWheel class
// (device adapter)
//////////////////////////////////////////////////////////////////////////////
class IntegratedFilterWheel : public CStateDeviceBase<IntegratedFilterWheel>
{
public:
   IntegratedFilterWheel();
   ~IntegratedFilterWheel();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();
   unsigned long GetNumberOfPositions() const;

   // action interface
   // ----------------
   int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnCOMPort(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   int SetCommand(const unsigned char* command, unsigned length);
   int GetCommand(unsigned char* response, unsigned length, double timeoutMs);
   int DiscoverNumberOfPositions();
   int Home();
   int GoToPosition(long pos);
   int RetrieveCurrentPosition(long& pos);

   long numberOfPositions_;
   bool home_;
   bool initialized_;
   long position_;
   std::string port_;
   double answerTimeoutMs_;      // max wait for the device to answer

   static const unsigned int stepsTurn_ = 1774933; // steps in one wheel turn
};

#endif //_INTEGRATEDFILTERWHEEL_H_
