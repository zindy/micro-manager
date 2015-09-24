/*
 * Scion Firewire Camera Device Adapter
 *
 * AUTHOR:
 * Scion Corporation, 2009
 *
 * Copyright (c) 2004-2009 Scion Corporation
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

#ifdef WIN32
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
   #define snprintf _snprintf 
#endif

#include	"ScionCamera.h"
#include	<math.h>

extern	void	sLogMessage(char msg[]);

extern	int		command_period();


// local references

//----------------------------------------------------------------------------
//
//	get_frame() -	routine captures a frame 
//			
//					return -	0 = ok
//								1 = abort
//								2 = timeout
//								3 = stop continuous	
//								4 = buffer overflow	
//
//----------------------------------------------------------------------------

int CScionCamera::get_frame()
{
unsigned int	cc;
unsigned int	return_cc;

Cfw_image		*ip;

return_cc = 0;

restart_stream = 1;

// frame capture - using buffer 0

//cc = so.capture->start_frame();
//cc = so.capture->stop_frame();

cc = so.capture->get_frame();
if(cc == SFW_OK)
	{
	// frame complete

#ifdef	LOG_ENABLED
	sLogMessage("frame complete - get info\r\n");
#endif

	image_valid = 1;

	ip = so.capture->get_image();

	ii.buffer_no = so.capture->get_buffer_no();
	ii.bp = (unsigned char *)ip->get_bp();
	ii.format = ip->get_format();
	ii.depth = ip->get_component_depth();
	ii.image_mode = ip->get_image_type();
	ii.pi = ip;
	ii.image_valid = 1;
	}
else
	{
#ifdef	LOG_ENABLED
	sLogMessage("get image error\r\n");
#endif
	return_cc = 1;
	}

return(return_cc);
}	


//----------------------------------------------------------------------------
//
//	start_snap() -	routine to start a snap
//
//					the frame capture is starting and this routine
//					blocks until exposure time has completed then returns.
//
//					does not wait for the readout and transfer
//
//----------------------------------------------------------------------------

int	CScionCamera::start_snap()
{
unsigned int	cc;
unsigned int	return_cc;

return_cc = 0;

restart_stream = 1;

// frame capture - using buffer 0

cc = so.capture->start_frame();

snap_in_progress = true;

// wait for exposure duration (exposure in ms)
CDeviceUtils::SleepMs((long)d_exposure);

return(return_cc);
}

//----------------------------------------------------------------------------
//
//	complete_snap() -	routine to check for frame complete
//
//						return -	0 = ok
//									1 = abort
//									2 = timeout
//									3 = stop continuous	
//									4 = buffer overflow
//
//
//----------------------------------------------------------------------------

int	CScionCamera::complete_snap()
{
int			cc;
Cfw_image	*ip;

// wait for complete or time-out
cc = so.capture->stop_frame();

snap_in_progress = false;

if(cc != SFW_OK)
	{
	return(1);
	}

// frame complete

#ifdef	LOG_ENABLED
//sLogMessage("frame complete - get info\r\n");
#endif

image_valid = 1;

ip = so.capture->get_image();

ii.buffer_no = so.capture->get_buffer_no();
ii.bp = (unsigned char *)ip->get_bp();
ii.format = ip->get_format();
ii.depth = ip->get_component_depth();
ii.image_mode = ip->get_image_type();
ii.pi = ip;
ii.image_valid = 1;

return(0);
}


//----------------------------------------------------------------------------
//
//	start_frame() -	routine to initiate a frame capture sequence
//
//					this routine is used with double buffering
//
//
//----------------------------------------------------------------------------

int	CScionCamera::start_frame()
{
int					cc;

// start capture for selected buffer
cc = so.capture->start_frame();

return(1);
}


//----------------------------------------------------------------------------
//
//	complete_frame() -	routine to check for frame capture sequence complete
//
//						starts next frame.
//
//						this routine is used with double buffering
//
//						return -	0 = ok
//									1 = abort
//									2 = timeout
//									3 = stop continuous	
//									4 = buffer overflow
//
//
//----------------------------------------------------------------------------

int	CScionCamera::complete_frame()
{
int			cc;
Cfw_image	*ip;

// wait for complete or time-out
cc = so.capture->complete_frame();
if(cc != SFW_OK)
	{
	return(1);
	}

// frame complete

image_valid = 1;

ip = so.capture->get_image();

ii.buffer_no = so.capture->get_buffer_no();
ii.bp = (unsigned char *)ip->get_bp();
ii.format = ip->get_format();
ii.depth = ip->get_component_depth();
ii.image_mode = ip->get_image_type();
ii.pi = ip;
ii.image_valid = 1;
	
return(0);
}


//----------------------------------------------------------------------------
//
//	available_frame() -	routine to get latest available frame
//
//						starts next frame.
//
//						this routine is used with double buffering
//
//						return -	0 = ok
//									1 = abort
//									2 = timeout
//									3 = stop continuous	
//									4 = buffer overflow
//
//
//----------------------------------------------------------------------------

int	CScionCamera::available_frame()
{
int			cc;
Cfw_image	*ip;

// wait for complete or time-out
cc = so.capture->current_frame();
if(cc != SFW_OK)
	{
	return(1);
	}

// frame complete

image_valid = 1;

ip = so.capture->get_image();

ii.buffer_no = so.capture->get_buffer_no();
ii.bp = (unsigned char *)ip->get_bp();
ii.format = ip->get_format();
ii.depth = ip->get_component_depth();
ii.image_mode = ip->get_image_type();
ii.pi = ip;
ii.image_valid = 1;
	
return(0);
}


//----------------------------------------------------------------------------
//
//	IsCapturing() - returns true if sequence capture is active
//
//
//----------------------------------------------------------------------------

bool CScionCamera::IsCapturing()
{
return sequenceRunning_;
}


//----------------------------------------------------------------------------
//
//	InsertImage() - method to insert image into micro manager buffer pool
//
//----------------------------------------------------------------------------
int CScionCamera::InsertImage()
{
const unsigned char* img;

#ifdef	LOG_ENABLED
//sLogMessage("insert image in micro manager pool\r\n");
#endif

// call to GetImageBuffer will complete any pending capture
img = GetImageBuffer();
if (img == 0) 
	{	
	return DEVICE_ERR;
	}

int ret = GetCoreCallback()->InsertImage(this, img, GetImageWidth(), 
								GetImageHeight(), GetImageBytesPerPixel());

if (!stopOnOverflow_ && ret == DEVICE_BUFFER_OVERFLOW)
	{
	// do not stop on overflow - just reset the buffer
	GetCoreCallback()->ClearImageBuffer(this);
	return(GetCoreCallback()->InsertImage(this, img, 
				GetImageWidth(), GetImageHeight(), GetImageBytesPerPixel()));
	} 
else
	{
	return ret;
	}
}


void CScionCamera::OnThreadExiting() throw()
{
   try 
   {
      LogMessage(g_Msg_SEQUENCE_ACQUISITION_THREAD_EXITING);
      GetCoreCallback()->AcqFinished(this, 0);
   }
   catch (...) 
   {
      LogMessage(g_Msg_EXCEPTION_IN_ON_THREAD_EXITING, false);
   }
}

//----------------------------------------------------------------------------
//
//	svc() - thread routine to perform sequence capture
//
//----------------------------------------------------------------------------

int CScionCamera::SequenceThread::svc()
{
long count(0);

// capture requested number of images
while (!IsStopped() && count < numImages_)
{
	// get next image
	int ret = camera_->SnapImage();
	if (ret != DEVICE_OK)
   {
      camera_->sequenceRunning_ = false;
      camera_->OnThreadExiting();
		return(1);
   }

	// got image - insert image in micro manager buffer pool
	ret = camera_->InsertImage();
	if (ret != DEVICE_OK)
   {
      camera_->sequenceRunning_ = false;
      camera_->OnThreadExiting();
		return(1);
   }

	camera_->image_counter_++;
	count++;
}

// sequence complete
camera_->OnThreadExiting();
camera_->sequenceRunning_ = false;

return(0);
}


//----------------------------------------------------------------------------
//
//	StartSequenceAcquisition() - initiate sequence capture method
//
//
//----------------------------------------------------------------------------

int CScionCamera::StartSequenceAcquisition(long numImages, 
				double interval_ms, bool stopOnOverflow)
{
#ifdef	LOG_ENABLED
sLogMessage("start seqquence acquisition\r\n");
#endif

LogMessage("Starting sequence acquisition");

if (Busy() || sequenceRunning_)
	{return DEVICE_CAMERA_BUSY_ACQUIRING;}

// prepare core
int ret = GetCoreCallback()->PrepareForAcq(this);
if (ret != DEVICE_OK)
	{return ret;}

// make sure the circular buffer is properly sized
GetCoreCallback()->InitializeImageBuffer(1, 1, GetImageWidth(), 
							GetImageHeight(), GetImageBytesPerPixel());

stopOnOverflow_ = stopOnOverflow;
interval_ms = interval_ms;
sequenceLength_ = numImages;
image_counter_ = 0;

ctp->SetLength(numImages);
ctp->Start();

sequenceRunning_ = true;

LogMessage("Sequence acquisition started");

return DEVICE_OK;
}


//----------------------------------------------------------------------------
//
//	RestartSequenceAcquisition() - method to restart sequence
//
//
//----------------------------------------------------------------------------

int CScionCamera::RestartSequenceAcquisition() 
{
return StartSequenceAcquisition(sequenceLength_ - image_counter_, 
										interval_ms_, stopOnOverflow_);
}

//----------------------------------------------------------------------------
//
//	StopSequenceAcquisition() - method to stop sequence capture
//
//
//----------------------------------------------------------------------------

int CScionCamera::StopSequenceAcquisition()
{
#ifdef	LOG_ENABLED
sLogMessage("stop sequence acquisition\r\n");
#endif

LogMessage("Requesting sequence acquisition to stop", true);

ctp->Stop();
ctp->wait();
sequenceRunning_ = false;

return DEVICE_OK;
}


