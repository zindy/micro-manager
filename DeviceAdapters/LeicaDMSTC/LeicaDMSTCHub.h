/*
 * LeicaDMSTC hub module. Required for operation of LeicaDMSTC
 * device.
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

#ifndef _LeicaDMSTCHUB_H_
#define _LeicaDMSTCHUB_H_

#include "../../MMDevice/MMDevice.h"


class LeicaDMSTCHub
{
public:
   LeicaDMSTCHub();
   ~LeicaDMSTCHub();

   void SetPort(const char* port) {port_ = port;}
   int Initialize(MM::Device& device, MM::Core& core);
   int DeInitialize() {initialized_ = false; return DEVICE_OK;};
   bool Initialized() {return initialized_;};
   std::string Version() {return version_;};
   std::string Microscope() {return microscope_;};

   int SetXYAbs(MM::Device& device, MM::Core& core, long positionX, long positionY);
   int GetXYAbs(MM::Device& device, MM::Core& core, long& positionX, long& positionY);

   int SetXYRel(MM::Device& device, MM::Core& core, long positionX, long positionY);

   int GetXYUpperThreshold(MM::Device& device, MM::Core& core, long& positionX, long& positionY);
   int GetXYLowerThreshold(MM::Device& device, MM::Core& core, long& positionX, long& positionY);

   int SetSpeedX(MM::Device& device, MM::Core& core, int speed);
   int GetSpeedX(MM::Device& device, MM::Core& core, int& speed);

   int SetSpeedY(MM::Device& device, MM::Core& core, int speed);
   int GetSpeedY(MM::Device& device, MM::Core& core, int& speed);
   
   int SetAcceleration(MM::Device& device, MM::Core& core, int acc);
   int GetAcceleration(MM::Device& device, MM::Core& core, int& acc);

   double GetStepSize(MM::Device& device, MM::Core& core);

   //int MoveXYConst(MM::Device& device, MM::Core& core, int speedX, int speedY);
   int StopXY(MM::Device& device, MM::Core& core);

   int SetAcknowledge(MM::Device& device, MM::Core& core, int ack);

   int ResetStage(MM::Device& device, MM::Core& core);
   int InitRange(MM::Device& device, MM::Core& core);

private:
   int GetVersion(MM::Device& device, MM::Core& core, std::string& version);
   int GetMicroscope(MM::Device& device, MM::Core& core, std::string& microscope);

   void ClearRcvBuf();
   void ClearAllRcvBuf(MM::Device& device, MM::Core& core);

   int GetCommand(MM::Device& device, MM::Core& core, int deviceId, int command, std::string& answer);
   int GetCommand(MM::Device& device, MM::Core& core, int deviceId, int command, int& answer1, int& answer2, int& answer3, int& answer4, int& answer5);
   int GetCommand(MM::Device& device, MM::Core& core, int deviceId, int command, int& answer1, int& answer2, int& answer3, int& answer4);
   int GetCommand(MM::Device& device, MM::Core& core, int deviceId, int command, int& answer);
   int SetCommand(MM::Device& device, MM::Core& core, int deviceId, int command, int dataX, int dataY);
   int SetCommand(MM::Device& device, MM::Core& core, int deviceId, int command, int data);
   int SetCommand(MM::Device& device, MM::Core& core, int deviceId, int command);

   // IDs of devices in the microscope
   // static const int gMic_ = 50;
   // static const int zDrive_ = 60;
   static const int xyStage_ = 10;

   static const int RCV_BUF_LENGTH = 1024;
   char rcvBuf_[RCV_BUF_LENGTH];

   std::string port_;
   std::string version_;
   std::string microscope_;
   bool initialized_;
};

#endif // _LeicaDMSTCHUB_H_
