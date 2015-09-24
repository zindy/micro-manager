/*
 * Newport Controller Driver
 *
 * AUTHOR:
 * Liisa Hirvonen, 03/17/2009
 * Nico Stuurman 08/18/2005, added velocity, multiple addresses, enabling multiple controllers, relative position, easier busy check and multiple fixes for annoying behavior, see repository logs for complete list
 *
 * Copyright (c) 2009-2013 University of Melbourne, Australia
 * Copyright (c) 2015 Regents of the University of California
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

#ifndef _Newport_H_
#define _Newport_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include <string>
#include <map>

//////////////////////////////////////////////////////////////////////////////
// Error codes
//
#define ERR_PORT_CHANGE_FORBIDDEN   10004
#define ERR_UNRECOGNIZED_ANSWER      10009
#define ERR_OFFSET                  10100
#define ERR_POSITION_BEYOND_LIMITS  10300
#define ERR_TIMEOUT                 10301
#define CONTROLLER_ERROR            20000

class NewportZStage : public CStageBase<NewportZStage>
{

public:
   NewportZStage();
   ~NewportZStage();

   // Device API
   // ----------
   int Initialize();
   int Shutdown();

   void GetName(char* pszName) const;
   bool Busy();

   // Stage API
   // ---------
   int SetPositionUm(double pos);
   int SetRelativePositionUm(double pos);
   int GetPositionUm(double& pos);
   int SetPositionSteps(long steps);
   int GetPositionSteps(long& steps);
   int SetOrigin();
   int GetLimits(double& min, double& max);

   // action interface
   // ----------------
   int OnConversionFactor(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnControllerAddress(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPosition(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnVelocity(MM::PropertyBase* pProp, MM::ActionType eAct);

   int IsStageSequenceable(bool& isSequenceable) const {
      isSequenceable = false;
      return DEVICE_OK;
   }
   bool IsContinuousFocusDrive() const {return false;}

private:
   int SetVelocity(double velocity);
   int GetVelocity(double& velocity);
   int GetError(bool& error, std::string& errorCode);
   int WaitForBusy();
   int GetValue(const char* cmd, double& val);
   std::string MakeCommand(const char* cmd);

   std::string port_;
   double stepSizeUm_;
   double conversionFactor_;
   int cAddress_;
   bool initialized_;
   double lowerLimit_; // limit in native coordinates
   double upperLimit_; // limit in native coordinates
   double velocity_;   // velocity in native coordinates
   double velocityLowerLimit_; // limit in native coordinates
   double velocityUpperLimit_; // limit in native coordinates
};


#endif //_Newport_H_
