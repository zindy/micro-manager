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


#define  MAX_PROFILE_ITEM     128

#define	DISABLE_MODE	0
#define NORMAL_MODE	1
#define STROBE_MODE     2
#define TRIGGER_MODE    3

#define MODULE_AA	0
#define MODULE_AV	1
#define MODULE_SA	2
#define MODULE_SV	3
#define MODULE_MA	4
#define MODULE_CA	5
#define MODULE_HA	6
#define MODULE_HV	7
#define MODULE_FA	8
#define MODULE_FV	9
#define MODULE_XA	10
#define MODULE_XV	11
#define MODULE_QA	12

#pragma pack(1)
typedef struct {
    int Normal_CurrentMax;
    int Normal_CurrentSet;

    int Strobe_CurrentMax;
    int Strobe_RepeatCnt;
    int Strobe_Profile[MAX_PROFILE_ITEM][2];

    int Trigger_CurrentMax;
    int Trigger_Polarity;
    int Trigger_Profile[MAX_PROFILE_ITEM][2];
} TLedChannelData;
#pragma pack()

// Export functions:
int MTUSB_LEDDriverInitDevices( void );
int MTUSB_LEDDriverOpenDevice( int DeviceIndex );
int MTUSB_LEDDriverCloseDevice( int DevHandle );
int MTUSB_LEDDriverSerialNumber( int DevHandle, char *SerNumber, int Size );
int MTUSB_LEDDriverDeviceChannels( int DevHandle );
int MTUSB_LEDDriverDeviceModuleType( int DevHandle);
int MTUSB_LEDDriverSetMode( int DevHandle, int Channel, int Mode );
int MTUSB_LEDDriverSetNormalPara( int DevHandle, int Channel, TLedChannelData *LedChannelDataPtr );
int MTUSB_LEDDriverSetNormalCurrent( int DevHandle, int Channel, int Current );
int MTUSB_LEDDriverSetStrobePara( int DevHandle, int Channel, TLedChannelData *LedChannelDataPtr );
int MTUSB_LEDDriverSetTriggerPara( int DevHandle, int Channel, TLedChannelData *LedChannelDataPtr );
int MTUSB_LEDDriverResetDevice( int DevHandle );
int MTUSB_LEDDriverStorePara( int DevHandle );
int MTUSB_LEDDriverRestoreDefault( int DevHandle );
int MTUSB_LEDDriverGetLoadVoltage( int DevHandle, int Channel );
int MTUSB_LEDDriverGetCurrentPara( int DevHandle, int Channel,TLedChannelData *LedChannelDataPtr,int *Mode );
int MTUSB_LEDDriverSendCommand( int DevHandle, char *CommandString );
