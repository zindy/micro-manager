/*
 * nPoint C400 Controller
 *
 * AUTHOR:
 * Lon Chu (lonchu@yahoo.com) created on August 2011
 * Lon Chu (lonchu@yahoo.com) modified on
 *
 * Copyright (c) 2011 nPoint
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

#pragma once

#include <string>
#include <map>

// Global function to reset the serial port
int ClearPort(MM::Device& device, MM::Core& core, const char* port);

class nPC400
{
public:
    ~nPC400();   // Destructor

    typedef int C400;
    enum _C400
    {
        C400_ChannelBoardName        = 0,            // C400 channel board name
        C400_CH1DeviceName           = 1,            // Channel 1 device name
        C400_CH2DeviceName           = 2,            // Channel 2 device name
        C400_CH3DeviceName           = 3,            // Channel 3 device name
        C400_CH4DeviceName           = 4,            // Channel 4 device name
        C400_CH5DeviceName           = 5,            // Channel 5 device name
        C400_CH6DeviceName           = 6,            // Channel 6 device name
        C400_SoftwareVersion         = 7,            // C400 software version string
        C400_LogFilename             = 8,            // C400 log filename
        C400_ChannelBoardNameLabel   = 9,            // C400 channels board connected variable label
        C400_ChannelBoardDescLabel   = 10,           // C400 channels board connected variable label
        C400_ChannelBoardConnLabel   = 11,           // C400 channels board connected variable label
        C400_SoftwareVersionLabel    = 12,           // C400 software version vabel
        C400_NumberOfAxesLabel       = 13,           // C400 number of axes controlled label
        C400_DebugLogFlagLabel       = 14,           // C400 debug log flag label
        C400_ChannelDeviceNameLabel  = 15,           // Channel device name label
        C400_ChannelDeviceDescLabel  = 16,           // Channel device description label
        C400_ChannelDeviceConnLabel  = 17,           // Channel device connection label
        C400_ChannelRangeRadLabel    = 18,           // Channel device range radius label
        C400_ChannelRangeUnitLabel   = 19,           // Channel device range unit label
        C400_ChannelRangeLabel       = 20,           // Channel device range label
        C400_ChannelPositionSetLabel = 21,           // Channel device position set label
        C400_ChannelPositionGetLabel = 22,           // Channel device position get label
        C400_ChannelSrvoStatLabel    = 23,           // Channel device servo state label
        C400_ChannelPropGainLabel    = 24,           // Channel device propotional gain label
        C400_ChannelIntgGainLabel    = 25,           // Channel deviceIntegral gain label
        C400_ChannelDervGainLabel    = 26            // Channel device position get name
    };

    enum
    {
        C400_CH1 = 0,                               // channel 1
        C400_CH2 = 1,                               // channel 2
        C400_CH3 = 2,                               // channel 3
        C400_CH4 = 3,                               // channel 4
        C400_CH5 = 4,                               // channel 5
        C400_CH6 = 5,                               // channel 6
        C400_TxTerm = 0x55,                         // unique termination from MM to C400 communication - host mode
        C400_RxTerm = 0x55                          // unique termination from C400 to MM communication
    };

    static nPC400* Instance();                                                                                  // only interface for singleton
    std::string GetC400Str(int nC400StrCode) const;                                                             // access prdefined strings
    static int ByteCopy(unsigned char* bDst, const unsigned char* bSrc, int nLength);                           // copy byte buffer for iLength
    static void Byte2Hex(const unsigned char bByte, char* sHex);                                                // convert byte number to hex
    static void SetChannelsAvailable(int nChannelsAvailable) { m_nChannelsAvailable = nChannelsAvailable; }     // set channels availability
    static int GetChannelsAvailability() { return m_nChannelsAvailable; }                                       // get channels availability
    static void SetNumberOfAxes(int nNumberOfAxes) { m_nNumberOfAxes = nNumberOfAxes; }                         // set number of axes controlled by C400
    static int  GetNumberOfAxes() { return m_nNumberOfAxes; }                                                   // get numebr of axes controlled by C400
    static int GetDebugLogFlag() { return m_nDebugLogFlag; }                                                    // get debug log flag
    static void SetDebugLogFlag(int nDebugLogFlag) { m_nDebugLogFlag = nDebugLogFlag; }                         // set debug log flag
    static std::string& GetSerialPort() { return m_sPort; }                                                     // get serial port symbol
    static void SetSerialPort(std::string sPort) { m_sPort = sPort; }                                           // set serial port symbol

protected:
    nPC400();    // Constructor

private:
    static bool                 m_yInstanceFlag;            // singleton flag
    static nPC400*              m_pC400;                    // singleton copy
    static int                  m_nChannelsAvailable;       // C400 availability
    static int                  m_nNumberOfAxes;            // number of axes (channels)
    static int                  m_nDebugLogFlag;            // debug log flag
    static std::string          m_sPort;                    // serial port symbols
    std::map<int, std::string>  m_sC400Str;                 // constant strings
};
