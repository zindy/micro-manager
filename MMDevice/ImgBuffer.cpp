/*
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

#include "ImgBuffer.h"
#include <math.h>
#include <assert.h>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// ImgBuffer class
//
ImgBuffer::ImgBuffer(unsigned xSize, unsigned ySize, unsigned pixDepth) :
   pixels_(0), width_(xSize), height_(ySize), pixDepth_(pixDepth)
{
   pixels_ = new unsigned char[xSize * ySize * pixDepth];
   assert(pixels_);
   memset(pixels_, 0, xSize * ySize * pixDepth);
}

ImgBuffer::ImgBuffer() :
   pixels_(0),
   width_(0),
   height_(0),
   pixDepth_(0)
{
}

ImgBuffer::ImgBuffer(const ImgBuffer& right)                
{
   pixels_ = 0;
   *this = right;
}

ImgBuffer::~ImgBuffer()
{
   delete[] pixels_;
}

const unsigned char* ImgBuffer::GetPixels() const
{
   return pixels_;
}

unsigned char* ImgBuffer::GetPixelsRW()
{
   return pixels_;
}

void ImgBuffer::SetPixels(const void* pix)
{
   memcpy((void*)pixels_, pix, width_ * height_ * pixDepth_);
}

// Set pixels, from a source that has extra bytes at the end of each scanline
// (row).
void ImgBuffer::SetPixelsPadded(const void* pixArray, int paddingBytesPerLine)
{
   const char* src = reinterpret_cast<const char*>(pixArray);
   char* dst = reinterpret_cast<char*>(pixels_);
   const size_t lineSize = width_ * pixDepth_;

   for(size_t i = 0; i < height_; i++)
   {
      memcpy(dst, src, lineSize);
      src += lineSize + paddingBytesPerLine;
      dst += lineSize;
   }
}

void ImgBuffer::ResetPixels()
{
   if (pixels_)
      memset(pixels_, 0, width_ * height_ * pixDepth_);
}

bool ImgBuffer::Compatible(const ImgBuffer& img) const
{
   if (  Height() != img.Height() ||
         Width() != img.Width() ||
         Depth() != img.Depth())
         return false;
   
   return true;
}

void ImgBuffer::Resize(unsigned xSize, unsigned ySize, unsigned pixDepth)
{
   // re-allocate internal buffer if it is not big enough
   if (width_ * height_ * pixDepth_ < xSize * ySize * pixDepth)
   {
      delete[] pixels_;
      pixels_ = new unsigned char [xSize * ySize * pixDepth];
      assert(pixels_);
   }

   width_ = xSize;
   height_ = ySize;
   pixDepth_ = pixDepth;
}

void ImgBuffer::Resize(unsigned xSize, unsigned ySize)
{
   // re-allocate internal buffer if it is not big enough
   if (width_ * height_ < xSize * ySize)
   {
      delete[] pixels_;
      pixels_ = new unsigned char[xSize * ySize * pixDepth_];
   }

   width_ = xSize;
   height_ = ySize;

   memset(pixels_, 0, width_ * height_ * pixDepth_);
}

void ImgBuffer::Copy(const ImgBuffer& right)
{
   if (!Compatible(right))
      Resize(right.width_, right.height_, right.pixDepth_);

   SetPixels((void*)right.GetPixels());
}

ImgBuffer& ImgBuffer::operator=(const ImgBuffer& img)
{
   if(this == &img)
      return *this;

   if (pixels_)
      delete[] pixels_;

   width_ = img.Width();
   height_ = img.Height();
   pixDepth_ = img.Depth();
   pixels_ = new unsigned char[width_ * height_ * pixDepth_];

   Copy(img);

   return *this;
}

void ImgBuffer::SetMetadata(const Metadata& md)
{
   //metadata_ = md;
   // Serialize/Restore instead of =operator used to avoid object new/delete
   // issues accross the DLL boundary (on Windows)
   // TODO: this is inefficient and should be revised
    metadata_.Restore(md.Serialize().c_str());
}


///////////////////////////////////////////////////////////////////////////////
// FrameBuffer class
///////////////////////////////////////////////////////////////////////////////

FrameBuffer::FrameBuffer(unsigned xSize, unsigned ySize, unsigned byteDepth)
{
   width_ = xSize;
   height_ = ySize;
   depth_ = byteDepth;
   handlePending_ = false;
   frameID_ = 0;
}

FrameBuffer::FrameBuffer()
{
   width_ = 0;
   height_ = 0;
   depth_ = 0;
   handlePending_ = false;
   frameID_ = 0;
}

FrameBuffer::~FrameBuffer()
{
   Clear();
}

void FrameBuffer::Clear()
{
   for (unsigned i=0; i<images_.size(); i++)
      delete images_[i];
   images_.clear();
   indexMap_.clear();
   handlePending_ = false;
}

void FrameBuffer::Preallocate(unsigned channels, unsigned slices)
{
   for (unsigned i=0; i<channels; i++)
      for (unsigned j=0; j<slices; j++)
      {
         ImgBuffer* img = FindImage(i, j);
         if (!img)
            InsertNewImage(i, j);
      }
}

void FrameBuffer::Resize(unsigned xSize, unsigned ySize, unsigned byteDepth)
{
   Clear();
   width_ = xSize;
   height_ = ySize;
   depth_ = byteDepth;
}

bool FrameBuffer::SetImage(unsigned channel, unsigned slice, const ImgBuffer& imgBuf)
{
   handlePending_ = false;
   ImgBuffer* img = FindImage(channel, slice);

   if (img)
   {
      // image already exists
      *img = imgBuf;
   }
   else
   {
      // create a new buffer
      ImgBuffer* img = InsertNewImage(channel, slice);
      *img = imgBuf;
   }

   return true;
}

bool FrameBuffer::GetImage(unsigned channel, unsigned slice, ImgBuffer& img) const
{
   ImgBuffer* imgBuf = FindImage(channel, slice);

   if (imgBuf)
   {
      img = *imgBuf;
      return true;
   }
   else
      return false;
}

bool FrameBuffer::SetPixels(unsigned channel, unsigned slice, const unsigned char* pixels)
{
   handlePending_ = false;
   ImgBuffer* img = FindImage(channel, slice);

   if (img)
   {
      // image already exists
      img->SetPixels(pixels);
   }
   else
   {
      // create a new buffer
      ImgBuffer* img = InsertNewImage(channel, slice);
      img->SetPixels(pixels);
   }

   return true;
}

const unsigned char* FrameBuffer::GetPixels(unsigned channel, unsigned slice) const
{
   ImgBuffer* img = FindImage(channel, slice);
   if (img)
      return img->GetPixels();
   else
      return 0;
}

ImgBuffer* FrameBuffer::FindImage(unsigned channel, unsigned slice) const
{
   map<unsigned long, ImgBuffer*>::const_iterator it = indexMap_.find(GetIndex(channel, slice));
   if (it != indexMap_.end())
      return it->second;
   else
      return 0;
}

unsigned long FrameBuffer::GetIndex(unsigned channel, unsigned slice)
{
   // set the slice in the upper and channel in the lower part
   unsigned long idx((unsigned short)slice);
   idx = idx << 16;
   idx = idx | (unsigned short) channel;
   return idx;
}

ImgBuffer* FrameBuffer::InsertNewImage(unsigned channel, unsigned slice)
{
   assert(FindImage(channel, slice) == 0);

   ImgBuffer* img = new ImgBuffer(width_, height_, depth_);
   images_.push_back(img);
   indexMap_[GetIndex(channel, slice)] = img;
   return img;
}
