/*
 * Camera driver for gphoto2 cameras.
 *
 * AUTHOR:
 * Koen De Vleeschauwer, www.kdvelectronics.eu, 2011
 *
 * Copyright (c) 2011 Koen De Vleeschauwer, www.kdvelectronics.eu
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SIMPLECAM_H_
#define _SIMPLECAM_H_

#include <string>
#include <vector>
#include <sstream>
#define _SIMPLECAM_GPHOTO_
#include <gphoto2/gphoto2.h>
#include <gphoto2/gphoto2-version.h>
#include <gphoto2/gphoto2-port-version.h>
#include <FreeImagePlus.h>

// Device name and description

#define SIMPLECAM_DEVICENAME  "GPhoto"
#define SIMPLECAM_DESCRIPTION "GPhoto2 Generic Camera driver"

class CSimpleCam
{
public:
   CSimpleCam();
   ~CSimpleCam();
   bool listCameras(std::vector<std::string>& detected);             /* returns list of detected cameras */
   bool connectCamera(std::string cameraModelStr);                   /* attempt to connect to the camera. cameraModelStr is one of the cameras detected by listCameras */
   bool disconnectCamera();                                          /* disconnect from camera */
   bool isConnected();                                               /* true if camera is connected and ready */
   bool listShutterSpeeds(std::vector<std::string>& shutterSpeeds);  /* if connected to a camera, returns list of available shutter speeds */
   bool getShutterSpeed(std::string& currentShutterSpeed);           /* if connected to a camera, returns current shutter speed */
   bool setShutterSpeed(std::string newShutterSpeed);                /* if connected to a camera, sets new shutter speed. newShutterSpeed is one of the shutter speeds returned by listShutterSpeeds */
   bool listISOs(std::vector<std::string>& ISOs);                    /* if connected to a camera, returns list of available ISOs */
   bool getISO(std::string& currentISO);                             /* if connected to a camera, returns current ISO */
   bool setISO(std::string newISO);                                  /* if connected to a camera, sets new ISO. newISO is one of the shutter speeds returned by listISO */
   std::string captureImage();                                       /* if connected to a camera, takes a picture, and saves it to disk. return value is the filename of the picture */
   fipImage capturePreview();                                        /* if connected to a camera, returns a viewfinder preview. return value is a FreeImagePlus bitmap */

private:
   GPContext *context_;
   Camera   *camera_;
   int getShutterSpeedWidget(CameraWidget* &rootConfig, CameraWidget* &shutterSpeedWidget);
   int getISOWidget(CameraWidget* &rootConfig, CameraWidget* &isoWidget);
   int getWidget(CameraWidget* &rootConfig, CameraWidget* &configWidget, const char* configName);
   int setLibPaths();   /* Mac OS X: load gphoto2 camera drivers from libgphoto2/libgphoto2, and i/o drivers from libgphoto2/libgphoto2_port, if these directories exist. */
};

#endif //_SIMPLECAM_H_
// not truncated
