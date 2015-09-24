/*
 * Thorlabs device adapters: TDC001 Controller (version 0.0)
 *
 * AUTHOR:
 * Emilio J. Gualda, IGC, 2012
 *
 * Copyright (c) 2012 Emilio J. Gualda
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

#ifndef _THORLABSDCSTAGE_H_
#define _THORLABSDCSTAGE_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include "APTAPI.h"
#include <string>
#include <map>

//////////////////////////////////////////////////////////////////////////////
// Error codes
//
#define ERR_PORT_CHANGE_FORBIDDEN    10004
#define ERR_UNRECOGNIZED_ANSWER      10009
#define ERR_UNSPECIFIED_ERROR        10010
#define ERR_HOME_REQUIRED            10011
#define ERR_INVALID_PACKET_LENGTH    10012
#define ERR_RESPONSE_TIMEOUT         10013
#define ERR_BUSY                     10014
#define ERR_STEPS_OUT_OF_RANGE       10015
#define ERR_STAGE_NOT_ZEROED         10016

// utility functions
int ClearPort(MM::Device& device, MM::Core& core);
unsigned const char* GenerateYCommand(const unsigned char* xCmd);

/////////////////////////////////////////////////////////////////////////////
// Device specific data structures
// Declarations copied from Thorlabs documentation 
/////////////////////////////////////////////////////////////////////////////
typedef struct _HWINFO
{
  unsigned long dwSerialNum;		   // Unique 8 digit serial number.
  char szModelNum[8];		         // Alphanumeric model number.
  unsigned short wHWType;		      // Hardware type ident (see #defines below).
  unsigned long dwSoftwareVersion;	// Software version
  char szNotes[64];		            // Arbitrary alphanumeric info string.
  unsigned short wNumChannels;		// Number of channels of operation
} HWINFO;

// velocity and acceleration parameters
typedef struct _MOTVELPARAMS
{
  unsigned short wChannel;	// Channel ident (see #defines earlier).
  long lMinVel;	         // Minimum (start) velocity
  long lAccn;		         // Acceleration in position pos. steps/sec*sec 
  long lMaxVel;	         // Maximum (final) velocity in pos. steps/sec 
} MOTVELPARAMS;

// motor status parameters
typedef struct _DCMOTSTATUS
{
   unsigned short wChannel;   // Channel ident.
	long lPosition;			   // Position in encoder counts. 
	unsigned short wVelocity;  // Velocity in encoder counts/sec.
	unsigned short wReserved;	// Controller specific use 
	unsigned long dwStatusBits;// Status bits (see #defines below).
} DCMOTSTATUS;

///////////////////////////////////////////////////////////////////////////////
// fixed stage parameters
///////////////////////////////////////////////////////////////////////////////
const int cmdLength = 6;               // command block length
const long xAxisMaxSteps = 2200000L;   // maximum number of steps in X
const long yAxisMaxSteps = 1500000L;   // maximum number of steps in Y
const double stepSizeUm = 0.05;        // step size in microns
const double accelScale = 13.7438;     // scaling factor for acceleration
const double velocityScale = 134218.0; // scaling factor for velocity

//////////////////////////////////////////////////////////////////////////////
// global utility
int ClearPort(MM::Device& device, MM::Core& core, std::string port);


// I change the name of the function PiesoZStage for ThorlabsDCStage

class ThorlabsDCStage : public CStageBase<ThorlabsDCStage>
{
public:
   ThorlabsDCStage();
   ~ThorlabsDCStage();
  
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
  // int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
     int OnPosition(MM::PropertyBase* pProp, MM::ActionType eAct);
	 int OnVelocity(MM::PropertyBase* pProp, MM::ActionType eAct); 
	 int OnHome(MM::PropertyBase* pProp, MM::ActionType eAct);


private:

//   bool GetValue(std::string& sMessage, double& pos);
//   int SetMaxTravel();
//   double GetTravelTimeMs(long steps);
   int SetHome(double home);
   bool IsHomed();
   int IsHomed2(double &home);
   int GetVelParam(double &vel);
   int SetVelParam(double vel);

   //Private variables
   std::string port_;
   double stepSizeUm_;
   bool initialized_;
   double answerTimeoutMs_;
   double maxTravelUm_;
   //double maxVelocity_;
   bool Homed_;
   bool HomeInProgress_;
   int hola_;
   long plNumUnits;
   long	plSerialNum[1];
   char szModel[256], szSWVer[256], szHWNotes[256];
   float curPosUm_; // cached current position
   float pfPosition[1];
   float newPosition[1];
   float newVel[1];
   float pfMaxVel[1];
   float pfMinVel[1];
   float pfAccn[1];
   float pfMaxAccn[1];
   double home;

};

#endif //_THORLABSDCSTAGE_H_
