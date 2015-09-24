/*
 * Basic implementation for the raw image buffer data structure.
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com
 *
 * Copyright (c) 2005 Nenad Amodaj, . All rigths reserved.
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

/*
 * NOTES:
 *            Imported from ADVI for use in Micro-Manager
 */

#if !defined(_IMG_BUFFER_)
#define _IMG_BUFFER_

#include <string>
#include <vector>
#include <map>
#include "MMDevice.h"
#include "ImageMetadata.h"

///////////////////////////////////////////////////////////////////////////////
//
// CImgBuffer class
// ~~~~~~~~~~~~~~~~~~
// Variable pixel depth image buffer
//

class ImgBuffer
{
public:
   ImgBuffer(unsigned xSize, unsigned ySize, unsigned pixDepth);
   ImgBuffer(const ImgBuffer& ib);
   ImgBuffer();
   ~ImgBuffer();

   unsigned int Width() const {return width_;}
   unsigned int Height() const {return height_;}
   unsigned int Depth() const {return pixDepth_;}
   void SetPixels(const void* pixArray);
   void SetPixelsPadded(const void* pixArray, int paddingBytesPerLine);
   void ResetPixels();
   const unsigned char* GetPixels() const;
   unsigned char* GetPixelsRW();

   void Resize(unsigned xSize, unsigned ySize, unsigned pixDepth);
   void Resize(unsigned xSize, unsigned ySize);
   bool Compatible(const ImgBuffer& img) const;

   void SetName(const char* name) {name_ = name;}
   const std::string& GetName() {return name_;}
   void SetMetadata(const Metadata& md);
   const Metadata& GetMetadata() const {return metadata_;}

   void Copy(const ImgBuffer& rhs);
   ImgBuffer& operator=(const ImgBuffer& rhs);

private:
   unsigned char* pixels_;
   unsigned int width_;
   unsigned int height_;
   unsigned int pixDepth_;
   std::string name_;
   Metadata metadata_;
};

class FrameBuffer
{
public:
   FrameBuffer();
   FrameBuffer(unsigned xSize, unsigned ySize, unsigned byteDepth);
   ~FrameBuffer();

   void Resize(unsigned xSize, unsigned ySize, unsigned pixDepth);
   void Clear();
   void Preallocate(unsigned channels, unsigned slices);

   bool SetImage(unsigned channel, unsigned slice, const ImgBuffer& img);
   bool GetImage(unsigned channel, unsigned slice, ImgBuffer& img) const;
   ImgBuffer* FindImage(unsigned channel, unsigned slice) const;
   const unsigned char* GetPixels(unsigned channel, unsigned slice) const;
   bool SetPixels(unsigned channel, unsigned slice, const unsigned char* pixels);
   unsigned Width() const {return width_;}
   unsigned Height() const {return height_;}
   unsigned Depth() const {return depth_;}

   void SetID(long id)   {frameID_ = id;}
   long GetID() const      {return frameID_;}
   bool IsHandlePending() const {return handlePending_;}
   void SetHandlePending() {handlePending_ = true;}

private:
   static unsigned long GetIndex(unsigned channel, unsigned slice);
   ImgBuffer* InsertNewImage(unsigned channel, unsigned slice);

   std::vector<ImgBuffer*> images_;
   std::map<unsigned long, ImgBuffer*> indexMap_;
   long frameID_;
   bool handlePending_;
   unsigned int width_;
   unsigned int height_;
   unsigned int depth_;
};


#endif // !defined(_IMG_BUFFER_)
