/*
 * Prior ProScan controller adapter
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, 06/01/2006
 *
 * Copyright (c) 2006 Regents of the University of California
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

#ifndef _PRIOR_H_
#define _PRIOR_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include <string>
#include <map>

//////////////////////////////////////////////////////////////////////////////
// Error codes
//
#define ERR_UNKNOWN_POSITION         10002
#define ERR_PORT_CHANGE_FORBIDDEN    10004
#define ERR_SET_POSITION_FAILED      10005
#define ERR_INVALID_STEP_SIZE        10006
#define ERR_INVALID_MODE             10008
#define ERR_UNRECOGNIZED_ANSWER      10009
#define ERR_UNSPECIFIED_ERROR        10010

#define ERR_OFFSET 10100

int ClearPort(MM::Device& device, MM::Core& core);

class Shutter : public CShutterBase<Shutter>
{
public:
   Shutter(const char* name, int id);
   ~Shutter();

   bool Busy();
   void GetName(char* pszName) const;
   int Initialize();
   int Shutdown();
      
   // Shutter API
   int SetOpen(bool open = true);
   int GetOpen(bool& open);
   int Fire(double deltaT);

   // action interface
   // ----------------
   int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnDelay(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   int SetShutterPosition(bool state);
   int GetShutterPosition(bool& state);
   bool initialized_;
   const int id_;
   std::string name_;
   std:: string port_;
   MM::MMTime changedTime_;

   Shutter& operator=(Shutter& /*rhs*/) {assert(false); return *this;}
};

class Wheel : public CStateDeviceBase<Wheel>
{
public:
   Wheel(const char* name, unsigned id);
   ~Wheel();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();
   unsigned long GetNumberOfPositions()const {return numPos_;}

   // action interface
   // ----------------
   int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnDelay(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnBusy(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnSpeed(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   int SetWheelPosition(unsigned pos);

   bool initialized_;
   unsigned numPos_;
   const unsigned id_;
   std::string name_;
   std::string port_;
   unsigned curPos_;
   bool open_;
   bool busy_;
   MM::MMTime changedTime_;

   Wheel& operator=(Wheel& /*rhs*/) {assert(false); return *this;}
};


class XYStage : public CXYStageBase<XYStage>
{
public:
   XYStage();
   ~XYStage();
  
   // Device API
   // ----------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();

   // XYStage API
   // -----------
  int SetPositionSteps(long x, long y);
  int SetRelativePositionSteps(long x, long y);
  int GetPositionSteps(long& x, long& y);
  int Home();
  int Stop();
  int SetOrigin();//jizhen 4/12/2007
  int GetLimitsUm(double& xMin, double& xMax, double& yMin, double& yMax);
  int GetStepLimits(long& xMin, long& xMax, long& yMin, long& yMax);
  double GetStepSizeXUm() {return stepSizeXUm_;}
  double GetStepSizeYUm() {return stepSizeYUm_;}
  int IsXYStageSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}

   // action interface
   // ----------------
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnStepSizeX(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnStepSizeY(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnMaxSpeed(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAcceleration(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnSCurve(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   int GetResolution(double& resX, double& resY);
   int GetDblParameter(const char* command, double& param);
   int GetPositionStepsSingle(char axis, long& steps);
   bool HasCommand(std::string command);
   bool ControllerBusy();

   MMThreadLock lock_;
   bool initialized_;
   std::string port_;
   double stepSizeXUm_;
   double stepSizeYUm_;
   MM::MMTime changedTime_;
};

class ZStage : public CStageBase<ZStage>
{
public:
   ZStage();
   ~ZStage();
  
   // Device API
   // ----------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();

   // Stage API
   // ---------
  int SetPositionUm(double pos);
  int GetPositionUm(double& pos);
  int SetPositionSteps(long steps);
  int GetPositionSteps(long& steps);
  int SetOrigin();
  int GetLimits(double& min, double& max);

  int IsStageSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}
  bool IsContinuousFocusDrive() const {return false;}

   // action interface
   // ----------------
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnMaxSpeed(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAcceleration(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnSCurve(MM::PropertyBase* pProp, MM::ActionType eAct);


private:
   int ExecuteCommand(const std::string& cmd, std::string& response);
   int Autofocus(long param);
   int GetResolution(double& res);

   bool initialized_;
   std::string port_;
   double stepSizeUm_;
   long curSteps_;
   double answerTimeoutMs_;
   bool HasCommand(std::string command);
};

class NanoZStage : public CStageBase<NanoZStage>
{
public:
   NanoZStage();
   ~NanoZStage();
  
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

  int IsStageSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}
  bool IsContinuousFocusDrive() const {return false;}

   // action interface
   // ----------------
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnVersion(MM::PropertyBase* pProp, MM::ActionType eAct);


private:
   int ExecuteCommand(const std::string& cmd, std::string& response);
   bool HasCommand(std::string command);
   int GetModelAndVersion(std::string& model, std::string& version);

   bool initialized_;
   std::string port_;
   double stepSizeUm_;
   long curSteps_;
   double answerTimeoutMs_;
};

class BasicController : public CGenericBase<BasicController>
{
public:
   BasicController();
   ~BasicController();
  
   // Device API
   // ----------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();
   MM::DeviceType GetType() const {return MM::GenericDevice;}

   // action interface
   // ----------------
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnCommand(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnResponse(MM::PropertyBase* pProp, MM::ActionType eAct);;

private:
   int ExecuteCommand(const std::string& cmd, std::string& response);

   bool initialized_;
   std::string port_;
   double answerTimeoutMs_;
   std::string command_;
   std::string response_;
};

class Lumen : public CShutterBase<Lumen>
{
public:
   Lumen();
   ~Lumen();

   bool Busy();
   void GetName(char* pszName) const;
   int Initialize();
   int Shutdown();
      
   // Shutter API
   int SetOpen(bool open = true);
   int GetOpen(bool& open);
   int Fire(double deltaT);

   // action interface
   // ----------------
   int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnDelay(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnIntensity(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   int SetShutterPosition(bool state);
   bool initialized_;
   std:: string port_;
   MM::MMTime changedTime_;
   long intensity_;
   bool curState_;
};

class TTLShutter : public CShutterBase<TTLShutter>
{
public:
   TTLShutter(const char* name, int id);
   ~TTLShutter();

   bool Busy();
   void GetName(char* pszName) const;
   int Initialize();
   int Shutdown();
      
   // Shutter API
   int SetOpen(bool open = true);
   int GetOpen(bool& open);
   int Fire(double deltaT);

   // action interface
   // ----------------
   int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnDelay(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   int SetShutterPosition(bool state);
   int GetShutterPosition(bool& state);
   std::string name_;
   bool initialized_;
   const int id_;
   std:: string port_;
   MM::MMTime changedTime_;

   TTLShutter& operator=(TTLShutter& /*rhs*/) {assert(false); return *this;}
};

#endif //_PRIOR_H_
