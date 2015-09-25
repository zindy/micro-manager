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
File:		Madlib.h
Copyright:	Mad City Labs Inc., 2008
License:	Distributed under the BSD license.
*/
#ifndef _MADLIB_H_
#define _MADLIB_H_

#define		MCL_SUCCESS				 0
#define     MCL_GENERAL_ERROR		-1
#define		MCL_DEV_ERROR			-2
#define		MCL_DEV_NOT_ATTACHED	-3
#define		MCL_USAGE_ERROR			-4
#define		MCL_DEV_NOT_READY		-5
#define		MCL_ARGUMENT_ERROR		-6
#define		MCL_INVALID_AXIS		-7
#define		MCL_INVALID_HANDLE		-8
#define		MCL_INVALID_DRIVER      -9

#pragma pack(push, 1)
struct ProductInformation {
	unsigned char  axis_bitmap; //bitmap of available axis
	short ADC_resolution;		//# of bits of resolution
	short DAC_resolution;		//# of bits of resolution
	short Product_id;
	short FirmwareVersion;
	short FirmwareProfile;
};
#pragma pack(pop)

#ifdef __cplusplus
	extern"C"{
#else
	typedef unsigned char bool;
#endif

#define MADLIB_API

MADLIB_API	bool	MCL_InitLibrary(void *heap);
MADLIB_API  void	MCL_ReleaseLibrary();

MADLIB_API  int		MCL_GrabAllHandles();
MADLIB_API  int		MCL_GetAllHandles(int *handles, int size);
MADLIB_API  void	MCL_ReleaseHandle(int handle);
MADLIB_API	double	MCL_SingleReadN(unsigned int axis, int handle);
MADLIB_API	int		MCL_SingleWriteN(double position, unsigned int axis, int handle);
MADLIB_API	double	MCL_GetCalibration(unsigned int axis, int handle);
MADLIB_API	int		MCL_GetSerialNumber(int handle);
MADLIB_API	int		MCL_GetProductInfo(struct ProductInformation *pi, int handle);
MADLIB_API	bool	MCL_DeviceAttached(int milliseconds, int handle);

MADLIB_API  bool    MCL_CorrectDriverVersion();

#ifdef __cplusplus
	}
#endif

#endif
