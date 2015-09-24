/*
 * Generic implementation of the circular buffer
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, 01/05/2007
 *
 * Copyright (c) 2007 Regents of the University of California,
 * Copyright (c) 2008 100X Imaging Inc
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

#if !defined(_CIRCULAR_BUFFER_)
#define _CIRCULAR_BUFFER_

#include <vector>
#include "../MMDevice/ImgBuffer.h"
#include "../MMDevice/MMDevice.h"
#include "ErrorCodes.h"
#include "Error.h"

#ifdef WIN32
#pragma warning( disable : 4290 ) // exception declaration warning
#endif

///////////////////////////////////////////////////////////////////////////////
//
// CircularBuffer class
// ~~~~~~~~~~~~~~~~~~~~

static MMThreadLock g_bufferLock;
static MMThreadLock g_insertLock;

class CircularBuffer
{
public:
   CircularBuffer(unsigned int memorySizeMB);
   ~CircularBuffer();

   unsigned GetMemorySizeMB() const { return memorySizeMB_; }

   bool Initialize(unsigned channels, unsigned slices, unsigned int xSize, unsigned int ySize, unsigned int pixDepth);
   unsigned long GetSize() const;
   unsigned long GetFreeSize() const;
   unsigned long GetRemainingImageCount() const;

   unsigned int Width() const {MMThreadGuard guard(g_bufferLock); return width_;}
   unsigned int Height() const {MMThreadGuard guard(g_bufferLock); return height_;}
   unsigned int Depth() const {MMThreadGuard guard(g_bufferLock); return pixDepth_;}

   bool InsertImage(const unsigned char* pixArray, unsigned int width, unsigned int height, unsigned int byteDepth, const Metadata* pMd) throw (CMMError);
   bool InsertMultiChannel(const unsigned char* pixArray, unsigned int numChannels, unsigned int width, unsigned int height, unsigned int byteDepth, const Metadata* pMd) throw (CMMError);
   const unsigned char* GetTopImage() const;
   const unsigned char* GetNextImage();
   const ImgBuffer* GetTopImageBuffer(unsigned channel, unsigned slice) const;
   const ImgBuffer* GetNthFromTopImageBuffer(unsigned long n) const;
   const ImgBuffer* GetNextImageBuffer(unsigned channel, unsigned slice);
   void Clear() {MMThreadGuard guard(g_bufferLock); insertIndex_=0; saveIndex_=0; overflow_ = false;}

   bool Overflow() {MMThreadGuard guard(g_bufferLock); return overflow_;}

private:
   unsigned int width_;
   unsigned int height_;
   unsigned int pixDepth_;
   long imageCounter_;
   std::map<std::string, long> imageNumbers_;
   long insertIndex_;
   long saveIndex_;
   unsigned long memorySizeMB_;
   unsigned int numChannels_;
   unsigned int numSlices_;
   bool overflow_;
   std::vector<FrameBuffer> frameArray_;

   unsigned long GetClockTicksMs() const;

};


#endif // !defined(_CIRCULAR_BUFFER_)
