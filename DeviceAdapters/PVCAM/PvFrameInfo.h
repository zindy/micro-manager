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

#ifndef _PVFRAMEINFO_H_
#define _PVFRAMEINFO_H_

#include "PVCAMIncludes.h"
/**
* A local definition for the FRAME_INFO. Used to copy the
* essential FRAME_INFO metedata together with additional 
* frame metadata.
*/
class PvFrameInfo
{
public:
    PvFrameInfo();
    ~PvFrameInfo();

    /// FRAME_INFO related metadata
    void SetPvHCam(short int);
    short int PvHCam() const;
    void SetPvFrameNr(int);
    int PvFrameNr() const;
    void SetPvTimeStamp(long long);
    long long PvTimeStamp() const;
    void SetPvReadoutTime(int);
    int PvReadoutTime() const;
    void SetPvTimeStampBOF(long long);
    long long PvTimeStampBOF() const;
    ///

    /// Other metadata added by the adapter
    void SetTimestampMsec(double msec);
    double TimeStampMsec() const;

    void SetRecovered(bool recovered);
    bool IsRecovered() const;

private:

    // FRAME_INFO Metadata
    short int pvHCam_;         // int16  FRAME_INFO.hCam
    int       pvFrameNr_;      // int32  FRAME_INFO.FrameNr
    long long pvTimeStamp_;    // long64 FRAME_INFO.TImeStamp (EOF)
    int       pvReadoutTime_;  // int32  FRAME_INFO.ReadoutTime
    long long pvTimeStampBOF_; // long64 FRAME_INFO.TimeStampBOF

    // Additional Metadata
    double    timestampMsec_; // MM Timestamp
    bool      isRecovered_;   // Recovered from missed callback
};

#endif // _PVFRAMEINFO_H_
