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

#ifndef _IMAGEPROCESSORCHAIN_H_
#define _IMAGEPROCESSORCHAIN_H_

#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ImgBuffer.h"
#include "../../MMDevice/DeviceThreads.h"
#include <string>
#include <map>



//////////////////////////////////////////////////////////////////////////////
// ImageProcessorChain class
// run chain of image processors
//////////////////////////////////////////////////////////////////////////////
class ImageProcessorChain : public CImageProcessorBase<ImageProcessorChain>
{
public:
   ImageProcessorChain () : nSlots_(10), busy_(false) {}
   ~ImageProcessorChain () { }

   int Shutdown() {return DEVICE_OK;}
   void GetName(char* name) const {strcpy(name,"ImageProcessorChain");}

   int Initialize();

   bool Busy(void) { return busy_;};

   int Process(unsigned char* buffer, unsigned width, unsigned height, unsigned byteDepth);

   // action interface
   // ----------------
   int OnProcessor(MM::PropertyBase* pProp, MM::ActionType eAct, long indexx);

private:
   const int nSlots_;
   bool busy_;
   std::map< int, std::string> processorNames_;
   std::map< int, MM::ImageProcessor*> processors_;

   ImageProcessorChain& operator=( const ImageProcessorChain& ){ 
      return *this;
   };

   
};




#endif //_IMAGEPROCESSORCHAIN_H_
