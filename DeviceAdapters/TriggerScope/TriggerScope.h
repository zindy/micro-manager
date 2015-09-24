/*
 * Implements the ARC TriggerScope device adapter.
 * See http://www.trggerscope.com
 *
 * AUTHOR:
 * Austin Blanco, 5 Oct 2014
 *
 * Copyright (c) 2014 Advanced Research Consulting. ()
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#ifndef _TriggerScope_H_
#define _TriggerScope_H_

#include "../../MMDevice/DeviceBase.h"
#include <string>
#include <map>
#include <algorithm>
using namespace std;

//////////////////////////////////////////////////////////////////////////////
// Error codes
//
#define ERR_UNKNOWN_MODE         102
#define ERR_UNKNOWN_POSITION     103
#define ERR_IN_SEQUENCE          104
#define ERR_SEQUENCE_INACTIVE    105
#define ERR_STAGE_MOVING         106
#define SIMULATED_ERROR          200
#define HUB_NOT_AVAILABLE        107


//////////////////////////////////////////////////////////////////////////////
// CTriggerScope class
// TriggerScope device control
//////////////////////////////////////////////////////////////////////////////

class CTriggerScope : public CGenericBase<CTriggerScope>
{
public:
   CTriggerScope(void);
   ~CTriggerScope(void);

   bool Busy();
   void GetName(char* pszName) const;

   int Initialize();
   int Shutdown();


   void TestResourceLocking(const bool recurse);

   // action interface
   // ----------------
   int OnCOMPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnTTL1(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnTTL2(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnDAC(MM::PropertyBase* pProp, MM::ActionType eAct);


/////////////////////////////////////
//  Communications
/////////////////////////////////////


   void Send(string cmd);
   void ReceiveOneLine(int nLoopMax=5);
   void Purge();
   int HandleErrors();

   void ReceiveSerialBytes(unsigned char* buf, unsigned long buflen, unsigned long bytesToRead, unsigned long &totalBytes);
   void SendSerialBytes(unsigned char* cmd, unsigned long len);
   void ConvertSerialData(unsigned char *cmd, unsigned char *buf, unsigned long buflen, unsigned long *data, unsigned long datalen );
   void FlushSerialBytes(unsigned char* buf, unsigned long buflen);

private:
   bool busy_;
   MM::TimeoutMs* timeOutTimer_;
   string serial_string_;
   string buf_string_;
   string port_;
   int error_;
   double firmwareVer_;
   int cmdInProgress_;
   int initialized_;

   long ttl1_;
   long ttl2_;
   double dac_;

   MMThreadLock* pResourceLock_;

   MM::MMTime zeroTime_, currentTime_;
};

#endif //_TriggerScope_H_
