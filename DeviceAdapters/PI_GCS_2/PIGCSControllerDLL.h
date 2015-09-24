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

#ifndef _PI_GCS_CONTROLLER_DLL_H_
#define _PI_GCS_CONTROLLER_DLL_H_

#include "../../MMDevice/DeviceBase.h"
#include "Controller.h"
#include <string>

class PIGCSControllerDLL;
class PIGCSControllerDLLDevice : public CGenericBase<PIGCSControllerDLLDevice>
{
public:
	PIGCSControllerDLLDevice();
	~PIGCSControllerDLLDevice();

   // Device API
   // ----------
   int Initialize();
   int Shutdown();

   void SetDLL(std::string dll_name);
   void SetInterface(std::string type, std::string parameter);
   void ShowInterfaceProperties(bool bShow);

   void CreateProperties();
   void CreateInterfaceProperties(void);
  
   static const char* DeviceName_;
   void GetName(char* pszName) const;
   bool Busy();


   static const char* PropName_;
   static const char* PropInterfaceType_;
   static const char* PropInterfaceParameter_;


   int OnDLLName(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnInterfaceType(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnInterfaceParameter(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
    int OnJoystick1(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnJoystick2(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnJoystick3(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnJoystick4(MM::PropertyBase* pProp, MM::ActionType eAct);

    PIGCSControllerDLL* ctrl_;
    std::string dllName_;
    std::string interfaceType_;
    std::string interfaceParameter_;
    bool initialized_;
    bool bShowInterfaceProperties_;
};

class PIGCSControllerDLL: public PIController
{
public:
	PIGCSControllerDLL(const std::string& label, PIGCSControllerDLLDevice* proxy, MM::Core* logsink);
	~PIGCSControllerDLL();

	virtual bool qIDN(std::string& sIDN);
	virtual bool INI(const std::string& axis);
	virtual bool CST(const std::string& axis, const std::string& stagetype);
	bool qCST(const std::string& axes, std::string& stages);
	virtual bool SVO(const std::string& axis, BOOL svo);
	virtual bool FRF(const std::string& axes);
	virtual bool REF(const std::string& axes);
	virtual bool MNL(const std::string& axes);
	virtual bool FNL(const std::string& axes);
	virtual bool FPL(const std::string& axes);
	virtual bool MPL(const std::string& axes);
	virtual int GetError();
	virtual bool IsReferencing(const std::string& axes, BOOL* );
	virtual bool IsControllerReady( BOOL* );
	virtual bool IsMoving(const std::string& axes, BOOL* );
	virtual bool MOV(const std::string& axis, const double* target);
	virtual bool MOV(const std::string& axis1, const std::string& axis2, const double* target);
	virtual bool qPOS(const std::string& axis, double* position);
	virtual bool qPOS(const std::string& axis1, const std::string& axis2, double* position);
	virtual bool STP();
	virtual bool JON(int joystick, int state);
	virtual bool qJON(int joystick, int& state);
	virtual bool VEL(const std::string& axis, const double* velocity);
	virtual bool qVEL(const std::string& axis, double* velocity);
	virtual bool qTPC(int& nrOutputChannels);

	virtual bool HasINI() {return INI_ != NULL;}
	virtual bool HasSVO() {return SVO_ != NULL;}
	virtual bool HasCST() {return CST_ != NULL;}
	bool HasqCST() const {return qCST_ != NULL;}
	virtual bool HasIsReferencing() { return IsReferencing_ != NULL; }
	virtual bool HasIsControllerReady() { return IsControllerReady_ != NULL; }
	virtual bool HasIsMoving() { return IsMoving_ != NULL; }
	virtual bool HasFRF() { return FRF_ != NULL; }
	virtual bool HasREF() { return REF_ != NULL; }
	virtual bool HasFNL() { return FNL_ != NULL; }
	virtual bool HasMNL() { return MNL_ != NULL; }
	virtual bool HasFPL() { return FPL_ != NULL; }
	virtual bool HasMPL() { return MPL_ != NULL; }
	virtual bool HasJON() { return JON_ != NULL; }
	virtual bool HasVEL() { return VEL_ != NULL; }
	virtual bool Has_qTPC() {return qTPC_ != NULL; }

	int LoadDLL(const std::string& dllName);
	int ConnectInterface(const std::string& interfaceType, const std::string& interfaceParameter);
	void CloseAndUnload();


private:
	typedef int ( WINAPI *FP_ConnectRS232 ) ( int, int );
	typedef int ( WINAPI *FP_Connect ) ( int );
	typedef int ( WINAPI *FP_IsConnected ) (int);
	typedef int ( WINAPI *FP_CloseConnection ) ( int );
	typedef int ( WINAPI *FP_EnumerateUSB ) ( char*, long, const char* );
	typedef int ( WINAPI *FP_ConnectUSB ) ( const char*);
	typedef int ( WINAPI *FP_GetError ) ( int );
	typedef int ( WINAPI *FP_qIDN ) ( int, char*, int );
	typedef int ( WINAPI *FP_qVER )	( int, char*, int );
	typedef int ( WINAPI *FP_INI ) ( int, const char* );
	typedef int ( WINAPI *FP_CST ) ( int, const char*, const char*);
	typedef int ( WINAPI *FP_qCST ) ( int, const char*, char*, int);
	typedef int ( WINAPI *FP_qFRF )	( int, const char*, BOOL* );
	typedef int ( WINAPI *FP_FRF ) ( int, const char* );
	typedef int ( WINAPI *FP_FPL ) ( int, const char* );
	typedef int ( WINAPI *FP_FNL ) ( int, const char* );
	typedef int ( WINAPI *FP_IsReferencing ) ( int, const char*, BOOL* );
	typedef int ( WINAPI *FP_IsControllerReady ) ( int, BOOL* );
	typedef int ( WINAPI *FP_IsMoving ) ( int, const char*, BOOL* );
	typedef int ( WINAPI *FP_REF ) ( int, const char* );
	typedef int ( WINAPI *FP_MPL ) ( int, const char* );
	typedef int ( WINAPI *FP_MNL ) ( int, const char* );
	typedef int ( WINAPI *FP_qPOS ) ( int, const char*, double* );
	typedef int ( WINAPI *FP_MOV ) ( int, const char*, const double* );
	typedef int ( WINAPI *FP_STP ) ( int );
	typedef int ( WINAPI *FP_SVO )	( int, const char*, const BOOL* );
	typedef int ( WINAPI *FP_qSVO )	( int, const char*, BOOL* );
	typedef int ( WINAPI *FP_JON )	( int, const int*, const BOOL*, int );
	typedef int ( WINAPI *FP_qJON )	( int, const int*, BOOL*, int );
	typedef int ( WINAPI *FP_qVEL ) ( int, const char*, double* );
	typedef int ( WINAPI *FP_VEL ) ( int, const char*, const double* );
	typedef int ( WINAPI *FP_qTPC ) ( int, int* );

	FP_ConnectRS232 ConnectRS232_;
	FP_Connect Connect_;
	FP_IsConnected IsConnected_;
	FP_CloseConnection CloseConnection_;
	FP_EnumerateUSB EnumerateUSB_;
	FP_ConnectUSB ConnectUSB_;
	FP_GetError GetError_;
	FP_qIDN qIDN_;
	FP_qVER qVER_;
	FP_INI INI_;
	FP_CST CST_;
	FP_qCST qCST_;
	FP_qFRF qFRF_;
	FP_FRF FRF_;
	FP_FPL FPL_;
	FP_FNL FNL_;
	FP_IsReferencing IsReferencing_;
	FP_IsControllerReady IsControllerReady_;
	FP_IsMoving IsMoving_;
	FP_REF REF_;
	FP_MPL MPL_;
	FP_MNL MNL_;
	FP_qPOS qPOS_;
	FP_MOV MOV_;
	FP_STP STP_;
	FP_SVO SVO_;
	FP_qSVO qSVO_;
	FP_JON JON_;
	FP_qJON qJON_;
	FP_VEL VEL_;
	FP_qVEL qVEL_;
    FP_qTPC qTPC_;


	void* LoadDLLFunc( const char* funcName );
	int ConnectPCI(const std::string& interfaceParameter);
	int ConnectRS232(const std::string& interfaceParameter);
	int ConnectUSB(const std::string& interfaceParameter);
	std::string FindDeviceNameInUSBList(const char* szDevices, std::string interfaceParameter) const;

   std::string dllPrefix_;
   int ID_;
   bool needResetStages_;
   std::string dllName_;
   std::string interfaceType_;
   std::string interfaceParameter_;

#ifdef WIN32
	HMODULE module_;
#else
	void* module_;
#endif

protected:
   //lint -e{1401} // dummy ctor without any initialization
   PIGCSControllerDLL () {}
};




#endif //_PI_GCS_CONTROLLER_DLL_H_
