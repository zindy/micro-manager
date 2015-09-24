/*
 * Controls Vortran Stradus Diode Laser Modules
 *
 * AUTHOR:
 * David Sweeney
 *
 * Copyright (c) 2012 Vortran Laser Technology, , All rights reserved.
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _STRADUS_H_
#define _STRADUS_H_
#endif

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ModuleInterface.h"

#include <string>
#include <map>
#include <vector>

#define ERR_PORT_CHANGE    102

class Stradus: public CShutterBase<Stradus>
{
public:
    Stradus();
    ~Stradus();

    // MMDevice API
    // ------------
    int Initialize();
    int Shutdown();

    void GetName(char* pszName) const;
    bool Busy();
    int LaserOnOff(int);
    int epcOnOff(int);
    int digModOnOff(int);

	// Shutter API
    int SetOpen(bool open = true);
    int GetOpen(bool& open);
    int Fire(double deltaT);

    // action interface
    // ----------------
    int OnBaseT(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnPower(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnPowerStatus(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnPulPwr(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnPulPwrStatus(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnHours(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnLaserOnOff(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnDigMod(MM::PropertyBase* pProp,MM::ActionType eAct);
    int OnEPC(MM::PropertyBase* pProp,MM::ActionType eAct);
    int OnCurrent(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnInterlock(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnFault(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnSerialNumber(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnVersion(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnFaultCode(MM::PropertyBase* pProp, MM::ActionType eAct);
    void Tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters);
    
private:
    std::string port_;
    bool initialized_;
    bool busy_;
    double power_;
    int pulPwr_;
    std::string laserOn_;
    std::string epc_;
    std::string digMod_;
    std::string emissionStatus_;
    std::string interlock_;
    std::string fault_;
    std::string faultCode_;
    std::string serialNumber_;
    std::string version_;
    std::string name_;
    std::string baseT_;
    std::string hours_;
    std::string current_;
};
