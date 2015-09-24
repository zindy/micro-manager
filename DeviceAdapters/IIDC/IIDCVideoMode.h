/*
 * AUTHOR:
 * Mark A. Tsuchida
 *
 * Copyright (c) 2014 Regents of the University of California
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

#pragma once

#include "IIDCInterface.h"

#include <dc1394/dc1394.h>
#ifdef _MSC_VER
#undef restrict
#endif

#include <string>


namespace IIDC {

PixelFormat PixelFormatForLibDC1394ColorCoding(dc1394color_coding_t coding);


class VideoMode
{
   dc1394video_mode_t libdc1394mode_;
   dc1394color_coding_t libdc1394coding_;
   unsigned width_;
   unsigned height_;

protected:
   VideoMode(dc1394video_mode_t libdc1394mode) :
      libdc1394mode_(libdc1394mode)
   {}

   void SetImageSize(unsigned width, unsigned height)
   { width_ = width; height_ = height; }
   void SetLibDC1394Coding(dc1394color_coding_t coding)
   { libdc1394coding_ = coding; }

   virtual std::string GetColorCodingName() const;

public:
   virtual ~VideoMode() {}

   bool operator==(const VideoMode& rhs)
   { return (libdc1394mode_ == rhs.libdc1394mode_) && (libdc1394coding_ == rhs.libdc1394coding_); }
   bool operator!=(const VideoMode& rhs) { return !operator==(rhs); }

   virtual dc1394video_mode_t GetLibDC1394Mode() const { return libdc1394mode_; }
   virtual dc1394color_coding_t GetLibDC1394Coding() const { return libdc1394coding_; }

   virtual unsigned GetMaxWidth() const { return width_; }
   virtual unsigned GetMaxHeight() const { return height_; }
   virtual PixelFormat GetPixelFormat() const;
   virtual bool IsFormat7() const = 0;
   virtual bool IsSupported() const { return GetPixelFormat() != PixelFormatUnsupported; }
   virtual std::string ToString() const = 0;
};


class ConventionalVideoMode : public VideoMode
{
public:
   ConventionalVideoMode(dc1394camera_t* libdc1394camera, dc1394video_mode_t libdc1394mode);
   virtual bool IsFormat7() const { return false; }
   virtual std::string ToString() const;
};


class Format7VideoMode : public VideoMode
{
public:
   Format7VideoMode(dc1394camera_t* libdc1394camera,
         dc1394video_mode_t libdc1394mode, dc1394color_coding_t libdc1394coding);
   virtual bool IsFormat7() const { return true; }
   virtual std::string ToString() const;
};

} // namespace IIDC
