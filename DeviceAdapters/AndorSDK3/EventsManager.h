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
#include "atcore++.h"
#include <vector>

class TEventContainer;

class CEventsManager : public andor::IObserver
{
public:
   typedef enum 
   {
      EV_BUFFER_OVERFLOW_EVENT,
      EV_EXPOSURE_END_EVENT,
      EV_TOTAL_SIZE  //Do not remove; insert new members above this.
   } TSDK3EVENTS;

public:
   CEventsManager(andor::IDevice* cameraDevice);
   ~CEventsManager(void);

   void Update(andor::ISubject* Subject);
   bool Initialise(char * _errorMsg);

   void ResetEvent(CEventsManager::TSDK3EVENTS _event);
   bool IsEventRegistered(CEventsManager::TSDK3EVENTS _event);
   bool HasEventFired(CEventsManager::TSDK3EVENTS _event);
   bool WaitForEvent(CEventsManager::TSDK3EVENTS _event, int _timeout_ms);

private:
   TSDK3EVENTS getEventTypeFromSubject(andor::ISubject * subjectType);
   void eventsEnable(const AT_WC *const _event, bool _enable);

private:
   andor::IDevice * camDevice_;
   
   std::vector<TEventContainer *> v_events;
   typedef std::vector<TEventContainer *>::iterator TEvIter;
};

