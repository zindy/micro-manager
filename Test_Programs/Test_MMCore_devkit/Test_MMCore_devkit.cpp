/*
 * Command-line test program for MMCore and device drivers.
 * This file is built for Win32 development and may require small
 * modifications to compile on Mac or Linux.
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, 10/01/2007
 *
 * Copyright (c) 2007 Regents of the University of California
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

#include "../../MMCore/MMCore.h"

#ifdef WIN32
   //#include <windows.h>
#endif
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <string>
using namespace std;


// declaration of test methods
void TestDemoDevices(CMMCore& core);
void TestPixelSize(CMMCore& core);

/**
 * Creates MMCore object, loads configuration, prints the status and performs
 * a couple of basic tests.
 * 
 * Modify to exercise specific devices in more detail.
 */
int main(int argc, char* argv[])
{
   if (argc != 2)
   {
      cout << "Invalid number of command-line parameters." << endl;
      cout << "Usage: Test_MMCore <configuration file name>" << endl;
      return 1;
   }

   try {
      // Create CMMCore      object
      CMMCore core;

      // load system configuration
	   core.loadSystemConfiguration(argv[1]);
      core.enableStderrLog(false); // supress console echo of log/debug messages
   
      // print current device status
      // (this should work for any configuration)
      vector<string> devices = core.getLoadedDevices();
      for (size_t i=0; i<devices.size(); i++)
      {
         cout << devices[i] << endl;
         vector<string> props = core.getDevicePropertyNames(devices[i].c_str());
         for (size_t j=0; j<props.size(); j++)
         {
            string val = core.getProperty(devices[i].c_str(), props[j].c_str());
            cout << "    " << props[j] << "=" << val << endl;
         }
      }
	
      // add any testing routines here...

      // TestDemoDevices is just an example for a testing rountine
      // It assumes that specific demo configuration is already loaded
      TestDemoDevices(core);
      //TestPixelSize(core);

      // clean-up before exiting
	   core.unloadAllDevices();
   }
   catch (CMMError& err)
   {
      cout << "Exception: " << err.getMsg() << endl;
      cout << "Exiting now." << endl;
      return 1;
   }

   cout << "Test_MMCore ended OK." << endl;
   return 0;
}

/**
 * Test routine for the MMConfig_Demo.cfg.
 * Device names must match
 */
void TestDemoDevices(CMMCore& core)
{
   const char* XYStageName = "XY";
   const char* wheelName = "Emission";

   // Example 1: move filter wheel to state(position) 3
   // -------------------------------------------------
   core.setState(wheelName, 3);
   core.waitForDevice(wheelName);

   long state = core.getState(wheelName);
   cout << "State device " << wheelName << " in state " << state << endl;

   // Example 2: move filter wheel to specific label (must be previously defined)
   // ---------------------------------------------------------------------------
   core.setStateLabel(wheelName, "Chroma-HQ620");
   core.waitForDevice(wheelName);

   state = core.getState(wheelName);
   string stateLabel = core.getStateLabel(wheelName);
   cout << "State device " << wheelName << " in state " << state << ", labeled as " << stateLabel << endl;

   // Example 3: move multiple filters at once using one of the predefined configurations
   // -----------------------------------------------------------------------------------
   core.setConfig("Channel", "DAPI");
   core.waitForSystem();

   // print current status for all state devices
   vector<string> stateDevices = core.getLoadedDevicesOfType(MM::StateDevice);
   for (size_t i=0; i<stateDevices.size(); i++)
   {
      state = core.getState(stateDevices[i].c_str());
      stateLabel = core.getStateLabel(stateDevices[i].c_str());
      cout << "State device " << stateDevices[i] << " in state " << state << ", labeled as " << stateLabel << endl;
   }

   // Example 4: snap an image
   // ------------------------
   core.setExposure(100.0);
   core.setProperty("Camera", "PixelType", "8bit");
   core.snapImage();
   cout << "Image snapped." << endl;

   // Example 5: move XYStage
   // -----------------------
   core.setXYPosition(XYStageName, 0.0, 0.0);
   core.waitForDevice(XYStageName);

   core.setXYPosition(XYStageName, 10000.0, 10000.0);
   core.waitForDevice(XYStageName);

   double x,y;
   core.getXYPosition(XYStageName, x, y);
   
   cout << "XY position = " << x << "," << y << endl;
}

void TestPixelSize(CMMCore& core)
{
   core.definePixelSizeConfig("Resolution10", "Objective", "State", "1");
   core.definePixelSizeConfig("Resolution20", "Objective", "State", "3");
   core.definePixelSizeConfig("Resolution40", "Objective", "State", "0");
   core.setPixelSizeUm("Resolution10", 1.0);
   core.setPixelSizeUm("Resolution20", 0.5);
   core.setPixelSizeUm("Resolution40", 0.25);

   core.setState("Objective", 2);
   cout << "Pixel size = " << core.getPixelSizeUm() << " um" << endl;

   core.setState("Objective", 1);
   cout << "Pixel size = " << core.getPixelSizeUm() << " um" << endl;

   core.setState("Objective", 3);
   cout << "Pixel size = " << core.getPixelSizeUm() << " um" << endl;

   core.setState("Objective", 0);
   cout << "Pixel size = " << core.getPixelSizeUm() << " um" << endl;

}
