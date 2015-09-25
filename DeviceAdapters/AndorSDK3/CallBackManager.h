/*
 * Copyright (c) 2007, Regents of the University of California
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

#ifndef _CallBackManagerH
#define _CallBackManagerH

#include "Property.h"

class MySequenceThread;
class CAndorSDK3Camera;
class SnapShotControl;
namespace andor {
   class IDevice;
};

class ICallBackManager
{
public:
   virtual ~ICallBackManager() {}
   virtual bool IsSSCPoised() = 0;
   virtual bool SSCEnterPoised() = 0;
   virtual bool SSCLeavePoised() = 0;
   virtual bool IsLiveModeActive() = 0;
   virtual int  CPCCreateProperty(const char* name, const char* value, MM::PropertyType eType, 
                                    bool readOnly, MM::ActionFunctor* pAct=0, bool initStatus=false) = 0;
   virtual int  CPCSetAllowedValues(const char* pszName, std::vector<std::string>& values) = 0;
   virtual int  CPCAddAllowedValueWithData(const char* pszName, const std::string & value, long data) = 0;
   virtual int  CPCLog(const char * msg) = 0;

   virtual void PauseLiveAcquisition() = 0;
   virtual void CPCRestartLiveAcquisition() = 0;
   virtual void CPCStopLiveAcquisition() = 0;
   virtual void CPCStartLiveAcquisition() = 0;
   virtual void CPCResizeImageBuffer() = 0;
   
   virtual int  CPCGetBinningFactor() = 0;
   virtual andor::IDevice * GetCameraDevice() = 0;

};

class CCallBackManager : public ICallBackManager
{
public:
   CCallBackManager(CAndorSDK3Camera * _parentClass, MySequenceThread * _seqAcqThread, SnapShotControl * _snapControl);
   ~CCallBackManager();

   virtual bool IsSSCPoised();
   virtual bool SSCEnterPoised();
   virtual bool SSCLeavePoised();
   virtual bool IsLiveModeActive();
   virtual int  CPCCreateProperty(const char* name, const char* value, MM::PropertyType eType, 
                                    bool readOnly, MM::ActionFunctor* pAct=0, bool initStatus=false);
   virtual int  CPCSetAllowedValues(const char* pszName, std::vector<std::string>& values);
   virtual int  CPCAddAllowedValueWithData(const char* pszName, const std::string & value, long data);
   virtual int  CPCLog(const char * msg);

   virtual void PauseLiveAcquisition();
   virtual void CPCRestartLiveAcquisition();
   virtual void CPCStopLiveAcquisition();
   virtual void CPCStartLiveAcquisition();
   virtual void CPCResizeImageBuffer();

   virtual int  CPCGetBinningFactor();
   virtual andor::IDevice * GetCameraDevice();


private:
   CAndorSDK3Camera * parentClass_;
   MySequenceThread * thd_;
   SnapShotControl  * ssControl_;

};

#endif	//include only once
