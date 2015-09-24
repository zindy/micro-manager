/*
 * CrEST XLight adapter
 *
 * AUTHOR:
 * E. Chiarappa echiarappa@libero.it, 01/20/2014
 * Based on CARVII adapter by  G. Esteban Fernandez.
 *
 * Copyright (c) 2014 Crestoptics s.r.l.
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

#ifndef _XLIGHTHUB_H_
#define _XLIGHTHUB_H_

#include <string>
#include <deque>
#include <map>
#include "../../MMDevice/MMDevice.h"

/////////////////////////////////////////////////////////////////////////
// Error codes
//
#define ERR_NOT_CONNECTED           10002
#define ERR_COMMAND_CANNOT_EXECUTE  10003

enum CommandMode {
    Sync = 0,
    Async
};

class XLIGHTHub {
public:
    XLIGHTHub();
    ~XLIGHTHub();

    void SetPort(const char* port) {
        port_ = port;
		 
    }
    bool IsBusy();

    int SetDichroicPosition(MM::Device& device, MM::Core& core, int pos, int delay);
    int GetDichroicPositionEcho(MM::Device& device, MM::Core& core, int pos);

	int SetEmissionWheelPosition(MM::Device& device, MM::Core& core, int pos);
    int GetEmissionWheelPosition(MM::Device& device, MM::Core& core, int pos);
 
    int SetSpinMotorState(MM::Device& device, MM::Core& core, int state);
    int GetSpinMotorState(MM::Device& device, MM::Core& core, int state);

    int SetTouchScreenState(MM::Device& device, MM::Core& core, int state);
    int GetTouchScreenState(MM::Device& device, MM::Core& core, int state);

    int SetDiskSliderPosition(MM::Device& device, MM::Core& core, int pos, int delay);
 	int GetXlightCommandEcho(MM::Device& device, MM::Core& core);
 
    int StartSpinningDisk(MM::Device& device, MM::Core& core);

    int LockoutTouchscreen(MM::Device& device, MM::Core& core);
    int ActivateTouchscreen(MM::Device& device, MM::Core& core);

private:
    int ExecuteCommand(MM::Device& device, MM::Core& core, const char* command);
    //int GetAcknowledgment(MM::Device& device, MM::Core& core);
    int ParseResponse(const char* cmdId, std::string& value);
    void FetchSerialData(MM::Device& device, MM::Core& core);

    static const int RCV_BUF_LENGTH = 1024;
    char rcvBuf_[RCV_BUF_LENGTH];
    char asynchRcvBuf_[RCV_BUF_LENGTH];
    void ClearRcvBuf();
    void ClearAllRcvBuf(MM::Device& device, MM::Core& core);

    std::string port_;
    std::vector<char> answerBuf_;
    std::multimap<std::string, long> waitingCommands_;
    std::string commandMode_;
    int deviceWaitMs_;
};

#endif // _XLIGHTHUB_H_
