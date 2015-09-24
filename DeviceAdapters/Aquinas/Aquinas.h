/*
 * Interfaces with Aquinas microfluidics controller
 *
 * AUTHOR:
 * Nico Stuurman, nico@cmp.ucsf.edu
 *
 * Copyright (c) 2011 Regents of the University of California
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

#ifndef _AQUINAS_H_
#define _AQUINAS_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ModuleInterface.h"

//////////////////////////////////////////////////////////////////////////////
// Error codes                                                
//

#define ERR_PORT_CHANGE_FORBIDDEN    101

class AqController: public CGenericBase<AqController>
{
public: 
   AqController();
   ~AqController();

    // MMDevice API
    // ------------
    int Initialize();
    int Shutdown();

    void GetName(char* pszName) const;
    bool Busy();

   // action interface
   // ---------------
   int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnID(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnSetPressure(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnValveState(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnValveOnOff(MM::PropertyBase* pProp, MM::ActionType eAct, long valveNr);

private:
   int SetValveState();

   std::string port_;
   bool initialized_;
   double pressureSetPoint_;
   unsigned char valveState_;
   std::string id_;
};

#endif // _AQUINAS_H_
