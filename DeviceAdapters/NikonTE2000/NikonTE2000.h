/*
 * Nikon TE2000 adapter.
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, 05/03/2006
 *
 * Copyright (c)  Regents of the University of California
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

#ifndef _NIKON_TE2000_H_
#define _NIKON_TE2000_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include <string>
#include <map>

//////////////////////////////////////////////////////////////////////////////
// Error codes
//
#define ERR_NOT_CONNECTED         10002
#define ERR_UNKNOWN_POSITION      10003
#define ERR_TYPE_NOT_DETECTED     10004
#define ERR_EMPTY_ANSWER_RECEIVED 10005
#define ERR_PFS_NOT_CONNECTED     10006
#define ERR_PFS_FOCUS_FAILED      10007

class Hub : public HubBase<Hub>
{
public:
   Hub();
   ~Hub();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();
   int DetectInstalledDevices();

   // action interface
   // ----------------
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   void InstallIfMounted(std::string deviceName, const char* deviceCode);
   bool initialized_;
   std::string name_;
   std::string port_;
};

class Nosepiece : public CStateDeviceBase<Nosepiece>
{
public:
   Nosepiece();
   ~Nosepiece();
  
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

private:
   bool initialized_;
   unsigned numPos_;
   std::string name_;
};

class OpticalPath : public CStateDeviceBase<OpticalPath>
{
public:
   OpticalPath();
   ~OpticalPath();
  
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

private:
   bool initialized_;
   unsigned numPos_;
   std::string name_;
};

class Analyzer : public CStateDeviceBase<Analyzer>
{
public:
   Analyzer();
   ~Analyzer();
  
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

private:
   bool initialized_;
   unsigned numPos_;
   std::string name_;
};

class FilterBlock : public CStateDeviceBase<FilterBlock>
{
public:
   FilterBlock();
   ~FilterBlock();
  
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

private:
   bool initialized_;
   unsigned numPos_;
   std::string name_;
};

class ExcitationFilterBlock : public CStateDeviceBase<ExcitationFilterBlock>
{
public:
	ExcitationFilterBlock();
	~ExcitationFilterBlock();

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

private:
   bool initialized_;
   unsigned numPos_;
   std::string name_;
};

class FocusStage : public CStageBase<FocusStage>
{
public:
   FocusStage();
   ~FocusStage();

   bool Busy();
   void GetName(char* pszName) const;

   int Initialize();
   int Shutdown();
     
   // Stage API
   virtual int SetPositionUm(double pos);
   virtual int GetPositionUm(double& pos);
   virtual double GetStepSize() const {return (double)stepSize_nm_/1000;}
   virtual int SetPositionSteps(long steps) ;
   virtual int GetPositionSteps(long& steps);
   virtual int SetOrigin();
   virtual int GetLimits(double& lower, double& upper)
   {
      lower = lowerLimit_;
      upper = upperLimit_;
      return DEVICE_OK;
   }

   int IsStageSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}
   bool IsContinuousFocusDrive() const {return false;}

   // action interface
   // ----------------
   int OnPosition(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnStepSize(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   int stepSize_nm_;
   bool initialized_;
   double lowerLimit_;
   double upperLimit_;
};

//class Shutter : public CShutterBase<Shutter>
//{
//public:
//   Shutter();
//   ~Shutter();
//
//   bool Busy();
//   void GetName(char* pszName) const;
//   int Initialize();
//   int Shutdown();
//      
//   // Shutter API
//   int SetOpen(bool open = true);
//   int GetOpen(bool& open);
//   int Fire(double deltaT);
//
//   // action interface
//   // ----------------
//   int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);
//private:
//   bool initialized_;
//};
//

class Lamp : public CShutterBase<Lamp>
{
public:
   Lamp();
   ~Lamp();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();

   // Shutter API
   int SetOpen(bool open = true);
   int GetOpen(bool& open);
   int Fire(double deltaT);

   // action interface
   // ----------------
   int OnOnOff(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnVoltage(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnControl(MM::PropertyBase* pProp, MM::ActionType eAct);

   // additional (local) API
   // ----------------------
   void SetName(const char* name) {name_ = name;}

private:
   bool initialized_;
   std::string name_;
   MM::MMTime changedTime_;
};

class EpiShutter : public CShutterBase<EpiShutter>
{
public:
   EpiShutter();
   ~EpiShutter();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();

   // Shutter API
   int SetOpen(bool open = true);
   int GetOpen(bool& open);
   int Fire(double deltaT);

   // action interface
   // ----------------
   int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);

   // additional (local) API
   // ----------------------
   void SetName(const char* name) {name_ = name;}

private:
   bool initialized_;
   std::string name_;
   MM::MMTime changedTime_;
};

class UniblitzShutter : public CShutterBase<UniblitzShutter>
{
public:
   UniblitzShutter();
   ~UniblitzShutter();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();

   // Shutter API
   int SetOpen(bool open = true);
   int GetOpen(bool& open);
   int Fire(double deltaT);

   // action interface
   // ----------------
   int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnShutterNumber(MM::PropertyBase* pProp, MM::ActionType eAct);

   // additional (local) API
   // ----------------------
   void SetName(const char* name) {name_ = name;}

private:
   bool initialized_;
   std::string name_;
   long shutterNr_;
   long state_;
   MM::MMTime changedTime_;
};

//////////////////////////////////////////////////////////////////////////////
// AutoFocus class
// Nikon Perfect Focus control
//////////////////////////////////////////////////////////////////////////////
class PerfectFocus : public CAutoFocusBase<PerfectFocus>
{
public:
   PerfectFocus();
   ~PerfectFocus();
      
   // MMDevice API
   bool Busy();
   void GetName(char* pszName) const;

   int Initialize();
   int Shutdown();

   // AutoFocus API
   virtual int SetContinuousFocusing(bool state);
   virtual int GetContinuousFocusing(bool& state);
   virtual int FullFocus();
   virtual int IncrementalFocus();
   virtual int GetLastFocusScore(double& /*score*/) {return DEVICE_UNSUPPORTED_COMMAND;}
   virtual int GetCurrentFocusScore(double& score) {score = 0.0; return DEVICE_OK;}
   virtual bool IsContinuousFocusLocked(); 
   // These are required by the API.  However, we implemenet the PFSOffset drive instead
   virtual int GetOffset(double& offset) {offset = 0; return DEVICE_OK;};
   virtual int SetOffset(double /* offset */) {return DEVICE_OK;};

   // action interface
   // ----------------
   int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   bool initialized_;
};

class PFSOffset : public CStageBase<PFSOffset>
{
public:
   PFSOffset();
   ~PFSOffset();

   bool Busy();
   void GetName(char* pszName) const;

   int Initialize();
   int Shutdown();
     
   // Stage API
   virtual int SetPositionUm(double pos);
   virtual int GetPositionUm(double& pos);
   virtual double GetStepSize() const {return (double)stepSize_nm_/1000;}
   virtual int SetPositionSteps(long steps) ;
   // virtual int SetRelativePositionUm(double pos) ;
   virtual int GetPositionSteps(long& steps);
   virtual int SetOrigin();
   virtual int GetLimits(double& lower, double& upper)
   {
      lower = lowerLimit_;
      upper = upperLimit_;
      return DEVICE_OK;
   }

   bool IsContinuousFocusDrive() const {return true;}

   // action interface
   // ----------------
   int OnPosition(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnStepSize(MM::PropertyBase* pProp, MM::ActionType eAct);

   int IsStageSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}


private:
   double stepSize_nm_;
   bool initialized_;
   double lowerLimit_;
   double upperLimit_;
};

#endif //_NIKON_TE2000_H_
