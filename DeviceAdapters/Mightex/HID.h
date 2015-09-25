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

#ifndef __HID_H__
#define __HID_H__

#include <windows.h>
#include <stdio.h> 
#include <conio.h>
#include <stdlib.h>
#include <assert.h>

extern "C" 
{
#include <setupapi.h>
#include <hidsdi.h>
}

#define DEV_NUM  36 //16

extern void  HID_Init();
extern void  HID_UnInit();
extern int   HID_FindDevices( USAGE, USAGE );
extern BOOL  HID_GetName(int num, char *buf, int sz);
extern BOOL  HID_GetSerialNumber( int num, char *buf, int sz);
extern BOOL  HID_Open(int num);
extern void  HID_Close(int num);
extern BOOL  HID_Read (int num, unsigned char *buf, unsigned int sz, unsigned int *cnt);
extern BOOL  HID_Write(int num, unsigned char *buf, unsigned int sz, unsigned int *cnt);
extern BOOL  HID_SetFeature(int num, unsigned char *buf, unsigned int sz );
extern BOOL  HID_GetFeature(int num, unsigned char *buf, unsigned int sz );

#endif /* __HID_H__ */
