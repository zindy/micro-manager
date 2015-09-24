/*
 * LeicaDMSTC XY stage adapter.
 *
 * AUTHOR:
 * G. Esteban Fernandez, 27-Aug-2012
 * Based on LeicaDMR adapter by Nico Stuurman.
 *
 * Copyright (c) 2012 , Children's Hospital Los Angeles
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

#ifndef _LeicaDMSTC_H_
#define _LeicaDMSTC_H_

#include "../../MMDevice/DeviceBase.h"
#include <string>
#include <map>

//////////////////////////////////////////////////////////////////////////////
// Error codes
//

#define ERR_UNKNOWN_COMMAND          10002
#define ERR_UNKNOWN_POSITION         10003
#define ERR_HALT_COMMAND             10004
#define ERR_CANNOT_CHANGE_PROPERTY   10005


#define ERR_PORT_NOT_SET            11001
#define ERR_NOT_CONNECTED           11002
#define ERR_COMMAND_CANNOT_EXECUTE  11003
#define ERR_NO_ANSWER               11004
#define ERR_DEVICE_NOT_FOUND       11005
#define ERR_UNEXPECTED_ANSWER       11006
#define ERR_INDEX_OUT_OF_BOUNDS     11007
#define ERR_INVALID_REFLECTOR_TURRET 11008
#define ERR_INVALID_POSITION        11009
#define ERR_OBJECTIVE_SET_FAILED    11010

#define ERR_UNEXPECTED_ANSWER_GetCommandString	110061
#define ERR_UNEXPECTED_ANSWER_SetCommandData	110062
#define ERR_UNEXPECTED_ANSWER_SetCommandDataXY	110063
#define ERR_UNEXPECTED_ANSWER_SetCommand		110064

class Hub : public CGenericBase<Hub>
{
public:
   Hub();
   ~Hub();
  
   // Device API
   // ----------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();

   // action interface
   // ----------------
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnMicroscope(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnVersion(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   bool initialized_;
   std::string version_;
   std::string micType;
   // MMCore name of serial port
   std::string port_;
};

class XYStage : public CXYStageBase<XYStage>
{
public:
   XYStage();
   ~XYStage();

   bool Busy();
   void GetName(char* pszName) const;
   int Initialize();
   int Shutdown();

   // Stage API   
   int SetPositionUm(double posX, double posY);
   int SetRelativePositionUm(double posX, double posY);
   int SetAdapterOriginUm(double x, double y);
   int GetPositionUm(double& posX, double& posY);
   int GetLimitsUm(double& xMin, double& xMax, double& yMin, double& yMax);
   //int Move(double speedX, double speedY);

   int SetPositionSteps(long stepsX, long stepsY);
   int GetPositionSteps(long& stepsX, long& stepsY);
   int SetRelativePositionSteps(long stepsX, long stepsY);
   int Home();
   int Stop();
   int SetOrigin();
   int GetStepLimits(long& xMin, long& xMax, long& yMin, long& yMax);
   double GetStepSizeXUm();
   double GetStepSizeYUm();
   
   
   int IsXYStageSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}
   // Sequence functions
   // Sequences can be used for fast acquisitions, sycnchronized by TTLs rather than
   // computer commands. 
   // Sequences of positions can be uploaded to the XY stage.  The device will cycle through
   // the uploaded list of states (triggered by an external trigger - most often coming 
   // from the camera).  If the device is capable (and ready) to do so isSequenceable will
   // be true. If your device can not execute this (true for most XY stages
   // simply set isSequenceable to false
   //int GetXYStageSequenceMaxLength(long& nrEvents) const = 0;
   //int StartXYStageSequence() const = 0;
   //int StopXYStageSequence() const = 0;
   // Remove all values in the sequence
   //int ClearXYStageSequence() const = 0;
   // Add one value to the sequence
   //int AddToXYStageSequence(double positionX, double positionY) const = 0;
   // Signal that we are done sending sequence values so that the adapter can send the whole sequence to the device
   //int SendXYStageSequence() const = 0;

   // action interface
   // ----------------
   //int OnAcceleration(MM::PropertyBase* pProp, MM::ActionType eAct);
   //int OnSpeedX(MM::PropertyBase* pProp, MM::ActionType eAct);
   //int OnSpeedY(MM::PropertyBase* pProp, MM::ActionType eAct);
   //int OnStop(MM::PropertyBase* pProp, MM::ActionType eAct);

private:                                                                     
   bool initialized_;
   double stepSize_um_;
   std::string name_;  
   long lowerLimitX_;
   long lowerLimitY_;
   long upperLimitX_;
   long upperLimitY_;
   long originXSteps_;
   long originYSteps_;

   MM::MMTime changedTime_;
}; 


#endif //_LeicaDMSTC_H_
