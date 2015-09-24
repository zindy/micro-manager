/*
 * Standa stage
 *
 * AUTHOR:
 * Eugene Seliverstov, XIMC, http://ximc.ru
 *
 * Copyright (c) 2014-2015 XIMC
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

#ifndef HEADER_STANDA8SMC4_H
#define HEADER_STANDA8SMC4_H

#include "DeviceBase.h"
#include "DeviceThreads.h"
#include "ximc.h"

#define ERR_PORT_CHANGE    102

class Standa8SMC4Z : public CStageBase<Standa8SMC4Z>
{
public:
   Standa8SMC4Z();
   ~Standa8SMC4Z();

   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();

   void GetName(char* name) const;
   bool Busy();

   // Stage API
   virtual int SetPositionUm(double pos);
   virtual int Move(double velocity);
   virtual int SetAdapterOriginUm(double d);
   virtual int GetPositionUm(double& pos);
   virtual int SetPositionSteps(long steps);
   virtual int GetPositionSteps(long& steps);
   virtual int SetOrigin();
   virtual int GetLimits(double& lower, double& upper);

   virtual int IsStageSequenceable(bool& isSequenceable) const;

   virtual bool IsContinuousFocusDrive() const;

   // action interface
   // ----------------
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnUnitMultiplier(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   bool initialized_;
   std::string port_;
   double unitMultiplier_;
   long originSteps_;
   device_t device_;
   calibration_t calibration_;
   bool operationBusy_;
};

class Standa8SMC4XY : public CXYStageBase<Standa8SMC4XY>
{
public:
   Standa8SMC4XY();
   ~Standa8SMC4XY();

   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();

   void GetName(char* name) const;
   bool Busy();

   // XYStage API
   virtual int GetLimitsUm(double& xMin, double& xMax, double& yMin, double& yMax);
   virtual int Move(double vx, double vy);

   virtual int SetPositionSteps(long x, long y);
   virtual int GetPositionSteps(long& x, long& y);
   virtual int SetPositionUm(double x, double y);
   virtual int GetPositionUm(double& x, double& y);
   virtual int SetRelativePositionUm(double dx, double dy);
   virtual int SetRelativePositionSteps(long x, long y);

   virtual int Home();
   virtual int Stop();
   virtual int SetAdapterOriginUm(double x, double y);
   virtual int SetOrigin();//jizhen, 4/12/2007
   virtual int GetStepLimits(long& xMin, long& xMax, long& yMin, long& yMax);
   virtual double GetStepSizeXUm();
   virtual double GetStepSizeYUm();

   virtual int IsXYStageSequenceable(bool& isSequenceable) const;

   virtual bool IsContinuousFocusDrive() const;

   // action interface
   // ----------------
   int OnPortX(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPortY(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnUnitMultiplierX(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnUnitMultiplierY(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   int UpdatePositions(double dxum = 0.0, double dyum = 0.0);

private:
   bool initialized_;
   std::string portX_;
   std::string portY_;
   double unitMultiplierX_;
   double unitMultiplierY_;
   long originStepsX_;
   long originStepsY_;
   device_t deviceX_;
   device_t deviceY_;
   calibration_t calibrationX_;
   calibration_t calibrationY_;
   bool operationBusy_;
};

#endif // HEADER_STANDA8SMC4_H
// vim: ts=3 sw=3 et
