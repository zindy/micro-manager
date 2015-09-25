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
File:		MicroDriveXYStage.h
Copyright:	Mad City Labs Inc., 2008
License:	Distributed under the BSD license.
*/
#ifndef _MICRODRIVEXYSTAGE_H_
#define _MICRODRIVEXYSTAGE_H_



// MCL headers
#include "MicroDrive.h"
#include "MCL_MicroDrive.h"

// MM headers
#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ImgBuffer.h"
#include "../../MMDevice/ModuleInterface.h"

// List/heap headers
#include "device_list.h"
#include "handle_list_if.h"
#include "HandleListType.h"
#include "heap.h"

#include <string>
#include <map>

#define ERR_UNKNOWN_MODE         102
#define ERR_UNKNOWN_POSITION     103
#define ERR_NOT_VALID_INPUT      104

class MicroDriveXYStage : public CXYStageBase<MicroDriveXYStage>
{
public:
   MicroDriveXYStage();
   ~MicroDriveXYStage();

   bool Busy();
   void GetName(char* pszName) const;

   int Initialize();
   int Shutdown();
     
   // XYStage API
   virtual double GetStepSize();
   virtual int SetPositionSteps(long x, long y);
   virtual int GetPositionSteps(long& x, long& y);
   virtual int Home();
   virtual int Stop();
   virtual int SetOrigin();
   virtual int GetLimits(double& lower, double& upper);
   virtual int GetLimitsUm(double& xMin, double& xMax, double& yMin, double& yMax);
   virtual int GetStepLimits(long &xMin, long &xMax, long &yMin, long &yMax);
   virtual double GetStepSizeXUm();
   virtual double GetStepSizeYUm();
   virtual int SetRelativePositionUm(double dx, double dy);
   virtual int SetPositionUm(double x, double y);
   virtual int IsXYStageSequenceable(bool& isSequenceable) const; 
   virtual int GetPositionUm(double& x, double& y);

   // Action interface
   int OnPositionXmm(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPositionYmm(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnSetOriginHere(MM::PropertyBase* pProp, MM::ActionType eAct);  
   int OnCalibrate(MM::PropertyBase* pProp, MM::ActionType eAct);      
   int OnReturnToOrigin(MM::PropertyBase* pProp, MM::ActionType eAct);  
   int OnPositionXYmm(MM::PropertyBase* pProp, MM::ActionType eAct);  
   int OnVelocity(MM::PropertyBase* pProp, MM::ActionType eAct);  
   int OnEncoded(MM::PropertyBase* pProp, MM::ActionType eAct);  

private:
   /// Private methods

   // Initialization
   int CreateMicroDriveXYProperties();

   // Set/Get positions
   int SetPositionMm(double x, double y);
   int GetPositionMm(double& x, double& y);
   int SetRelativePositionMm(double x, double y);
   int SetPositionXSteps(long x);
   int SetPositionYSteps(long y);

   // Calibration & origin methods
   int Calibrate();
   int MoveToForwardLimits();
   int ReturnToOrigin();

   // Pause devices
   void PauseDevice(); 

   // Check if blocked
   bool XMoveBlocked(double possNewPos);
   bool YMoveBlocked(double possNewPos);

   void GetOrientation(bool& mirrorX, bool& mirrorY);

   /// Private variables
   int MCLhandle_;
   double stepSize_mm_;
   bool busy_;
   bool initialized_;
   double encoderResolution_; 
   double maxVelocity_;
   double maxVelocityTwoAxis_;
   double maxVelocityThreeAxis_;
   double minVelocity_;
   double velocity_;
   int rounding_;

   bool encoded_;
   double lastX_;
   double lastY_;
};


#endif //_MICRODRIVEXYSTAGE_H_
