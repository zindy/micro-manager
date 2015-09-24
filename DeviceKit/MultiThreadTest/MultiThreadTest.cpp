/*
 * Command-line test program for MMCore and device drivers under
 * multiple-threads of execution
 *
 * AUTHOR:
 * Nenad Amodaj, http://nenad.amodaj.com
 *
 * Copyright (c) 2012 Regents of the University of California
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
#include "../../MMDevice/ImageMetadata.h"

#include <iostream>
#include <iomanip>
#include <assert.h>
#include <string>

using namespace std;

class TestThread : public MMDeviceThreadBase
{
public:
   TestThread(CMMCore* c) : core(c) {}
   ~TestThread() {}

   int svc();

private:
   CMMCore* core;
};

int TestThread::svc()
{
   return 0;
}

// main routine
int main(int argc, char* argv[])
{
   int retval = 0;

   if (argc != 2)
   {
      cout << "Invalid number of command-line parameters." << endl;
      cout << "Use: MultiThreadTest <configuration file name>" << endl;
      return 1;
   }

   try {
      CMMCore core;

      // load system configuration
      core.loadSystemConfiguration(argv[1]);
      core.enableStderrLog(false);
 
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
            cout << "    " << props[j] << "=" << val;
            if (core.hasPropertyLimits(devices[i].c_str(), props[j].c_str()))
            {
               cout << ", range: " << core.getPropertyLowerLimit(devices[i].c_str(), props[j].c_str())
                  << "-" << core.getPropertyUpperLimit(devices[i].c_str(), props[j].c_str());
            }
            cout << endl;
            std::cout.flush();
         }
      }

      // clean-up before exiting
      core.unloadAllDevices();
   }
   catch (CMMError& err)
   {
      cout << "Exception: " << err.getMsg() << endl;
      cout << "Exiting now." << endl;
      return 2;
   }

   return retval;
}

