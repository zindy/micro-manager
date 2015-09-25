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

#include "PvFrameInfo.h"

PvFrameInfo::PvFrameInfo() :
    pvHCam_(0),
    pvFrameNr_(0),
    pvTimeStamp_(0),
    pvReadoutTime_(0),
    pvTimeStampBOF_(0),
    timestampMsec_(0),
    isRecovered_(false)
{
}

PvFrameInfo::~PvFrameInfo()
{
}

void PvFrameInfo::SetPvHCam(short int hCam) { pvHCam_ = hCam; }
short int PvFrameInfo::PvHCam() const { return pvHCam_; }

void PvFrameInfo::SetPvFrameNr(int pvFrameNr) { pvFrameNr_ = pvFrameNr; }
int PvFrameInfo::PvFrameNr() const { return pvFrameNr_; }

void PvFrameInfo::SetPvTimeStamp(long long pvTimeStamp) { pvTimeStamp_ = pvTimeStamp; }
long long PvFrameInfo::PvTimeStamp() const { return pvTimeStamp_; }

void PvFrameInfo::SetPvReadoutTime(int pvReadoutTime) { pvReadoutTime_ = pvReadoutTime; }
int PvFrameInfo::PvReadoutTime() const { return pvReadoutTime_; }

void PvFrameInfo::SetPvTimeStampBOF(long long pvTimeStampBOF) { pvTimeStampBOF_ = pvTimeStampBOF; }
long long PvFrameInfo::PvTimeStampBOF() const { return pvTimeStampBOF_; }

void PvFrameInfo::SetTimestampMsec(double msec) { timestampMsec_ = msec; }
double PvFrameInfo::TimeStampMsec() const { return timestampMsec_; }

void PvFrameInfo::SetRecovered(bool recovered) { isRecovered_ = recovered; }
bool PvFrameInfo::IsRecovered() const { return isRecovered_; }
