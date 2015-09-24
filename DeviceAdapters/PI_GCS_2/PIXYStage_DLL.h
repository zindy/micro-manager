/*
 * PI GCS DLL ZStage
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, 08/28/2006
 * Steffen Rau, s.rau@pi.ws, 28/03/2008
 *
 * Copyright (c) 2006 Regents of the University of California
 * Copyright (c) 2008 Physik Instrumente (PI) GmbH & Co. KG
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

#ifndef _PI_XYSTAGE_DLL_H_
#define _PI_XYSTAGE_DLL_H_

#include "PI_GCS_2.h"
#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"

class PIController;

class PIXYStage : public CXYStageBase<PIXYStage>
{
public:
   PIXYStage();
   ~PIXYStage();
     
   // XYStage API
   bool Busy();
   void GetName(char* name) const;
   static const char* DeviceName_;

   int Initialize();
   int Shutdown();

   // XY Stage API
   virtual double GetStepSize();
   virtual int SetPositionSteps(long x, long y);
   virtual int GetPositionSteps(long &x, long &y);
   virtual int Home();
   virtual int Stop();
   virtual int SetOrigin();
   virtual int GetLimitsUm(double& xMin, double& xMax, double& yMin, double& yMax);
   virtual int GetStepLimits(long& xMin, long& xMax, long& yMin, long& yMax);
   virtual double GetStepSizeXUm();
   virtual double GetStepSizeYUm();

   // action interface
   // ----------------
   int OnControllerName(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAxisXName(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAxisXStageType(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAxisXHoming(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAxisYName(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAxisYStageType(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAxisYHoming(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnXVelocity(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnYVelocity(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnControllerNameYAxis(MM::PropertyBase* pProp, MM::ActionType eAct);

   int IsXYStageSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}


private:
   void CreateProperties();

   std::string axisXName_;
   std::string axisXStageType_;
   std::string axisXHomingMode_;
   std::string axisYName_;
   std::string axisYStageType_;
   std::string axisYHomingMode_;
   std::string controllerName_;
   std::string controllerNameYAxis_;

   PIController* ctrl_;
   PIController* ctrlYAxis_;

   double stepSize_um_;
   double originX_;
   double originY_;
   bool initialized_;
};


#endif //_PI_XYSTAGE_DLL_H_
