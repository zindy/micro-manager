/*
 * Controls Omicron xX-laserseries modules through serial port
 *
 * AUTHOR:
 * Jan-Erik Herche, Ralf Schlotter
 * -----------------------------------------------------------------------------
 *
 * Copyright (c) 2012 Omicron Laserage Laserprodukte GmbH
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

#ifndef _OMICRON_H_
#define _OMICRON_H_
#endif

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ModuleInterface.h"

#include <string>
#include <map>
#include <iomanip>
#include <iostream>

//-----------------------------------------------------------------------------
// Error code
//-----------------------------------------------------------------------------

#define ERR_PORT_CHANGE_FORBIDDEN    101

//-----------------------------------------------------------------------------

class Omicron: public CGenericBase<Omicron>
{
public:
    Omicron();
    ~Omicron();

    // MMDevice API
    int Initialize();
    int Shutdown();

    void GetName(char* pszName) const;
    bool Busy();
    int LaserOnOff(int);

    // Properties
    int OnCWSubOperatingmode(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnDevice(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnFault(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnHours(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnLaserOnOff(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnOperatingmode(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnPower1(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnPower2(MM::PropertyBase* pProp, MM::ActionType eAct);
    int OnPowerSetpoint1(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnPowerSetpoint2(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnPowerStatus(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnReset(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnSerialNumber(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnSpecPower(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnTemperatureDiode(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnTemperatureBaseplate(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnWavelength(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
   std::string port_;    
   bool initialized_;
	bool busy_;
	double power1_;
	double power2_;
   std::string laserOn_;
	std::string operatingmode_;
   std::string fault_;

   std::string serialNumber_;
	std::string hours_;
	std::string wavelength_;
	std::string specpower_;
	std::string suboperatingmode_;
    
   int specpower;
	int device_;
    


	bool PharseAnswerString(std::string &InputBuffer, const std::string &Kommando, std::vector<std::string> &ParameterVec);

};
