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

#ifndef _SCIONCAMERA_H_
#define _SCIONCAMERA_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include <string>
#include <map>

#ifdef WIN32
#include	"sfwlib.h"
#include	"buffers.h"
#include	"capture.h"
#include	"device.h"
#include	"imageinfo.h"
#include	"settings.h"
#include	"iformat.h"
#include	"thread.h"
#include	"interface.h"
#else
#include	<sfwcore/buffers.h>
#include	<sfwcore/capture.h>
#include	<sfwcore/device.h>
#include	<sfwcore/imageinfo.h>
#include	<sfwcore/settings.h>
#include	<sfwcore/iformat.h>
#include	<sfwcore/thread.h>
#include	<sfwcore/interface.h>
#endif

//
// for distribution - select PRODUCTION_VERSION
//

// define PRODUCTION_VERSION to enable production version options
#define PRODUCTION_VERSION


//
// production version selections - override above definitions
//

#ifdef PRODUCTION_VERSION
#undef LOG_ENABLED
//#define	LOG_ENABLED			// define LOG_ENABLED to enable log messages
//#define	ENABLE_SEQUENCE_ACQ		// define ENABLE_SEQUENCE_ACQ to enable threaded acquistion
#endif

typedef	struct
	{
	unsigned int		image_valid;		// 1 = image valid
	unsigned int		image_mode;			// 0 = grayscale, 1 = color
	unsigned int		format;				// image format
	unsigned int		depth;				// image depth
	unsigned int		buffer_no;			// buffer number
	unsigned char		*bp;				// buffer pointer
	Cfw_image			*pi;				// image pointer
	}	image_info_t;

class	CScionObjects
{
public:
	CScionObjects();
	virtual		~CScionObjects();
		
public:
	Csfw_interface	*cam_interface;			// camera interface
	Cdevice			*camera;				// camera device object
	Ccapture		*capture;				// capture control object

	Ccam_settings	*cam_config;			// camera configuration object
	Cimage_binfo	*image_info;			// capture image info

	Cimage			*b_image;				// image buffer (for conversions)
};


//////////////////////////////////////////////////////////////////////////////
// Error codes
//
#define ERR_UNKNOWN_MODE         102
#define ERR_UNKNOWN_POSITION     103


//////////////////////////////////////////////////////////////////////////////
// CScionCamera class
// Simulation of the Camera device
//////////////////////////////////////////////////////////////////////////////
class CScionCamera : public CCameraBase<CScionCamera>  
{
public:
   CScionCamera();
   CScionCamera(unsigned int camera_type);
   ~CScionCamera();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   void GetName(char* name) const;      
   bool Busy();
   
   // MMCamera API
   // ------------
   int SnapImage();
   const unsigned char* GetImageBuffer();
   long GetWaitTime() const;
   unsigned GetImageWidth() const;
   unsigned GetImageHeight() const;
   unsigned GetImageBytesPerPixel() const;
   unsigned GetBitDepth() const;
   long GetImageBufferSize() const;
   double GetExposure() const;
   void SetExposure(double exp);
   int GetBinning() const;
   int SetBinning(int binSize);
   int IsExposureSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}
   int SetROI(unsigned x, unsigned y, unsigned xSize, unsigned ySize); 
   int GetROI(unsigned& x, unsigned& y, unsigned& xSize, unsigned& ySize); 
   int ClearROI();
   int StartSequenceAcquisition(long numImages, double interval_ms, bool stopOnOverflow);
   int StopSequenceAcquisition();

   // action interface
   // ----------------

   int OnCameraType(MM::PropertyBase* pProp, MM::ActionType eAct);

   int OnBinning(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPixelType(MM::PropertyBase* pProp, MM::ActionType eAct);

   int OnTestMode(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPreviewMode(MM::PropertyBase* pProp, MM::ActionType eAct);

   int OnStreamMode(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnReadoutSpeed(MM::PropertyBase* pProp, MM::ActionType eAct);

   int OnGain(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnContrast(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnBlackLevel(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnExposure(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnShutterSync(MM::PropertyBase* pProp, MM::ActionType eAct);

   int OnGammaMode(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnGamma(MM::PropertyBase* pProp, MM::ActionType eAct);

   int OnRedGain(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnBlueGain(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnGreenGain(MM::PropertyBase* pProp, MM::ActionType eAct);

   int OnRedBoost(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnBlueBoost(MM::PropertyBase* pProp, MM::ActionType eAct);


private:
	static int		camera_index[18];		// camera index table

	bool			initialized_;
	bool			busy_;

	class SequenceThread : public MMDeviceThreadBase
		{
		public:
			SequenceThread(CScionCamera* pCam) : stop_(false), 
						numImages_(0) {camera_ = pCam;}
			~SequenceThread() {}

		// our implementation of virtual methods for sequence capture
		int svc(void);			// capture process
		
		// asnyc stop
		void Stop() 
      {
         MMThreadGuard(this->stopLock_);
         stop_ = true;
      }
     
       bool IsStopped()
       {
          MMThreadGuard(this->stopLock_);
          return stop_;
       }    

		// sequence start
		void Start()
			{
			stop_ = false;
			activate();
			}

		void SetLength(long images) {numImages_ = images;}

		private:
         MMThreadLock stopLock_;
			CScionCamera*	camera_;
         bool			stop_;
			long			numImages_;
		};

	SequenceThread	*ctp;
	bool			sequenceRunning_;
	bool			stopOnOverflow_;
	int         InsertImage();
	bool			IsCapturing();
   void        OnThreadExiting() throw();

	bool			snap_in_progress;
	bool			reload_config;
	bool			size_modified;

	unsigned long	image_counter_;
	unsigned long	sequenceLength_;
	double			interval_ms_;

	int				RestartSequenceAcquisition();
	int				SetCameraPropertyList();
	int				ResizeImageBuffer();
	
	CScionObjects	so;						// objects (wrapper to aid destructor maintenance)

	int				get_frame();			// get frame from camera
	int				start_snap();			// initiate a snap - blocks for exposure time
	int				complete_snap();		// complete current capture
	int				start_frame();			// start frame capture - does not block
	int				complete_frame();		// complete current captue
	int				available_frame();		// get most recent frame captured

	unsigned int	type;					// camera id
	std::string		typeName;				// Name of this type of camera
	unsigned int	index;					// camera index

	unsigned int	stream_mode;			// 1 = stream on
	unsigned int	restart_stream;			// 1 = restart stream

   bool           shutterSync_;

   long           frame_period_28mhz;     // frame time @28mhz clock rate
   long           frame_period_14mhz;     // frame time @14mhz clock rate
   long           frame_period_7mhz;      // frame time @7mhz clock rate


	double			d_gain;					// gain
	double			d_max_gain;				// max allowed gain value
	double			d_min_gain;				// min allowed gain value
	double			d_bl;					// black level
	double			d_red_gain;				// red gain
	double			d_green_gain;			// green gain
	double			d_blue_gain;			// blue gain
	double			d_gamma;				// gamma value 
	double			d_exposure;				// exposure
	
	unsigned int	pview_width;			// preview width for camera
	unsigned int	pview_height;			// preview height for camera
	unsigned int	max_width;				// max image width for camera
	unsigned int	max_height;				// max image height for camera

	unsigned int	image_width;			// image width
	unsigned int	image_height;			// image height
	unsigned int	image_rowbytes;			// image rowbytes
	unsigned int	image_depth;			// image bit depth
	unsigned int	image_components;		// image no of components
	unsigned int	image_component_size;	// image components size (bytes)
	unsigned int	image_pixel_size;		// image pixel size

	unsigned int	roi_width;				// roi width;
	unsigned int	roi_height;				// roi height;
	unsigned int	roix;					// roi x;
	unsigned int	roiy;					// roi y;
	RECT			roi;					// current region of interest
	unsigned int	roi_showing;			// 1 if roi displayed, else 0

	image_info_t	ii;						// captured image info

	unsigned int	image_valid;			// 1 = image valid
	Cimage_binfo	image_info;				// capture image info

	Cimage			b_image;				// image buffer (for conversions)
};


#endif //_SCIONCAMERA_H_
