/*
 * Adapter for Velleman K8055 DAQ board
 *
 * AUTHOR:
 * Nico Stuurman, nico@cmp.ucsf.edu, 11/02/2007
 *
 * Copyright (c) 2007 Regents of the University of California
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

#ifndef _K8055Interface_H_
#define _K8055Interface_H_

//#include "/usr/local/include/usb.h"
#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"

class K8055Interface
{
public:
   K8055Interface();
   ~K8055Interface();

   int OpenDevice();
   int CloseDevice(MM::Device& device, MM::Core& core);
   long ReadAnalogChannel(MM::Device& device, MM::Core& core, long Channelno);
   int ReadAllAnalog(MM::Device& device, MM::Core& core, long* data1, long* data2);
   int OutputAnalogChannel(MM::Device& device, MM::Core& core, long channel, long data);
   int OutputAllAnalog(MM::Device& device, MM::Core& core, long data1,long data2);
   int ClearAllAnalog(MM::Device& device, MM::Core& core);
   int ClearAnalogChannel(MM::Device& device, MM::Core& core, long channel);
   int SetAnalogChannel(MM::Device& device, MM::Core& core, long channel);
   int SetAllAnalog(MM::Device& device, MM::Core& core);
   int WriteAllDigital(MM::Device& device, MM::Core& core, long data);
   int ClearDigitalChannel(MM::Device& device, MM::Core& core, long channel);
   int ClearAllDigital(MM::Device& device, MM::Core& core);
   int SetDigitalChannel(MM::Device& device, MM::Core& core, long channel);
   int SetAllDigital(MM::Device& device, MM::Core& core);
   int ReadDigitalChannel(MM::Device& device, MM::Core& core, long channel);
   long ReadAllDigital(MM::Device& device, MM::Core& core);
   int ResetCounter(MM::Device& device, MM::Core& core, long counternr);
   long ReadCounter(MM::Device& device, MM::Core& core, long counterno);
   int SetCounterDebounceTime(MM::Device& device, MM::Core& core, long counterno, long debouncetime);
   int WriteK8055Data(MM::Device& device, MM::Core& core, unsigned char cmd);
   int ReadK8055Data(MM::Device& device, MM::Core& core);

   std::string port_;
   bool initialized_;

private:
   long boardAddress_;
   unsigned char data_in[9], data_out[9];
};

#endif // _K8055Interface_H_
