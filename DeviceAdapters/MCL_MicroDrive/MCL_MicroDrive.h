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

/*
File:		MCL_MicroDrive.h
Copyright:	Mad City Labs Inc., 2008
License:	Distributed under the BSD license.
*/
#ifndef _MCL_MICRO_DRIVE_H
#define _MCL_MICRO_DRIVE_H

#define XAXIS 1
#define YAXIS 2
#define ZAXIS 3

#define VALIDX 0x1
#define VALIDY 0x2
#define VALIDZ 0x4
#define AXIS_MASK 0x7

#define X_REVERSE_LIMIT 0x01
#define X_FORWARD_LIMIT 0x02
#define Y_REVERSE_LIMIT 0x04
#define Y_FORWARD_LIMIT 0x08
#define Z_REVERSE_LIMIT 0x10
#define Z_FORWARD_LIMIT 0x20
#define BOTH_FORWARD_LIMITS 0xA

static const char* g_StageDeviceName = "MicroDrive Z Stage";
static const char* g_XYStageDeviceName = "MicroDrive XY Stage";

static const char* g_Keyword_SetPosXmm = "Set position X axis (mm)";
static const char* g_Keyword_SetPosYmm = "Set position Y axis (mm)";
static const char* g_Keyword_SetPosZmm = "Set position Z axis (mm)";
static const char* g_Keyword_SetOriginHere = "Set origin here";
static const char* g_Keyword_Calibrate = "Calibrate";
static const char* g_Keyword_ReturnToOrigin = "Return to origin";
static const char* g_Keyword_PositionTypeAbsRel = "Position type (absolute/relative)";
static const char* g_Keyword_Encoded = "EncodersPresent";

static const char* g_Listword_No = "No";
static const char* g_Listword_Yes = "Yes";
static const char* g_Listword_AbsPos = "Absolute Position";
static const char* g_Listword_RelPos = "Relative Position";

#endif
