/*
 * Rapp Scanner adapter
 *
 * AUTHOR:
 * Arthur Edelstein, 12/22/2011
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

#ifndef _Rapp_H_
#define _Rapp_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include <string>
#include <map>

#pragma warning( push )
#pragma warning( disable : 4251 )
#include "obsROE_Device.h"
#pragma warning( pop )

class RappScanner : public CGalvoBase<RappScanner>
{
public:
   RappScanner();
   ~RappScanner();
  
   // Device API
   // ----------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();

   // so far, only the RappScanner attempts to get the controller status on initialization, so
   // that's where the device detection is going for now
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
   int OnCalibrationMode(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnSequence(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnTTLTriggered(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnSpotSize(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnRasterFrequency(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnMinimumRectSize(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnAccuracy(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnLaser(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   bool laser2_;
   bool initialized_;
   std::string port_;
   obsROE_Device* UGA_;
   long calibrationMode_;
   std::vector<tPointList> polygons_;
   long polygonAccuracy_;
   double polygonMinRectSize_;
   double currentX_;
   double currentY_;
   std::string sequence_;
   std::string ttlTriggered_;
   long rasterFrequency_;
   double spotSize_;
   double pulseTime_us_;
   bool stopOnOverflow_;

   // Helper functions
   int SafeStoreSequence(tStringList sequenceList);
   void RunDummyCalibration(bool laser2);

};

std::vector<std::string> & split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
std::string replaceChar(std::string str, char ch1, char ch2);

#endif //_Rapp_H_
