/*
 * PI GCS Controller Driver
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, 08/28/2006
 * Steffen Rau, s.rau@pi.ws, 28/03/2008
 *
 * Copyright (c) 2006 Regents of the University of California
 * Copyright (c) 2008 Physik Instrumente (PI) GmbH & Co. KG
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

#ifndef _PI_GCS_CONTROLLER_H_
#define _PI_GCS_CONTROLLER_H_

#include "../../MMDevice/DeviceBase.h"
#include "Controller.h"
#include <string>

class PIGCSControllerCom;
class PIGCSControllerComDevice : public CGenericBase<PIGCSControllerComDevice>
{
public:
	PIGCSControllerComDevice();
	~PIGCSControllerComDevice();

   // Device API
   // ----------
   int Initialize();
   int Shutdown();
  
   void SetFactor_UmToDefaultUnit(double dUmToDefaultUnit, bool bHideProperty = true);

   void CreateProperties();

   static const char* DeviceName_;
   static const char* UmToDefaultUnitName_;
   void GetName(char* pszName) const;
   bool Busy();


   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnUmInDefaultUnit(MM::PropertyBase* pProp, MM::ActionType eAct);


	bool GCSCommandWithAnswer(const std::string command, std::vector<std::string>& answer, int nExpectedLines = -1);
	bool GCSCommandWithAnswer(unsigned char singleByte, std::vector<std::string>& answer, int nExpectedLines = -1);
	bool SendGCSCommand(const std::string command);
	bool SendGCSCommand(unsigned char singlebyte);
	bool ReadGCSAnswer(std::vector<std::string>& answer, int nExpectedLines = -1);
	int GetLastError() const { return lastError_; }

	double umToDefaultUnit_;
private:
	//int OnJoystick(MM::PropertyBase* pProp, MM::ActionType eAct, int joystick);
	int OnJoystick1(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnJoystick2(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnJoystick3(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnJoystick4(MM::PropertyBase* pProp, MM::ActionType eAct);

   std::string port_;
   int lastError_;
   bool initialized_;
   bool bShowProperty_UmToDefaultUnit_;
   PIGCSControllerCom* ctrl_;
};


class PIGCSControllerCom : public PIController
{
public:
	PIGCSControllerCom(const std::string& label, PIGCSControllerComDevice* proxy, MM::Core* logsink);
	~PIGCSControllerCom();

	int Connect();

	virtual bool qIDN(std::string& sIDN);
	virtual bool INI(const std::string& axis);
	virtual bool CST(const std::string& axis, const std::string& stagetype);
	virtual bool SVO(const std::string& axis, BOOL svo);
	virtual int GetError();
	virtual bool IsControllerReady( BOOL* );
	virtual bool IsMoving(const std::string& axes, BOOL* );
	virtual bool MOV(const std::string& axis, const double* target);
	virtual bool MOV(const std::string& axis1, const std::string& axis2, const double* target);
	virtual bool qPOS(const std::string& axis, double* position);
	virtual bool qPOS(const std::string& axis1, const std::string& axis2, double* position);
	virtual bool FRF(const std::string& axes);
	virtual bool REF(const std::string& axes);
	virtual bool MNL(const std::string& axes);
	virtual bool FNL(const std::string& axes);
	virtual bool FPL(const std::string& axes);
	virtual bool MPL(const std::string& axes);
	virtual bool STP();
	virtual bool JON(int joystick, int state);
	virtual bool qJON(int joystick, int& state);
	virtual bool VEL(const std::string& axis, const double* velocity);
	virtual bool qVEL(const std::string& axis, double* velocity);
	virtual bool qTPC(int& nrOutputChannels);
   bool ONL(std::vector<int> outputChannels, std::vector<int> values);

	virtual bool HasINI() {return hasINI_;}
	virtual bool HasSVO() {return hasSVO_;}
	virtual bool HasCST() {return hasCST_;}
	virtual bool HasIsReferencing() {return false;}
	virtual bool HasIsControllerReady() {return true;}
	virtual bool HasIsMoving() {return true;}
	virtual bool HasFRF() {return true;}
	virtual bool HasREF() {return true;}
	virtual bool HasFNL() {return true;}
	virtual bool HasMNL() {return true;}
	virtual bool HasFPL() {return true;}
	virtual bool HasMPL() {return true;}
	virtual bool HasJON() {return hasJON_;}
	virtual bool HasVEL() {return hasVEL_;}
	virtual bool Has_qTPC() {return has_qTPC_;}
   bool HasONL() const {return hasONL_;}

private:
	std::string ConvertToAxesStringWithSpaces(const std::string& axes) const;
	bool CheckError(bool& hasCmdFlag);
	bool CheckError(void);
   PIGCSControllerComDevice* deviceProxy_;
	bool hasCST_;
	bool hasSVO_;
	bool hasINI_;
	bool hasJON_;
	bool hasVEL_;
   bool has_qTPC_;
   bool hasONL_;
protected:
   //lint -e{1401} // dummy ctor without any initialization
   PIGCSControllerCom () {}
};




#endif //_PI_GCS_CONTROLLER_H_
