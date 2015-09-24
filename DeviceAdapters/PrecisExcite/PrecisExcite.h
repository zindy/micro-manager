/*
 * PrecisExcite controller adapter
 *
 * AUTHOR:
 * Arthur Edelstein, arthuredelstein@gmail.com, 3/17/2009
 *
 * Copyright (c) 2009 Regents of the University of California
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

#ifndef _PRECISEXCITE_H_
#define _PRECISEXCITE_H_

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

enum TriggerType {OFF, RISING_EDGES, FALLING_EDGES, BOTH_EDGES, FOLLOW_PULSE};
string TriggerLabels[] = {"Off","RisingEdges","FallingEdges","BothEdges","FollowPulse"};
char TriggerCmd[] = {'Z', '+', '-', '*', 'X'};

class Controller : public CShutterBase<Controller>
{
public:
   Controller(const char* name);
   ~Controller();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();
   
   // action interface
   // ----------------
   int OnAddress(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnIntensity(MM::PropertyBase* pProp, MM::ActionType eAct, long index);
   //int OnChannelLetter(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnChannelLabel(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnConnectionType(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnReceivedData(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnTrigger(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnTriggerSequence(MM::PropertyBase* pProp, MM::ActionType eAct);



   // Shutter API
   int SetOpen(bool open = true);
   int GetOpen(bool& open);
   int Fire(double deltaT);


private:

   bool initialized_;
   long intensity_;
   long state_;
   std::string name_;
   bool busy_;
   int error_;
   MM::MMTime changedTime_;

   std::string port_;
   char currentChannelLetter_;
   string currentChannelLabel_;
   long currentChannel_;
   unsigned char buf_[1000];
   string buf_string_;
   vector<string> buf_tokens_;
   unsigned long buf_bytes_;
   long armState_;
   TriggerType triggerMode_;

   double answerTimeoutMs_;
   vector<char> channelLetters_;
   vector<string> channelLabels_;
   string triggerSequence_;
   string triggerMessage_;
   string trigger_;


   void SetIntensity(long intensity, long index);
   void GetIntensity(long& intensity, long index);
   void ReadGreeting();
   int ReadChannelLabels();
   void GeneratePropertyIntensity();
   void GenerateChannelChooser();
   void GeneratePropertyTrigger();
   void GeneratePropertyTriggerSequence();
   void GeneratePropertyState();
   void SetState(long state);
   void GetState(long &state);
   void Illuminate();
   void SetTrigger();
   void StripString(string& StringToModify);
   void Send(string cmd);
   void ReceiveOneLine();
   void Purge();
   int HandleErrors();
   

   Controller& operator=(Controller& /*rhs*/) {assert(false); return *this;}
};


#endif // _PRECISEXCITE_H_
