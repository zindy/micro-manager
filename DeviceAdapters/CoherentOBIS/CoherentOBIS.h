/*
 * CoherentObis controller adapter
 *
 * AUTHOR:
 * Forrest Collman
 * Adapted from CoherentCube driver written by Karl Hoover, UCSF
 *
 * Copyright (c) 2014 MBL, Woods Hole, MA
 * Copyright (c) 2009 Regents of the University of California,  (Hoover)
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

#ifndef _CoherentObis_H_
#define _CoherentObis_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/DeviceUtils.h"
#include <string>
//#include <iostream>
#include <vector>
using namespace std;

//////////////////////////////////////////////////////////////////////////////
// Error codes
//
#define ERR_PORT_CHANGE_FORBIDDEN    10004
#define ERR_DEVICE_NOT_FOUND         10005

#define POWERCONVERSION              1000 //convert the power into mW from the W it wants the commands in

class CoherentObis : public CShutterBase<CoherentObis>
{
private:
   double minlp_;
   double maxlp_;

public:
   // for any queriable token X such as P, SP, E, etc.
   // send the string ?X to the laser and then read a response such as
   // X=xyzzy, just return the result token xyzzy.
   std::string queryLaser( std::string thisToken )
   {
      std::string result;
      std::stringstream msg;
      msg <<  thisToken << "?";

      Purge();
      Send(msg.str());
      ReceiveOneLine();
      string buf_string = currentIOBuffer();
      return buf_string;
   }

   // for any settable token X, such as P, T, etc.
   // send the string X=value, then query ?X and return the string result
   std::string  setLaser( std::string thisToken, std::string thisValue)
   {
      stringstream msg;
      std::string result;

      msg << thisToken << " " << thisValue;
      Purge();
      Send(msg.str());
      ReceiveOneLine();
      string buf_string = currentIOBuffer();

      result = queryLaser(thisToken);
      return result;
   }

   void initLimits()
   {
      std::string val = queryLaser(maxPowerToken_);
      minlp(atof(val.c_str())*1000);
      val = queryLaser(maxPowerToken_);
      maxlp(atof(val.c_str())*1000);
   }

   // power setting limits:
   double minlp() { return minlp_; }
   void minlp(double v__) { minlp_= v__; }
   double maxlp() { return maxlp_; }
   void maxlp(double v__) { maxlp_= v__; }
   CoherentObis(const char* name);
   ~CoherentObis();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();
   const std::string currentIOBuffer() { return buf_string_; }
   
   // action interface
   // ----------------
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPowerSetpoint(MM::PropertyBase* pProp, MM::ActionType eAct, long index);
   int OnPowerReadback(MM::PropertyBase* pProp, MM::ActionType eAct, long index);
   int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);

   // some important read-only properties
   int OnHeadID(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnHeadUsageHours(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnMinimumLaserPower(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnMaximumLaserPower(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnWaveLength(MM::PropertyBase* pProp, MM::ActionType eAct/*, long*/);

   // Shutter API
   int SetOpen(bool open = true);
   int GetOpen(bool& open);
   int Fire(double deltaT);


private:
   bool initialized_;
   long state_;
   std::string name_;
   int error_;
   MM::MMTime changedTime_;

   // todo move these to DevImpl for better data hiding
   const std::string queryToken_;
   const std::string powerSetpointToken_;
   const std::string powerReadbackToken_;
   const std::string CDRHToken_;  // if this is on, laser delays 5 SEC before turning on
   const std::string CWToken_;
   const std::string laserOnToken_;
   const std::string TECServoToken_;
   const std::string headSerialNoToken_;
   const std::string headUsageHoursToken_;
   const std::string wavelengthToken_;
   const std::string externalPowerControlToken_;
   const std::string maxPowerToken_;
   const std::string minPowerToken_;

   std::string port_;

   string buf_string_;


   void SetPowerSetpoint(double powerSetpoint__, double& achievedSetpoint__);
   void GetPowerSetpoint(double& powerSetpoint__);
   void GetPowerReadback(double& value__);

   void GeneratePowerProperties();
   void GeneratePropertyState();

   void GenerateReadOnlyIDProperties();

   void SetState(long state);
   void GetState(long &state);

   void GetExternalLaserPowerControl(int& control__);
   void SetExternalLaserPowerControl(int control__);

   // todo -- can move these to the implementation
   void Send(string cmd);
   int ReceiveOneLine();
   void Purge();
   int HandleErrors();
   

   CoherentObis& operator=(CoherentObis& /*rhs*/) {assert(false); return *this;}
};


#endif // _CoherentObis_H_
