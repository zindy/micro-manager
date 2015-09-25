/*
 * Copyright (c) 2007, Regents of the University of California
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
#ifdef _WINDOWS

#include <windows.h>

#else // Linux / Posix

#include "pevents.h"
#include <errno.h>

#endif


class TEventContainer
{
public:
   TEventContainer(const AT_WC * _name, andor::IInteger * _event)
   :  actualEvent_(_event),
      wcs_name_(_name),
      eventRegistered_(false),
      eventFired_(false),
#ifdef _WINDOWS
      h_event_(CreateEvent(NULL, false, false, NULL))
#else
      h_event_(neosmart::CreateEvent(false, false))
#endif
   {
   }
#ifdef _WINDOWS
  ~TEventContainer() {CloseHandle(h_event_); };
  void Set() { SetEvent(h_event_); }; 
  void Reset() { ResetEvent(h_event_); };
#else
  ~TEventContainer() {neosmart::DestroyEvent(h_event_); };
  void Set() { neosmart::SetEvent(h_event_); }; 
  void Reset() { neosmart::ResetEvent(h_event_); };
#endif 
   andor::IInteger * GetActualEvent() { return actualEvent_; };
   const AT_WC * GetFeatureName() { return wcs_name_.c_str(); };
   void SetRegistered() { eventRegistered_ = true; };
   bool GetRegistered() { return eventRegistered_; };
   void SetFired() { eventFired_ = true; };
   void ResetFired() { eventFired_ = false; };
   bool GetFired() { return eventFired_; };
   bool Wait(int _iTimeout_ms)
   {
#ifdef _WINDOWS
     DWORD retVal = WaitForSingleObject(h_event_, _iTimeout_ms);
     if(retVal!=WAIT_OBJECT_0)
#else // Posix / Linux
     int retVal = neosmart::WaitForEvent(h_event_, _iTimeout_ms);
     if(retVal!=ETIMEDOUT)
#endif
     {
       return false;
     }
     return true;
   }
   
private:
   andor::IInteger * actualEvent_;
   std::wstring wcs_name_;
   bool eventRegistered_;
   bool eventFired_;
#ifdef _WINDOWS
   HANDLE h_event_;
#else
  neosmart::neosmart_event_t h_event_;
#endif
};

