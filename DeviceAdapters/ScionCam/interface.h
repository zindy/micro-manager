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

//////////////////////////////////////////////////////////////////////////////////////
//
//
//	sfwcore	- Scion Firewire Core Library
//
//	Version	1.0
//
//	Copyright 2004-2009  Scion Corporation  	(Win 2000/XP/Vista 32/64 Platforms)
//
//
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
//
//
//	File	interface.h
//
//	definitions for library inerface
//
//
//////////////////////////////////////////////////////////////////////////////////////

#if !defined(INTERFACE_H__INCLUDED_)
#define INTERFACE_H__INCLUDED_

#ifdef	_WIN32
#include "buffers.h"
#include "capture.h"
#include "device.h"
#include "imageinfo.h"
#include "settings.h"
#include "iformat.h"
#include "thread.h"
#else
#include	<sfwcore/buffers.h>
#include	<sfwcore/capture.h>
#include	<sfwcore/device.h>
#include	<sfwcore/imageinfo.h>
#include	<sfwcore/settings.h>
#include	<sfwcore/iformat.h>
#include	<sfwcore/thread.h>
#endif

#ifndef	DLLExport
#ifdef	_DLL
#define	DLLExport	__declspec (dllexport)
#else
#define	DLLExport	__declspec (dllimport)
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

DLLExport	Csfw_interface *	Create_sfw_interface();
DLLExport	void	Delete_sfw_interface(Csfw_interface *op);
DLLExport	SFW_HANDLE	sfw_interface_get_interface();
DLLExport	int		sfw_interface_list(LPSFW_LIST_HANDLE lp);
DLLExport	int		sfw_interface_dispose_object(LPSFW_OBJECT_HANDLE hp);
DLLExport	int		sfw_interface_camera_vendor_desc(unsigned int type, 
						unsigned int index, char *desc, unsigned int size);
DLLExport	int		sfw_interface_camera_vendor_prefix(unsigned int type, 
						unsigned int index, char *desc, unsigned int size);
DLLExport	int		sfw_interface_camera_product_desc(unsigned int type, 
						unsigned int index, char *desc, unsigned int size);
DLLExport	int		sfw_interface_camera_product_prefix(unsigned int type, 
						unsigned int index, char *desc, unsigned int size);

DLLExport	Cdevice *	Create_device();
DLLExport	void	Delete_device(Cdevice *op);

DLLExport	Ccam_settings *	Create_cam_settings();
DLLExport	void	Delete_cam_settings(Ccam_settings *op);

//DLLExport	Cconfig_collection * Create_config_collection();
//DLLExport	void	Delete_config_collection(Cconfig_collection *op);

DLLExport	Ccapture *	Create_capture();
DLLExport	Ccapture *	Create_capture_for_device(Cdevice *camera);
DLLExport	void	Delete_capture(Ccapture *op);

DLLExport	Cimage_core *	Create_image_core();
DLLExport	void	Delete_image_core(Cimage_core *op);

DLLExport	Cfw_image *	Create_fw_image();
DLLExport	void	Delete_fw_image(Cfw_image *op);

DLLExport	Cimage *	Create_image();
DLLExport	void	Delete_image(Cimage *op);

DLLExport	Cos_image *	Create_os_image();
DLLExport	void	Delete_os_image(Cos_image *op);

DLLExport	Cframe_info *	Create_frame_info();
DLLExport	void	Delete_frame_info(Cframe_info *op);

DLLExport	Cbuffer_pool *	Create_buffer_pool();
DLLExport	void	Delete_buffer_pool(Cbuffer_pool *op);

DLLExport	Ciformat *	Create_iformat();
DLLExport	void	Delete_iformat(Ciformat *op);

DLLExport	unsigned int classify_image(unsigned int image_type,
							unsigned int no_components,
							unsigned int component_depth,
							unsigned int component_order);

DLLExport	bool	decompose_format(const unsigned int format,
							unsigned int &image_type,
							unsigned int &no_components,
							unsigned int &component_depth,
							unsigned int &component_size,
							unsigned int &component_order,
							unsigned int &pixel_size);

DLLExport	Cimage_info *	Create_image_info();
DLLExport	void	Delete_image_info(Cimage_info *op);

DLLExport	Cimage_binfo *	Create_image_binfo();
DLLExport	void	Delete_image_binfo(Cimage_binfo *op);

DLLExport	Cthread_state *	Create_thread_state();
DLLExport	void	Delete_thread_state(Cthread_state *op);


#ifdef __cplusplus
}
#endif

#endif // !defined(DEVICE_H__INCLUDED_)
