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

#define _CRT_SECURE_NO_DEPRECATE

#include "XLIGHTHub.h"
#include "assert.h"
#include <memory.h>
#include <sstream>
#include <iostream>

#include "../../MMDevice/DeviceUtils.h"

using namespace std;

XLIGHTHub::XLIGHTHub() {
    // CARVII does not confirm successful device movement; wait time
    // in millisec to ensure movement has ended
    deviceWaitMs_ = 20;

    ClearRcvBuf();
}

XLIGHTHub::~XLIGHTHub() {
}


///////////////////////////////////////////////////////////////////////////////
// Device commands
///////////////////////////////////////////////////////////////////////////////

 
int XLIGHTHub::SetDichroicPosition(MM::Device& device, MM::Core& core, int pos, int delay) {
    int posCommand = pos + 1; //MM positions start at 0, CARVII pos start at 1
    ostringstream os;
    os << "C" << posCommand;
    bool succeeded = false;
    //int counter = 0;
    int ret = DEVICE_OK;
	deviceWaitMs_ = delay;

 
	 while (!succeeded ) {
        ret = ExecuteCommand(device, core, os.str().c_str());

        //to ensure wheel finishes movement, query the serial line waiting for echoed 
		// command executed which means XLight accomplished the movement
        
		//time_t now = time(0);
		//char* dt = ctime(&now);
		GetXlightCommandEcho(device,core);
		
       if (ret != DEVICE_OK)
            continue;
        else {
		 
			CDeviceUtils::SleepMs(deviceWaitMs_);
			succeeded = true;
		}
    }
    if (!succeeded)
        return ret;

    return DEVICE_OK;
}

int XLIGHTHub::GetDichroicPositionEcho(MM::Device& device, MM::Core& core, int pos) {
    ClearAllRcvBuf(device, core);
    int ret = ExecuteCommand(device, core, "rC");
    ret = core.GetSerialAnswer(&device, port_.c_str(), RCV_BUF_LENGTH, rcvBuf_, "\r");
    if (ret != DEVICE_OK)
        return ret;

     pos = rcvBuf_[2];

    return DEVICE_OK;
}

int XLIGHTHub::SetEmissionWheelPosition(MM::Device& device, MM::Core& core, int pos) {
    int posCommand = pos + 1; //MM positions start at 0, CARVII pos start at 1
    ostringstream os;
    os << "B" << posCommand;
    bool succeeded = false;
    //int counter = 0;  
    int ret = DEVICE_OK;

    // try up to 10 times
    while (!succeeded) {
        ret = ExecuteCommand(device, core, os.str().c_str());

        //to ensure wheel finishes movement, CARVII does not signal movement succeeded
       GetXlightCommandEcho(device,core);
		

        if (ret != DEVICE_OK)
            continue;
        else{
			CDeviceUtils::SleepMs(deviceWaitMs_);
			succeeded = true;}
    }
    if (!succeeded)
        return ret;

    return DEVICE_OK;
}

int XLIGHTHub::GetEmissionWheelPosition(MM::Device& device, MM::Core& core, int pos) {
    ClearAllRcvBuf(device, core);
    int ret = ExecuteCommand(device, core, "rE");
    ret = core.GetSerialAnswer(&device, port_.c_str(), RCV_BUF_LENGTH, rcvBuf_, "\r");
    if (ret != DEVICE_OK)
        return ret;

    // CARVII echoes command, dichroic position is in bit 3
    pos = rcvBuf_[2];

    return DEVICE_OK;
}
 
int XLIGHTHub::SetSpinMotorState(MM::Device& device, MM::Core& core, int state) {
    ostringstream os;
    os << "N" << state;
    bool succeeded = false;
    int counter = 0;
    int ret = DEVICE_OK;
	deviceWaitMs_ = 15;
    // try up to 10 times
    while (!succeeded && counter < 10) {
        ret = ExecuteCommand(device, core, os.str().c_str());

        //to ensure wheel finishes movement, CARVII does not signal movement succeeded
        CDeviceUtils::SleepMs(deviceWaitMs_);
		 
        if (ret != DEVICE_OK)
            counter++;
        else
            succeeded = true;
    }
    if (!succeeded)
        return ret;

    return DEVICE_OK;
}

int XLIGHTHub::GetSpinMotorState(MM::Device& device, MM::Core& core, int state) {
    ClearAllRcvBuf(device, core);
    int ret = ExecuteCommand(device, core, "rN");
    ret = core.GetSerialAnswer(&device, port_.c_str(), RCV_BUF_LENGTH, rcvBuf_, "\r");
    if (ret != DEVICE_OK)
        return ret;

    // CARVII echoes command, motor state is in bit 3
    state = rcvBuf_[2];

    return DEVICE_OK;
}

int XLIGHTHub::SetTouchScreenState(MM::Device& device, MM::Core& core, int state) {
    ostringstream os;
    os << "M" << state;
    bool succeeded = false;
    int counter = 0;

    int ret = DEVICE_OK;

    // try up to 10 times, wait 50 ms in between tries
    while (!succeeded && counter < 10) {
        ret = ExecuteCommand(device, core, os.str().c_str());
        if (ret != DEVICE_OK) {
            CDeviceUtils::SleepMs(50);
            counter++;
        } else
            succeeded = true;
    }
    if (!succeeded)
        return ret;
    else return DEVICE_OK;
}

int XLIGHTHub::GetTouchScreenState(MM::Device& device, MM::Core& core, int state) {
    ClearAllRcvBuf(device, core);
    int ret = ExecuteCommand(device, core, "rM");
    ret = core.GetSerialAnswer(&device, port_.c_str(), RCV_BUF_LENGTH, rcvBuf_, "\r");
    if (ret != DEVICE_OK)
        return ret;

    // CARVII echoes command, motor state is in bit 3
    state = rcvBuf_[2];

    return DEVICE_OK;
}

int XLIGHTHub::SetDiskSliderPosition(MM::Device& device, MM::Core& core, int pos, int delay) {
	
    deviceWaitMs_ = delay;
	ostringstream os;
    os << "D" << pos;
    bool succeeded = false;
 
    int ret = DEVICE_OK;
      // try up to 10 times
 	 while (!succeeded ) {
        ret = ExecuteCommand(device, core, os.str().c_str());

        //to ensure wheel finishes movement, query the serial line waiting for echoed 
		// command executed which means XLight accomplished the movement
        
		//time_t now = time(0);
		//char* dt = ctime(&now);
		GetXlightCommandEcho(device,core);
		
       if (ret != DEVICE_OK)
            continue;
        else {
		 
			CDeviceUtils::SleepMs(deviceWaitMs_);
			succeeded = true;
		}
    }
    if (!succeeded)
        return ret;
	 
	return DEVICE_OK;
}

int XLIGHTHub::GetXlightCommandEcho(MM::Device& device, MM::Core& core) {
     ClearAllRcvBuf(device, core);
	 int ret = 0;
   

    // XLight echoes command, motor state starting from bit 0
    
	 
	while (rcvBuf_[0] == 0)
	{
		ret = core.GetSerialAnswer(&device, port_.c_str(), RCV_BUF_LENGTH, rcvBuf_, "\r");
	 
	}
	 if (rcvBuf_[0] == 68 || rcvBuf_[0] == 67){
		 //time_t after = time(0);
		//char* dt = ctime(&after);
		 return DEVICE_OK;}
	 else 
		 return ret;
}

 
///////////////////////////////////////////////////////////////////////////////
// HUB generic methods
///////////////////////////////////////////////////////////////////////////////

/**
 * Clears the serial receive buffer.
 */
void XLIGHTHub::ClearAllRcvBuf(MM::Device& device, MM::Core& core) {
    // Read whatever has been received so far:
    unsigned long read;
    core.ReadFromSerial(&device, port_.c_str(), (unsigned char*) rcvBuf_, RCV_BUF_LENGTH, read);
    // Delete it all:
    memset(rcvBuf_, 0, RCV_BUF_LENGTH);
}

void XLIGHTHub::ClearRcvBuf() {
    memset(rcvBuf_, 0, RCV_BUF_LENGTH);
}

/**
 * Buys flag. True if the command processing is in progress.
 */
bool XLIGHTHub::IsBusy() {
    return waitingCommands_.size() != 0;
}

int XLIGHTHub::StartSpinningDisk(MM::Device& device, MM::Core& core) {
    //D1 slides disk into light path, N1 starts motor spinning
    return ExecuteCommand(device, core, "D1N1");
}

int XLIGHTHub::LockoutTouchscreen(MM::Device& device, MM::Core& core) {
    //Freezes touch screen to prevent catastrophic crash by simultaneous serial
    //and screen control
    return ExecuteCommand(device, core, "M1");
}

int XLIGHTHub::ActivateTouchscreen(MM::Device& device, MM::Core& core) {
    //Activates touchscreen control
    return ExecuteCommand(device, core, "M0");
}

/**
 * Sends serial command to the MMCore virtual serial port.
 */
int XLIGHTHub::ExecuteCommand(MM::Device& device, MM::Core& core, const char* command) {
    // empty the Rx serial buffer before sending command
    //FetchSerialData(device, core);

    ClearAllRcvBuf(device, core);
    // send command
    return core.SetSerialCommand(&device, port_.c_str(), command, "\r");
 	
	
}
