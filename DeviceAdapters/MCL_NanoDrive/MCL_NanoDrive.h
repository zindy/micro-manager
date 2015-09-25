/*
 * Copyright (c) 2008, Mad City Labs Inc.
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

#ifndef _MCL_NANODRIVE_H_
#define _MCL_NANODRIVE_H_

// x = 1, y = 2, z = 3 as defined by Madlib.h
#define XAXIS 1
#define YAXIS 2
#define ZAXIS 3  

#define VALIDX 0x1
#define VALIDY 0x2
#define VALIDZ 0x4
#define AXIS_MASK 0x7

#define NUM_STEPS_16 65535		// total number of steps that can be made by a 16 bit device
#define NUM_STEPS_20 1048575	// total number of steps that can be made by a 20 bit device

// External names used used by the rest of the system
// to load particular device from the "MCL_NanoDrive.dll" library
static const char* g_StageDeviceName = "MCL NanoDrive Z Stage";
static const char* g_XYStageDeviceName = "MCL NanoDrive XY Stage";

// Keywords - used when creating properties
static const char* g_Keyword_SetPosZUm = "Set position Z (um)";
static const char* g_Keyword_SetPosXUm = "Set position X (um)";
static const char* g_Keyword_SetPosYUm = "Set position Y (um)";
static const char* g_Keyword_SetOrigin = "Set origin here";

#endif //_MCL_NANODRIVE_H_
