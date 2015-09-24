/*
 * MP285 Device Adapter Error Codes & Messages
 *
 * AUTHOR:
 * Lon Chu (lonchu@yahoo.com) created on June 2011
 *
 * Copyright (c)  Sutter Instrument
 * Copyright (c) 2011 Mission Bay Imaging, San Francisco
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

#ifndef _MP285ERROR_H_
#define _MP285ERROR_H_

#include <string>
#include <map>

class MPError
{
public:
    ~MPError();

    typedef int MPErr;
    enum _MPErr
    {
        MPERR_OK                        = 0,        // OK
        MPERR_SerialOverRun             = 1,        // Serial command buffer over run
        MPERR_SerialTimeout             = 2,        // Receiving serial command time out
        MPERR_SerialBufferFull          = 3,        // Serial command buffer full
        MPERR_SerialInpInvalid          = 4,        // Invalid serial command
        MPERR_SerialIntrupMove          = 5,        // Serial command interrupt motion
        MPERR_SerialZeroReturn          = 6,        // No response from serial port
        MPERR_SerialUnknownError        = 7,        // Unknown serial command
        MPERR_GENERIC                   = 8,        // Unspecified MP285 adapter errors
        MPERR_FileOpenFailed            = 9         // Fail to open file
    };

    static MPError* Instance();
    std::string GetErrorText(int nErrorCode) const;

private:
    MPError();
    static bool m_yInstanceFlag;
    static MPError* m_pMPError;
    std::map<int, std::string> m_sErrorText;        // error messages
};

#endif  // _MP285ERROR_H_
