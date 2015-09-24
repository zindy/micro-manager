/*
 * Basic implementation for the accumultor data structure (frame averaging).
 *
 * AUTHOR:
 * Nenad Amodaj
 *
 * Copyright (c) 2011 Nenad Amodaj
 * Copyright (c) 2009 100X Imaging Inc
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

#include "ImgAccumulator.h"
#include <math.h>
#include <assert.h>
#include <algorithm>
#include <iostream>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// ImgAccumulator class
// byte depth of 2 does summation

ImgAccumulator::ImgAccumulator() :
pixels_(0), width_(0), height_(0), pixDepth_(0), length_(1) {
   frameIndex_ = 0;
}

ImgAccumulator::~ImgAccumulator()
{
   delete[] pixels_;
}

const unsigned char* ImgAccumulator::GetPixels() const
{
   return pixels_;
}

void ImgAccumulator::AddPixels(const void* pix, unsigned sourceWidth, unsigned, unsigned offsetY)
{
	//pixels coming in will always be 8 bit
	const unsigned char* pixPtr = static_cast<const unsigned char*>(pix);

	for (unsigned i=0; i<height_; i++) {
		for (unsigned j=0; j<width_; j++) {
			int srcIdx = (offsetY+i)*sourceWidth+j;
			int idx = i*width_+j;
			accumulator_[idx] += pixPtr[srcIdx];
		}
	}
	frameIndex_++;
}

void ImgAccumulator::ResetPixels()
{
	// reset pixel buffer
	if (pixels_)
		memset(pixels_, 0, width_ * height_ * pixDepth_);

	// reset accumulator
	accumulator_.clear();
	accumulator_.assign(width_ * height_, 0);

	frameIndex_ = 0;
}

void ImgAccumulator::Resize(unsigned xSize, unsigned ySize, unsigned pixDepth)
{
   // re-allocate internal buffer if it is not big enough
   if (width_ * height_ * pixDepth_ < xSize * ySize * pixDepth)
   {
      delete[] pixels_;
      pixels_ = new unsigned char [xSize * ySize * pixDepth];
   }
   
   width_ = xSize;
   height_ = ySize;
   pixDepth_ = pixDepth;

   // initialize content
   memset(pixels_, 0, width_ * height_ * pixDepth_);
   SetupAccumulator();
   frameIndex_ = 0;
}

void ImgAccumulator::Resize(unsigned xSize, unsigned ySize)
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
   accumulator_.resize(width_ * height_, 0.0);
   frameIndex_ = 0;
}

void ImgAccumulator::SetLength(unsigned length)
{
   length_ = length;
   SetupAccumulator();
}

void ImgAccumulator::SetupAccumulator() 
{
	accumulator_.resize(width_ * height_, 0);
}

void ImgAccumulator::CalculateOutputImage()
{
	//Do frame averaging
	long size = width_ * height_;

	if (pixDepth_ == 1) {
		for (long i=0; i<size; i++) {
			//divide by number of frames
			accumulator_[i] *= (1.0/length_);
			pixels_[i] = (unsigned char) min(accumulator_[i], (double)UCHAR_MAX);
		}
	} else {
		//reinterperet as two bytes and write to pixels
		unsigned short* bufPtr = reinterpret_cast<unsigned short*>(pixels_);
		for (long i=0; i<size; i++)	{
			bufPtr[i] = (unsigned short) min(accumulator_[i], (double)USHRT_MAX);
		}
	}

}

