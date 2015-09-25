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

#include "PvCircularBuffer.h"

#include <cstddef> // ptrdiff_t

PvCircularBuffer::PvCircularBuffer() :
    pBuffer_(0), size_(0), frameSize_(0), frameCount_(0), latestFrameIdx_(-1), pFrameInfoArray_(0)
{
}

PvCircularBuffer::~PvCircularBuffer()
{
    delete[] pBuffer_;
    delete[] pFrameInfoArray_;
}

int PvCircularBuffer::Capacity() const
{
    return frameCount_;
}

size_t PvCircularBuffer::FrameSize() const
{
    return frameSize_;
}


void* PvCircularBuffer::Data() const
{
    return pBuffer_;
}

size_t PvCircularBuffer::Size() const
{
    return size_;
}

int PvCircularBuffer::LatestFrameIndex() const
{
    return latestFrameIdx_;
}

void* PvCircularBuffer::FrameData(int index) const
{
    // No bounds checking. Most of the functions in this class are called very
    // often - with every frame which could be thousands of times per second so
    // we try to avoid unnecessary branching.
    return pBuffer_ + index * frameSize_;
}

const PvFrameInfo& PvCircularBuffer::FrameInfo(int index) const
{
    return pFrameInfoArray_[index];
}

void PvCircularBuffer::Reset()
{
    // Must be -1, once the very first frame arrives the index becomes 0
    latestFrameIdx_ = -1;
}

void PvCircularBuffer::Resize(size_t frameSize, int count)
{
    if (frameSize != frameSize_ || count != frameCount_)
    {
        frameCount_ = count;
        frameSize_  = frameSize;
        size_       = count * static_cast<size_t>(frameSize);
        delete[] pBuffer_;
        pBuffer_ = new unsigned char[size_];

        delete[] pFrameInfoArray_;
        pFrameInfoArray_ = new PvFrameInfo[frameCount_];
    }

    Reset();
}

void PvCircularBuffer::ReportFrameArrived(const PvFrameInfo& frameNfo, void* pFrameData)
{
    // Calculate the index of the received frame in our circular buffer
    const int curFrameIdx = static_cast<const int>
        ((ptrdiff_t(pFrameData) - ptrdiff_t(pBuffer_)) / frameSize_);

    // Store a copy of the frameNfo to our array
    pFrameInfoArray_[curFrameIdx] = frameNfo;

    // In case we ever need that we can check missed frames in this place as well.
    // Currently we use FRAME_INFO.FrameNr to check for missed callbacks but here
    // we can use the current and last index to check whether they are in sequence.

    latestFrameIdx_ = curFrameIdx;
}
