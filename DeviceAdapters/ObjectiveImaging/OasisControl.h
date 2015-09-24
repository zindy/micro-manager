/*
 * Oasis Controller (Objective Imaging)
 *
 * AUTHOR:
 * Egor Zindy, egor.zindy@manchester.ac.uk
 * mostly based on NiMotionControl.h by
 * Brian Ashcroft, ashcroft@leidenuniv.nl
 *
 * Copyright (c) 2014 University of Manchester,  (OasisControl.h)
 * Copyright (c) 2009 Leiden University, Leiden,  (NiMotionControl.h)
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

#ifndef _NIMOTION_H_
#define _NIMOTION_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include "oasis4i.h"
#include "oi_const.h"
#include <string>
#include <map>

//////////////////////////////////////////////////////////////////////////////
// Error codes
//
#define ERR_UNKNOWN_MODE         102
#define ERR_UNKNOWN_POSITION     103

//////////////////////////////////////////////////////////////////////////////
// Global flag used for the initialisation of the OASIS subsystem.
// Want to initialise only once for any number of controllers
//
bool oasisInitialized = false;


//////////////////////////////////////////////////////////////////////////////
// OasisXYStage class
// Uses flexmotion to control an xy stage
//////////////////////////////////////////////////////////////////////////////

class OasisXYStage : public CXYStageBase<OasisXYStage>
{
private:
   int BoardID;					// Board identification number

   double stepSize_um_;
   double posX_um_;
   double posY_um_;
   bool busy_;
   bool initialized_;

   double lowerLimitX_um_;
   double upperLimitX_um_;
   double lowerLimitY_um_;
   double upperLimitY_um_;

   std::stringstream tmpMessage;

public:
   OasisXYStage();
   ~OasisXYStage();

   bool Busy();
   void GetName(char* pszName) const;

   int Initialize();
   int Shutdown();

   // XYStage API
   int SetPositionUm(double x, double y);// {posX_um_ = x; posY_um_ = y; return DEVICE_OK;};
   int SetPositionSteps(long x, long y);
   int GetPositionUm(double& x, double& y);// {x = posX_um_; y = posY_um_; return DEVICE_OK;}
   int GetPositionSteps(long& x, long& y);
   int GetLimitsUm(double& xMin, double& xMax, double& yMin, double& yMax);
   int GetStepLimits(long& xMin, long& xMax, long& yMin, long& yMax);
   int Stop();// {return DEVICE_OK;}
   int Home();
   int SetOrigin();

   //FIXME need real values here...
   virtual double GetStepSize() {return stepSize_um_;}
   virtual double GetStepSizeXUm() {return stepSize_um_;}
   virtual double GetStepSizeYUm() {return stepSize_um_;}

   //virtual int GetLimits(double& lower, double& upper)

   // action interface
   // ----------------
   int OnBoardID(MM::PropertyBase* pProp, MM::ActionType eAct);
   int IsXYStageSequenceable(bool & seq) const { seq=false; return DEVICE_OK;}

   void LogInit();
   void LogIt();
};

#endif //_DEMOCAMERA_H_
