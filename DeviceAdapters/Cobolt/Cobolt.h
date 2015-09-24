/*
 * Controls power levels of Cobolt lasers through a serial port
 *
 * AUTHOR:
 * Karl Bellve, Karl.Bellve@umassmed.edu
 * with contribution from alexis Maizel, alexis.maizel@cos.uni-heidelberg.de
 *
 * Copyright (c) 2009 University of Massachusetts Medical School
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

#ifndef _COBOLT_H_
#define _COBOLT_H_
#endif

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ModuleInterface.h"

#include <string>
#include <map>

//////////////////////////////////////////////////////////////////////////////
// Error codes
//

#define ERR_PORT_CHANGE_FORBIDDEN    101


//class Cobolt: public CGenericBase<Cobolt>
class Cobolt: public CShutterBase<Cobolt>
{
public:
    Cobolt();
    ~Cobolt();

    // MMDevice API
    // ------------
    int Initialize();
    int Shutdown();

    void GetName(char* pszName) const;
    bool Busy();
    int LaserOnOff(int);
    
 
    // action interface
    // ----------------
    int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnPowerMax(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnPowerSetPoint(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnPowerSetMax(MM::PropertyBase* pProp, MM::ActionType  eAct);
    int OnPowerStatus(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnHours(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnLaserOnOff(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnLaserStatus(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnCurrent(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnInterlock(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnFault(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnSerialNumber(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnVersion(MM::PropertyBase* pProp, MM::ActionType eAct);
    
    // Shutter API
    // ----------------
    int SetOpen(bool open = true);
    int GetOpen(bool& open);
    int Fire(double deltaT);

private:
    std::string name_;
    std::string port_;
    bool initialized_;
    bool busy_;
    std::string hours_;
    double maxPower_;
    std::string laserOn_;
    std::string laserStatus_;
    std::string interlock_;  
    std::string current_;
    std::string fault_;
    std::string serialNumber_;
    std::string version_;
    
    int GetState(int &value);
    int SetPowerSetpoint(double requestedPowerSetpoint);
    int GetPowerSetpoint(double& value);


};
