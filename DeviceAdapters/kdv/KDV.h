/*
 * Z-stage for Meiji Techno EMZ series
 *
 * AUTHOR:
 * Koen De Vleeschauwer, www.kdvelectronics.eu, 2010
 * Nenad Amodaj, nenad@amodaj.com, 08/28/2006
 *
 * Copyright (c) 2006 Regents of the University of California
 * Copyright (c) 2010 KDV Electronics
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

#ifndef _KDV_H
#define _KDV_H

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include <string>
#include <map>
#include <list>

// Error codes

#define ERR_PORT_CHANGE_FORBIDDEN      10001
#define ERR_STEPPER_FAIL               10002
#define ERR_SPEED_ACCEL_OUT_OF_RANGE   10003
#define ERR_SPEED_ACCEL_ALGORITHM_FAIL 10004
#define ERR_POSITION_OUTSIDE_RANGE     10005
#define ERR_SET_POSITION_FAILED        10006

#define ERR_TMCL_ANSWER_TIMEOUT        10101
#define ERR_TMCL_RECV_CHECKSUM_ERROR   10102
#define ERR_TMCL_VALUE_OUT_OF_RANGE    10103
#define ERR_TMCL_XMIT_CHECKSUM_ERROR   10104
#define ERR_TMCL_INVALID_COMMAND       10105
#define ERR_TMCL_WRONG_TYPE            10106
#define ERR_TMCL_INVALID_VALUE         10107
#define ERR_TMCL_CONFIGURATION_LOCKED  10108
#define ERR_TMCL_COMMAND_NOT_AVAIL     10109
#define ERR_TMCL_UNKNOWN_STATUS_CODE   10110

///////////////////////////////////////////////////////////////////////////////
//
// Trinamic Pandrive TMCL
//
///////////////////////////////////////////////////////////////////////////////

enum tmcl_cmd
{
   //Opcodes of all TMCL commands that can be used in direct mode
   TMCL_ROR = 1,
   TMCL_ROL = 2,
   TMCL_MST = 3,
   TMCL_MVP = 4,
   TMCL_SAP = 5,
   TMCL_GAP = 6,
   TMCL_STAP = 7,
   TMCL_RSAP = 8,
   TMCL_SGP = 9,
   TMCL_GGP = 10,
   TMCL_STGP = 11,
   TMCL_RSGP = 12,
   TMCL_RFS = 13,
   TMCL_SIO = 14,
   TMCL_GIO = 15,
   TMCL_SCO = 30,
   TMCL_GCO = 31,
   TMCL_CCO = 32,
   //Opcodes of TMCL control functions (to be used to run or abort a TMCL program in the module)
   TMCL_APPL_STOP = 128,
   TMCL_APPL_RUN = 129,
   TMCL_APPL_RESET = 131,
   // Get version
   TMCL_GET_VERSION = 136,
   // Request target position reached event
   TMCL_WAIT_EVENT = 138
};

enum tmcl_option
{
   // Options for MVP command
   MVP_ABS = 0,
   MVP_REL = 1,
   MVP_COORD = 2,
   // Options for the SAP command
   SAP_ACTUAL_POS = 1,
   SAP_MAXIMUM_SPEED = 4,
   SAP_MAXIMUM_ACCELERATION = 5,
   SAP_RAMP_DIVISOR = 153,
   SAP_PULSE_DIVISOR = 154,
   SAP_FREEWHEEL_TIMEOUT = 204,
   // Options for the GAP command
   GAP_ACTUAL_POS = 1,
   GAP_POS_REACHED = 8,
   // Options for RFS command
   RFS_START = 0,
   RFS_STOP = 1,
   RFS_STATUS = 2
};

enum tmcl_status
{
   TMCL_STATUS_CMD_OK = 100,
   TMCL_STATUS_EEPROM_OK = 101,
   TMCL_STATUS_POS_OK = 128,
   TMCL_STATUS_CSUM_ERR = 1,
   TMCL_STATUS_CMD_ERR = 2,
   TMCL_STATUS_TYPE_ERR = 3,
   TMCL_STATUS_VAL_ERR = 4,
   TMCL_STATUS_EEPROM_ERR = 5,
   TMCL_STATUS_AVAIL_ERR = 6
};

///////////////////////////////////////////////////////////////////////////////
//
// Z stage
//
///////////////////////////////////////////////////////////////////////////////

class ZStage : public CStageBase<ZStage>
{
public:
   ZStage();
   ~ZStage();

   // Device API
   // ----------
   int Initialize();
   int Shutdown();

   void GetName(char* pszName) const;
   bool Busy();

   // Stage API
   // ---------
   int SetPositionUm(double pos);
   int GetPositionUm(double& pos);
   int SetPositionSteps(long steps);
   int GetPositionSteps(long& steps);
   int SetOrigin();
   int GetLimits(double& min, double& max);

   int IsStageSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}
   bool IsContinuousFocusDrive() const {return false;}

   // action interface
   // ----------------
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnPortTimeoutMs(MM::PropertyBase* pProp, MM::ActionType eAct);

   int OnStepperSpeed(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnStepperIdleTimerS(MM::PropertyBase* pProp, MM::ActionType eAct);

   int OnStageStepSizeUm(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnStageTravelUm(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnStagePositioningTimeOutS(MM::PropertyBase* pProp, MM::ActionType eAct);

private:

   /* serial port */
   std::string port_;

   bool initialized_;
   double portTimeoutMs_;

   /* stepper drive */
   class speedParam
   {
   public:
	  std::string descr;
	  int pulseDivisor, velocity, acceleration, rampDivisor;
   };
   std::list<speedParam> speedSetting_; /* list of available stepper speeds */
   std::string stepperSpeed_; /* current stepper speed */
   long stepperIdleTimerS_;
   bool stepperParamsChanged_;
   double stepperMicroStepsPerStep_;

   /* stage */
   double stageStepSizeUm_;
   double stageTravelUm_;
   long stagePositioningTimeOutS_;

   int Home();
   int GetFirmwareVersion();
   int SetStepperParams(std::string stageSpeed, long idleTimerS);

   unsigned char tmclStatus_;
   int tmclReply_;
   int TMCL_Execute(unsigned int address, tmcl_cmd command, unsigned int type, unsigned int motor, int value);
   int TMCL_SendCommand(unsigned int address, tmcl_cmd command, unsigned int type, unsigned int motor, int value);
   int TMCL_ReceiveReply(double timeOutMs);
   int TMCL_InitErrorMessages();
};

#endif //_KDV_H
// not truncated
