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

#include "NotificationThread.h"

#include "PVCAMAdapter.h"

NotificationThread::NotificationThread(Universal* pCamera) :
    universal_(pCamera),
    requestStop_(false),
    overflowed_(false),
    maxSize_(0)
{
}

NotificationThread::~NotificationThread(void)
{
    requestStop();
}

void NotificationThread::Reset()
{
    overflowed_ = false;
}

void NotificationThread::SetQueueCapacity(int capacity)
{
    maxSize_ = capacity;
}

int  NotificationThread::Capacity() const
{
    return maxSize_;
}

bool NotificationThread::PushNotification( const NotificationEntry& entry )
{
    threadMutex_.lock();

    bool bRet = true;

    deque_.push_back( entry );
    if ( static_cast<int>(deque_.size()) > maxSize_ )
    {
        // We delete the first notification in the queue because it's the oldest
        // entry and there is a risk that the frame pointer will get soon overwritten
        // by PVCAM.
        deque_.pop_front();
        overflowed_ = true;
        bRet = false; // The queue has overflowed
    }
    else
    {
        frameReadyCondition_.notify_one();
    }

    threadMutex_.unlock();
    return bRet;
}

// From MMDeviceThreadBase
int NotificationThread::svc()
{
    while(!requestStop_)
    {
        NotificationEntry n;
        if ( waitNextNotification( n ) )
        {
            universal_->ProcessNotification( n );
        }
        else
        {   // Request stop flagged
            break;
        }
    }
    return 0;
}

//===================================================================== PRIVATE

void NotificationThread::requestStop()
{
    // Request the thread to stop
    threadMutex_.lock();
    requestStop_ = true;
    frameReadyCondition_.notify_one();
    threadMutex_.unlock();

    // Wait for the thread func to exit
    this->wait();
}

bool NotificationThread::waitNextNotification( NotificationEntry& e )
{
    boost::unique_lock<boost::mutex> threadLock(threadMutex_);
    if (deque_.size() == 0 )
    {
        frameReadyCondition_.wait(threadLock);
        if ( requestStop_ )
            return false;
    }

    // There is a new frame notification in the queue
    e = deque_.front();
    deque_.pop_front();

    return true;
}
