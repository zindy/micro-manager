/*
 * Cross-platform wrapper class for using threads in MMDevices
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com 11/27/2007
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

#pragma once

#ifdef WIN32
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
#else
   #include <pthread.h>
#endif

/**
 * Base class for threads in MM devices
 */
class MMDeviceThreadBase
{
public:
   MMDeviceThreadBase() : thread_(0) {}
   virtual ~MMDeviceThreadBase() {}

   virtual int svc() = 0;

   virtual int activate()
   {
#ifdef _WIN32
      DWORD id;
      thread_ = CreateThread(NULL, 0, ThreadProc, this, 0, &id);
#else
      pthread_create(&thread_, NULL, ThreadProc, this);
#endif
      return 0; // TODO: return thread id
   }

   void wait()
   {
#ifdef _WIN32
      WaitForSingleObject(thread_, INFINITE);
#else
      pthread_join(thread_, NULL);
#endif
   }

private:
   // Forbid copying
   MMDeviceThreadBase(const MMDeviceThreadBase&);
   MMDeviceThreadBase& operator=(const MMDeviceThreadBase&);

#ifdef _WIN32
   HANDLE
#else
   pthread_t
#endif
   thread_;

   static
#ifdef _WIN32
   DWORD WINAPI
#else
   void*
#endif
   ThreadProc(void* param)
   {
      MMDeviceThreadBase* pThrObj = (MMDeviceThreadBase*) param;
#ifdef _WIN32
      return pThrObj->svc();
#else
      pThrObj->svc();
      return (void*) 0;
#endif
   }
};

/**
 * Critical section lock.
 */
class MMThreadLock
{
public:
   MMThreadLock()
   {
#ifdef _WIN32
      InitializeCriticalSection(&lock_);
#else
      pthread_mutexattr_t a;
      pthread_mutexattr_init(&a);
      pthread_mutexattr_settype(&a,
#ifdef __linux__
         // Not sure if _NP is needed any more
         PTHREAD_MUTEX_RECURSIVE_NP
#else
         PTHREAD_MUTEX_RECURSIVE
#endif
      );
      pthread_mutex_init(&lock_, &a);
      pthread_mutexattr_destroy(&a);
#endif
   }

   ~MMThreadLock()
   {
#ifdef _WIN32
      DeleteCriticalSection(&lock_);
#else
      pthread_mutex_destroy(&lock_);
#endif
   }

   void Lock()
   {
#ifdef _WIN32
      EnterCriticalSection(&lock_);
#else
      pthread_mutex_lock(&lock_);
#endif
   }

   void Unlock()
   {
#ifdef _WIN32
      LeaveCriticalSection(&lock_);
#else
      pthread_mutex_unlock(&lock_);
#endif
   }

private:
   // Forbid copying
   MMThreadLock(const MMThreadLock&);
   MMThreadLock& operator=(const MMThreadLock&);

#ifdef _WIN32
   CRITICAL_SECTION
#else
   pthread_mutex_t
#endif
   lock_;
};

class MMThreadGuard
{
public:
   MMThreadGuard(MMThreadLock& lock) : lock_(&lock)
   {
      lock_->Lock();
   }

   MMThreadGuard(MMThreadLock* lock) : lock_(lock)
   {
      if (lock != 0)
         lock_->Lock();
   }

   bool isLocked() {return lock_ == 0 ? false : true;}

   ~MMThreadGuard()
   {
      if (lock_ != 0)
         lock_->Unlock();
   }

private:
   // Forbid copying
   MMThreadGuard(const MMThreadGuard&);
   MMThreadGuard& operator=(const MMThreadGuard&);

   MMThreadLock* lock_;
};
