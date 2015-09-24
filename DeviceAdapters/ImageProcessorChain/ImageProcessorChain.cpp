/*
 * Runs a chain of other ImageProcessors on each image
 *
 * AUTHOR:
 * Karl Hoover
 *
 * Copyright (c) 2011 Regents of the University of California
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

#include "ImageProcessorChain.h"
#include <cstdio>
#include <string>
#include <math.h>
#include "../../MMDevice/ModuleInterface.h"
#include <sstream>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////

MODULE_API void InitializeModuleData()
{
   RegisterDevice("ImageProcessorChain", MM::ImageProcessorDevice, "ImageProcessorChain");
}

MODULE_API MM::Device* CreateDevice(const char* deviceName)
{
   if (deviceName == 0)
      return 0;


   else if(strcmp(deviceName, "ImageProcessorChain") == 0)
   {

      return new ImageProcessorChain();
   }

   // ...supplied name not recognized
   return 0;
}

MODULE_API void DeleteDevice(MM::Device* pDevice)
{
   delete pDevice;
}


int ImageProcessorChain::Initialize()
{


   std::vector<std::string> availableProcessors;
   availableProcessors.clear();
   availableProcessors.push_back("");
   char deviceName[MM::MaxStrLength];
   unsigned int deviceIterator = 0;
   for(;;)
   {
      GetLoadedDeviceOfType(MM::ImageProcessorDevice, deviceName, deviceIterator++);
      if( 0 < strlen(deviceName))
      {
         // let's not recursively chain the image processors....
         if( 0 != std::string(deviceName).compare(std::string("ImageProcessorChain")))
            availableProcessors.push_back(std::string(deviceName));
      }
      else
         break;
   }


   CPropertyActionEx* pAct = NULL;
   
   for( int ip = 0; ip < nSlots_; ++ip)
   {
      std::ostringstream processorSlotName;
      processorSlotName << "ProcessorSlot" << ip;
      pAct = new CPropertyActionEx (this, &ImageProcessorChain::OnProcessor, ip);
      (void)CreateProperty(processorSlotName.str().c_str(), "", MM::String, false, pAct); 
      for (std::vector<std::string>::iterator iap = availableProcessors.begin();  iap != availableProcessors.end(); ++iap)
         AddAllowedValue(processorSlotName.str().c_str(), iap->c_str());

   }

   return DEVICE_OK;
}

   // action interface
   // ----------------
int ImageProcessorChain::OnProcessor(MM::PropertyBase* pProp, MM::ActionType eAct, long indexx)
{
   if (eAct == MM::BeforeGet)
   {
      std::string name;
      if (processorNames_.end() != processorNames_.find((int)indexx))
         name = processorNames_[(int)indexx];
      pProp->Set(name.c_str());
   }
   else if (eAct == MM::AfterSet)
   {
      std::string name;
      pProp->Get(name);
      processorNames_[indexx] = name;

      for( int islot = 0; islot < this->nSlots_; ++islot)
      {
         processors_[islot] = NULL;
         if( processorNames_.end() != processorNames_.find(islot))
            if ( 0 < processorNames_[islot].length())
            {
               MM::Device* pDevice = GetDevice(processorNames_[islot].c_str());
               if( NULL != pDevice)
                  if( MM::ImageProcessorDevice == pDevice->GetType())
                     processors_[islot] = (MM::ImageProcessor*) pDevice;
            }
      }
      
   }

   return DEVICE_OK;
}


int ImageProcessorChain::Process(unsigned char *pBuffer, unsigned int width, unsigned int height, unsigned int byteDepth)
{
   int ret = DEVICE_OK;
   busy_ = true;


   for( int islot = 0; islot < this->nSlots_; ++islot)
   {
      if( processors_.end() != processors_.find(islot))
      {

         MM::ImageProcessor* pP = processors_[islot];
         if( NULL != pP)
         {
            try
            {
               pP->Process(pBuffer, width, height,byteDepth);
            }
            catch(...)
            {
               std::ostringstream m;
               char name[MM::MaxStrLength];
               pP->GetName(name);
               m << "Error in processor " << name;
               LogMessage(m.str().c_str(), false);
            }
         }
      }
   }

   busy_ = false;

   return ret;
}
