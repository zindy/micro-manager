/*
 * ASIFW1000 hub module. Required for operation of all
 * ASIFW1000 devices
 *
 * AUTHOR:
 * Nico Stuurman, nico@cmp.ucsf.edu, 02/02/2007
 * Based on NikonTE2000 controller adapter by Nenad Amodaj
 *
 * Copyright (c) 2006 Regents of the University of California
 * All rights reserved
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

#ifndef _ASIFW1000HUB_H_
#define _ASIFW1000HUB_H_

//#include <deque>
#include <map>
#include "../../MMDevice/MMDevice.h"

/////////////////////////////////////////////////////////////////////////
// Error codes
//
#define ERR_NOT_CONNECTED           11002
#define ERR_COMMAND_CANNOT_EXECUTE  11003
#define ERR_NO_ANSWER               11004
#define ERR_SETTING_WHEEL           11005
#define ERR_SETTING_VERBOSE_LEVEL   11006
#define ERR_SHUTTER_NOT_FOUND       11007
#define ERR_UNEXPECTED_ANSWER       11008


enum CommandMode
{
   Sync = 0,
   Async
};

class ASIFW1000Hub
{
public:
   ASIFW1000Hub();
   ~ASIFW1000Hub();

   void SetPort(const char* port) {port_ = port;}
   int GetVersion(MM::Device& device, MM::Core& core, char* version);
   int SetVerboseMode(MM::Device& device, MM::Core& core, int level);

   int OpenShutter(MM::Device& device, MM::Core& core, int shutterNr);
   int CloseShutter(MM::Device& device, MM::Core& core, int shutterNr);
   int GetShutterPosition(MM::Device& device, MM::Core& core, int shutterNr, bool& pos);

   int SetFilterWheelPosition(MM::Device& device, MM::Core& core, int wheelNr, int pos);
   int GetFilterWheelPosition(MM::Device& device, MM::Core& core, int wheelNr, int& pos);
   int SetCurrentWheel(MM::Device& device, MM::Core& core, int wheelNr);
   int GetCurrentWheel(MM::Device& device, MM::Core& core, int& wheelNr);
   int GetNumberOfPositions(MM::Device& device, MM::Core& core, int wheelNr, int& nrPos);
   int FilterWheelBusy(MM::Device& device, MM::Core& core, bool& busy);
   bool IsConnected();

private:
   int ExecuteCommand(MM::Device& device, MM::Core& core, const char* command);

   static const int RCV_BUF_LENGTH = 1024;
   char rcvBuf_[RCV_BUF_LENGTH];
   void ClearRcvBuf();
   void ClearAllRcvBuf(MM::Device& device, MM::Core& core);
   bool oldProtocol_;
   std::string port_;
   std::vector<char> answerBuf_;
   std::string commandMode_;
   long activeWheel_;
};

#endif // _ASIFW1000HUB_H_
