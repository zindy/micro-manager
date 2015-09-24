/*
 * Thorlabs device adapters: BBD Controller
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, 2011
 * http://nenad.amodaj.com
 *
 * Copyright (c) 2011 Thorlabs Inc
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

#ifndef _MOTORSTAGE_H_
#define _MOTORSTAGE_H_

#include "Thorlabs.h"

/////////////////////////////////////////////////////////////////////////////
// Device specific data structures
// Declarations copied from Thorlabs documentation 
/////////////////////////////////////////////////////////////////////////////
typedef struct _HWINFO
{
  unsigned int dwSerialNum;            // Unique 8 digit serial number.
  char szModelNum[8];                  // Alphanumeric model number.
  unsigned short wHWType;              // Hardware type ident (see #defines below).
  unsigned int dwSoftwareVersion;      // Software version
  char szNotes[64];                    // Arbitrary alphanumeric info string.
  unsigned short wNumChannels;         // Number of channels of operation
} HWINFO;

// velocity and acceleration parameters
typedef struct _MOTVELPARAMS
{
  unsigned short wChannel;   // Channel ident (see #defines earlier).
  unsigned int lMinVel;      // Minimum (start) velocity
  unsigned int lAccn;        // Acceleration in position pos. steps/sec*sec 
  unsigned int lMaxVel;      // Maximum (final) velocity in pos. steps/sec 
} MOTVELPARAMS;

// motor status parameters
typedef struct _DCMOTSTATUS
{
  unsigned short wChannel;   // Channel ident.
  unsigned int lPosition;    // Position in encoder counts. 
  unsigned short wVelocity;  // Velocity in encoder counts/sec.
  unsigned short wReserved;  // Controller specific use 
  unsigned int dwStatusBits; // Status bits (see #defines below).
} DCMOTSTATUS;

typedef enum {
  MOTORSTAGE_SERVO,
  MOTORSTAGE_STEPPER,
  MOTORSTAGE_UNDEFINED
} MOTTYPE;

class MotorStage
{
public:
   MotorStage(MM::Device *parent, MM::Core *core, std::string port, int axis, double answerTimeoutMs, double moveTimeoutMs);
   ~MotorStage() { }

   int Stop();
   int Home();
   int Enable();
   int ClearPort(void);
   int Initialize(HWINFO *info);
   int GetPositionSteps(long& p);
   int GetStatus(DCMOTSTATUS& stat);
   int MoveBlocking(long pos, bool relative);
   int GetVelocityProfile(MOTVELPARAMS& params);
   int SetVelocityProfile(const MOTVELPARAMS& params);
   int ParseVelocityProfile(const unsigned char* buf, int bufLen, MOTVELPARAMS& params);

private:
   int SendCommand(const ThorlabsCommand cmd);
   int SetCommand(const unsigned char* command, unsigned length);
   int ParseStatus(const unsigned char* buf, int bufLen, DCMOTSTATUS& stat);
   int ProcessEndOfMove(const unsigned char* buf, int bufLen);
   int GetCommandAnswer(unsigned char *reply, int reply_size, double timeout = -1, bool yieldToPolling = false);

   std::string port_;            // communications port
   int axis_;                    // which axis to control (0 = x, 1 = y, etc.)
   double moveTimeoutMs_;        //
   HWINFO info_;                 // device information
   MOTTYPE type_;                // type of stage (servo, stepper)
   MM::Device *parent_;  // parent device (XYStage, MotorXStage)
   MM::Core *core_;
   long currentPos_; 
   bool pollingPositionStep_;
   bool blockPolling_;   
};

#endif //_MOTORSTAGE_H_
