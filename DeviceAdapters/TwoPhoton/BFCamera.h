/*
 * Encapsulation for the Bitflow generic camera interface Ci
 *
 * AUTHOR:
 * Nenad Amodaj
 *
 * Copyright (c) 2011 Nenad Amodaj
 * Copyright (c) 2009 100X Imaging Inc
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

#include <vector>
#include	"BFApi.h"
#include	"BFErApi.h"
#include	"DSApi.h"
#include "CiApi.h"
#include "MMDevice.h"

#define BF_INCOMPATIBLE_CAMERAS 1010
#define BF_NOT_INITIALIZED 1011
#define BF_BUSY_ACQUIRING 1012
class BitFlowCamera;

class BFCamera
{
public:
   BFCamera(bool dual);
   ~BFCamera();

   int Initialize(MM::Device* caller, MM::Core* core);
   int Shutdown();
   unsigned long GetBufferSize() {return (unsigned long)width_ * height_ * depth_ + 2 * MAX_FRAME_OFFSET;}
   unsigned GetNumberOfBuffers() {return (unsigned) boards_.size();}
   const unsigned char* GetImageCont();
   int StartContinuousAcq() {return StartAcquiring();};
   int StopContinuousAcq() {return StopAcquiring();};
   int StartSequence() {return StartAcquiring();};
   int StopSequence() {return StopAcquiring();};
   bool isAcquiring() {return acquiring_;}
   int GetTimeout();
   void UseVFGs(std::string useChannels);
   bool VFGActive(int index);
  
   unsigned Width() const {return width_;}
   unsigned Height() const {return height_;}
   unsigned Depth() {return depth_;}
   int GetImageNumber() {return imageCount_;};

   bool isInitialized() {return initialized_;}

   static const int MAX_FRAME_OFFSET = 128;

private:
   unsigned char* buf_;
   std::vector<Bd> boards_;
   unsigned width_;
   unsigned height_;
   unsigned depth_;
   unsigned imageCount_;
   bool initialized_;
   std::vector<CiSIGNAL*> eofSignals_;
   BFU32 timeoutMs_;
   bool acquiring_;
   bool dual_;
   MM::Device* caller_;
   MM::Core* core_;
   std::vector<int> useVFGs_;


   int StartAcquiring();
   int StopAcquiring();
   void LogInterrupts();
   int SignalWaitErrorInterpret(BFRC ret);
   void ReloadBoardsAfterTimeoutError();
};
