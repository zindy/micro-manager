/*
 * Zaber Controller Driver
 *
 * AUTHOR:
 * David Goosen & Athabasca Witschi (contact@zaber.com)
 *
 * Copyright (c) 2014 Zaber Technologies
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

#ifndef _ZABER_H_
#define _ZABER_H_

#include <MMDevice.h>
#include <DeviceBase.h>
#include <ModuleInterface.h>
#include <sstream>
#include <string>

//////////////////////////////////////////////////////////////////////////////
// Various constants: error codes, error messages
//////////////////////////////////////////////////////////////////////////////

#define ERR_PORT_CHANGE_FORBIDDEN    10002
#define ERR_DRIVER_DISABLED          10004
#define ERR_BUSY_TIMEOUT             10008
#define ERR_AXIS_COUNT               10016
#define ERR_COMMAND_REJECTED         10032
#define	ERR_NO_REFERENCE_POS         10064
#define	ERR_SETTING_FAILED           10128

extern const char* g_Msg_PORT_CHANGE_FORBIDDEN;
extern const char* g_Msg_DRIVER_DISABLED;
extern const char* g_Msg_BUSY_TIMEOUT;
extern const char* g_Msg_AXIS_COUNT;
extern const char* g_Msg_COMMAND_REJECTED;
extern const char* g_Msg_NO_REFERENCE_POS;
extern const char* g_Msg_SETTING_FAILED;

// N.B. Concrete device classes deriving ZaberBase must set core_ in
// Initialize().
class ZaberBase
{
public:
	ZaberBase(MM::Device *device);
	virtual ~ZaberBase();

protected:
	int ClearPort() const;
	int SendCommand(const std::string command) const;
	int QueryCommand(const std::string command, std::vector<std::string>& reply) const;
	int GetSetting(long device, long axis, std::string setting, long& data) const;
	int SetSetting(long device, long axis, std::string setting, long data) const;
	bool IsBusy(long device) const;
	int Stop(long device) const;
	int GetLimits(long device, long axis, long& min, long& max) const;
	int SendMoveCommand(long device, long axis, std::string type, long data) const;
	int SendAndPollUntilIdle(long device, long axis, std::string command, int timeoutMs) const;

	bool initialized_;
	std::string port_;
	MM::Device *device_;
	MM::Core *core_;
	std::string cmdPrefix_;
};

#endif //_ZABER_H_
