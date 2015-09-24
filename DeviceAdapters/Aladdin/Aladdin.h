/*
 * Aladdin pump controller adapter
 *
 * AUTHOR:
 * Kurt Thorn, UCSF, November 2011
 *
 * Copyright (c) 2011 Regents of the University of California
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

#ifndef _ALADDIN_H_
#define _ALADDIN_H_

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
//#define ERR_UNKNOWN_POSITION         10002
#define ERR_PORT_CHANGE_FORBIDDEN    10004

//enum TriggerType {OFF, RISING_EDGES, FALLING_EDGES, BOTH_EDGES, FOLLOW_PULSE};
//string TriggerLabels[] = {"Off","RisingEdges","FallingEdges","BothEdges","FollowPulse"};
//char TriggerCmd[] = {'Z', '+', '-', '*', 'X'};

class AladdinController : public CGenericBase<AladdinController>
{
public:
   AladdinController(const char* name);
   ~AladdinController();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();
   
   // action interface
   // ----------------
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnVolume(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnDiameter(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnRate(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnDirection(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnRun(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   bool initialized_;
   std::string name_;
   int error_;
   MM::MMTime changedTime_;

   std::string port_;
   string buf_string_;
   vector<string> buf_tokens_;

   void SetVolume(double volume);
   void GetVolume(double& volume);
   void SetDiameter(double diameter);
   void GetDiameter(double& diameter);
   void SetRate(double rate);
   void GetRate(double& rate);
   void SetRun(long run);
   void GetRun(long& run);
   void GetDirection (string& direction);
   void SetDirection (string direction);
   void GeneratePropertyVolume();
   void GeneratePropertyDiameter();
   void GeneratePropertyRate();
   void GeneratePropertyDirection();
   void GeneratePropertyRun();
   void CreateDefaultProgram();
   void StripString(string& StringToModify);
   void Send(string cmd);
   void ReceiveOneLine();
   void Purge();
   void GetUnterminatedSerialAnswer (std::string& ans, unsigned int count);
   int HandleErrors();

   static const int RCV_BUF_LENGTH = 1024;

   AladdinController& operator=(AladdinController& /*rhs*/) {assert(false); return *this;}
};


#endif // _ALADDIN_H_
