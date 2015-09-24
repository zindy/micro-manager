/*
 * Andor MicroPoint Scanner adapter
 *
 * AUTHOR:
 * Arthur Edelstein, 2013
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

#ifndef _MicroPoint_H_
#define _MicroPoint_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include <string>
#include <map>

class MicroPoint : public CGalvoBase<MicroPoint>
{
public:
   MicroPoint();
   ~MicroPoint();
  
   // Device API
   // ----------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();

   MM::DeviceDetectionStatus DetectDevice(void);

   // Galvo API
   int PointAndFire(double x, double y, double pulseTime_us);
   int SetSpotInterval(double pulseTime_us);
   int SetPosition(double x, double y);
   int GetPosition(double& x, double& y);
   int SetIlluminationState(bool on);
   int AddPolygonVertex(int polygonIndex, double x, double y);
   int DeletePolygons();
   int LoadPolygons();
   int SetPolygonRepetitions(int repetitions);
   int RunPolygons();
   int RunSequence();
   int StopSequence();
   int GetChannel(char* channelName);

   double GetXRange();
   double GetYRange();

   // Property action handlers
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAttenuator(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnRepetitions(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   bool initialized_;
   std::string port_;
   double x_;
   double y_;
   long attenuatorPosition_;
   long repetitions_;
   std::string attenuatorText_;

   int WriteBytes(unsigned char* buf, int numBytes);
   int ConfigurePortDirectionRegisters();

   double AttenuatorTransmissionFromIndex(long n);
   int StepAttenuatorPosition(bool positive);
   int MoveAttenuator(long steps);
   int CreateAttenuatorProperty();
   int CreateRepetitionsProperty();
   bool IsAttenuatorHome();
   long FindAttenuatorPosition();
   
   std::vector<std::vector<std::pair<double,double> > > polygons_;
   long polygonRepetitions_;
};

#endif //_MicroPoint_H_
