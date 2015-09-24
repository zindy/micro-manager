/*
 * Mock device adapter for testing of device sequencing
 *
 * Copyright (c) 2014 Regents of the University of California
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

#pragma once

#include <cstddef>
#include <stdint.h>
#include <string>


class TextImageCursor
{
   uint8_t* buffer_;

   int stride_;
   int nRows_;

   int baseline_;
   int hPos_;

public:
   static const int GLYPH_HEIGHT = 5;
   static const int BASELINE_SKIP = GLYPH_HEIGHT + 4;
   static const int GLYPH_SPACING = 1;
   static const int MARGIN = 4;

public:
   TextImageCursor(uint8_t* buffer, int bufferWidth, int bufferHeight) :
      buffer_(buffer),
      stride_(bufferWidth),
      nRows_(bufferHeight),
      baseline_(MARGIN + GLYPH_HEIGHT),
      hPos_(MARGIN)
   {}

   uint8_t* GetBuffer() { return buffer_; }

   bool IsBeyondBuffer() const { return baseline_ > nRows_; }
   void NewLine() { baseline_ += BASELINE_SKIP; hPos_ = MARGIN; }
   void Space() { if (HasRoom(5)) Advance(5); else NewLine(); }
   bool HasRoom(int width) const
   {
      if (IsBeyondBuffer() || width > stride_ - 2 * MARGIN)
         return false;
      if (stride_ - hPos_ >= width + MARGIN)
         return true;
      return false;
   }
   bool MakeRoom(int width)
   {
      if (IsBeyondBuffer() || width > stride_ - 2 * MARGIN)
         return false;
      if (stride_ - hPos_ >= width + MARGIN)
         return true;
      NewLine();
      return !IsBeyondBuffer();
   }
   int GetBaselineIndex() const { return baseline_ * stride_ + hPos_; }
   int GetNorthStep() const { return -stride_; }
   int GetEastStep() const { return 1; }
   int GetWestStep() const { return -1; }
   int GetSouthStep() const { return stride_; }
   void Advance(int hDelta)
   {
      hPos_ += hDelta;
      if (hPos_ + MARGIN > stride_)
         NewLine();
   }
};


void DrawStringOnImage(TextImageCursor& cursor, const std::string& string,
      bool allowLineBreak = false);

// Draw a whole text image (no word wrapping)
void DrawTextImage(uint8_t* buffer, size_t width, size_t height,
      const std::string& text);
