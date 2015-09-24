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

#ifndef _PI_CONTROLLER_H_
#define _PI_CONTROLLER_H_

#define PI_MOTION_ERROR -1024L
#define COM_ERROR -1L
#define PI_CNTR_NO_ERROR  0L
#define PI_CNTR_UNKNOWN_COMMAND 2L
#define PI_CNTR_MOVE_WITHOUT_REF_OR_NO_SERVO 5L
#define PI_CNTR_POS_OUT_OF_LIMITS  7L
#define PI_CNTR_VEL_OUT_OF_LIMITS 8L
#define PI_CNTR_INVALID_AXIS_IDENTIFIER 15L
#define PI_CNTR_PARAM_OUT_OF_RANGE 17L
#define PI_CNTR_ILLEGAL_AXIS 23L
#define PI_CNTR_AXIS_UNDER_JOYSTICK_CONTROL  51L
#define PI_CNTR_ON_LIMIT_SWITCH 216L
#define PI_CNTR_MOTION_ERROR 1024L

#define ERR_GCS_PI_CNTR_POS_OUT_OF_LIMITS 102
#define ERR_GCS_PI_CNTR_MOVE_WITHOUT_REF_OR_NO_SERVO 103
#define ERR_GCS_PI_CNTR_AXIS_UNDER_JOYSTICK_CONTROL 104
#define ERR_GCS_PI_CNTR_INVALID_AXIS_IDENTIFIER 105
#define ERR_GCS_PI_CNTR_ILLEGAL_AXIS 106
#define ERR_GCS_PI_CNTR_VEL_OUT_OF_LIMITS 107
#define ERR_GCS_PI_CNTR_ON_LIMIT_SWITCH 108
#define ERR_GCS_PI_CNTR_MOTION_ERROR 109
#define ERR_GCS_PI_MOTION_ERROR 110
#define ERR_GCS_PI_CNTR_PARAM_OUT_OF_RANGE 111
#define ERR_GCS_PI_NO_CONTROLLER_FOUND 112
#define ERR_DLL_PI_DLL_NOT_FOUND 113
#define ERR_DLL_PI_INVALID_INTERFACE_NAME 114
#define ERR_DLL_PI_INVALID_INTERFACE_PARAMETER 115

extern const char* g_msg_CNTR_POS_OUT_OF_LIMITS;
extern const char* g_msg_CNTR_MOVE_WITHOUT_REF_OR_NO_SERVO;
extern const char* g_msg_CNTR_AXIS_UNDER_JOYSTICK_CONTROL;
extern const char* g_msg_CNTR_INVALID_AXIS_IDENTIFIER;
extern const char* g_msg_CNTR_ILLEGAL_AXIS;
extern const char* g_msg_CNTR_VEL_OUT_OF_LIMITS;
extern const char* g_msg_CNTR_ON_LIMIT_SWITCH;
extern const char* g_msg_CNTR_MOTION_ERROR;
extern const char* g_msg_MOTION_ERROR;
extern const char* g_msg_CNTR_PARAM_OUT_OF_RANGE;
extern const char* g_msg_NO_CONTROLLER_FOUND;
extern const char* g_msg_DLL_NOT_FOUND;
extern const char* g_msg_INVALID_INTERFACE_NAME;
extern const char* g_msg_INVALID_INTERFACE_PARAMETER;

#include "../../MMDevice/DeviceBase.h"
#include <string>

#ifndef WIN32
#define WINAPI
#define BOOL int
#define TRUE 1
#define FALSE 0
#endif

size_t ci_find(const std::string& str1, const std::string& str2);

class PIController
{
public:
	explicit PIController(const std::string& label);
	virtual ~PIController();

	static PIController* GetByLabel(const std::string& label);
	static void DeleteByLabel(const std::string& label);

	int InitStage(const std::string& axisName, const std::string& stageType);

	bool IsBusy();
	int Home(const std::string& axesNames, const std::string& homingMode);
	double umToDefaultUnit_;

	std::string MakeAxesString(const std::string& axis1Name, const std::string& axis2Name) const;
	static std::vector<std::string> tokenize(const std::string& lines);

	int GetTranslatedError();
	int TranslateError( int err);
	
	virtual bool qIDN(std::string&)                                          {return false;}
	virtual bool  INI(const std::string&)                                    {return false;}
	virtual bool  CST(const std::string&, const std::string&)                {return false;}
	virtual bool  SVO(const std::string&, BOOL)                              {return false;}
	virtual bool  FRF(const std::string&)                                    {return false;}
	virtual bool  REF(const std::string&)                                    {return false;}
	virtual bool  MNL(const std::string&)                                    {return false;}
	virtual bool  FNL(const std::string&)                                    {return false;}
	virtual bool  FPL(const std::string&)                                    {return false;}
	virtual bool  MOV(const std::string&, const double*)                     {return false;}
	virtual bool  MOV(const std::string&, const std::string&, const double*) {return false;}
	virtual bool qPOS(const std::string&, double*)                           {return false;}
	virtual bool qPOS(const std::string&, const std::string&, double*)       {return false;}
	virtual bool  STP()                                                      {return false;}
	virtual bool  JON(int, int)                                              {return false;}
	virtual bool qJON(int, int&)                                             {return false;}
	virtual bool  VEL(const std::string&, const double*)                     {return false;}
	virtual bool qVEL(const std::string&, double*)                           {return false;}
	virtual bool qTPC(int&)                                                  {return false;}
	virtual bool  MPL(const std::string&)                                    {return false;}
	virtual bool IsReferencing(const std::string&, BOOL* )                   {return false;}
	virtual bool IsControllerReady( BOOL* )                                  {return false;}
	virtual bool IsMoving(const std::string&, BOOL* )                        {return false;}

	virtual int GetError() {return PI_CNTR_NO_ERROR;}

	virtual bool HasINI() {return false;}
	virtual bool HasSVO() {return false;}
	virtual bool HasCST() {return false;}
	virtual bool HasFRF() {return false;}
	virtual bool HasREF() {return false;}
	virtual bool HasFNL() {return false;}
	virtual bool HasMNL() {return false;}
	virtual bool HasFPL() {return false;}
	virtual bool HasMPL() {return false;}
	virtual bool HasJON() {return false;}
	virtual bool HasVEL() {return false;}
	virtual bool Has_qTPC() {return false;}
	virtual bool HasIsReferencing() {return false;}
	virtual bool HasIsControllerReady() {return false;}
	virtual bool HasIsMoving() {return false;}

	int FindNrJoysticks();
   int OnJoystick(MM::PropertyBase* pProp, MM::ActionType eAct, int joystick);
   int GetNrOutputChannels();

   MM::Core* logsink_;
   MM::Device* logdevice_;
protected:
   void LogMessage(const std::string& msg) const;
   bool gcs2_;
   std::string label_;
   bool onlyIDSTAGEvalid_;
   static std::map<std::string, PIController*> allControllersByLabel_;
   bool referenceMoveActive_;
   int m_ControllerError;
   //lint -e{1401} // dummy ctor without any initialization
   PIController () {}
};



#endif //_PI_CONTROLLER_H_
