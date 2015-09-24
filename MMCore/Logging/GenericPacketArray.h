/*
 * AUTHOR:
 * Mark Tsuchida
 *
 * Copyright (c) 2014 Regents of the University of California,
 * All Rights reserved
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

#include "GenericLinePacket.h"

#include <boost/container/vector.hpp>

#include <algorithm>
#include <cstddef>
#include <iterator>

namespace mm
{
namespace logging
{
namespace internal
{


template <typename TMetadata>
class GenericPacketArray
{
public:
   typedef GenericLinePacket<TMetadata> LinePacketType;
   typedef typename boost::container::vector<LinePacketType>::iterator
      IteratorType;
   typedef typename boost::container::vector<LinePacketType>::const_iterator
      ConstIteratorType;

private:
   // Use boost's vector, which supports C++11-style emplace_back().
   boost::container::vector<LinePacketType> packets_;

public:
   template <typename TPacketIter>
   void Append(TPacketIter first, TPacketIter last)
   { std::copy(first, last, std::back_inserter(packets_)); }
   bool IsEmpty() const { return packets_.empty(); }
   void Clear() { packets_.clear(); }
   void Swap(GenericPacketArray& other) { packets_.swap(other.packets_); }
   IteratorType Begin() { return packets_.begin(); }
   IteratorType End() { return packets_.end(); }
   ConstIteratorType Begin() const { return packets_.begin(); }
   ConstIteratorType End() const { return packets_.end(); }

   void AppendEntry(typename TMetadata::LoggerDataType loggerData,
         typename TMetadata::EntryDataType entryData,
         typename TMetadata::StampDataType stampData,
         const char* entryText)
   {
      // Break up entryText into packets, either at CRLF or LF (new line), or at
      // PacketTextLen (line continuation).
      //
      // Do all that without scanning through entryText more than once, and
      // writing into the vector of packets in linear address order. (Okay, this
      // is probably overkill, but it's easy enough.)

      typedef GenericLinePacket<TMetadata> LinePacketType;

      const char* pText = entryText;
      PacketState nextState = PacketStateEntryFirstLine;
      std::size_t pastLastNonEmptyIndex = 0;
      do
      {
         packets_.emplace_back(nextState, loggerData, entryData, stampData);

         nextState = PacketStateLineContinuation;

         char* pLine = packets_.back().GetTextBuffer();
         const char* endLine = pLine + LinePacketType::PacketTextLen;
         while (*pText && pLine < endLine)
         {
            // The sequences "\r", "\r\n", and "\n" are considered newlines. If we
            // see one of those, mark the next as new line state. At which point,
            // pText will point to the next char after the newline sequence.
            if (*pText == '\r')
            {
               if (!*pText++)
                  break;
               if (*pText == '\n')
               {
                  if (!*pText++)
                     break;
                  nextState = PacketStateNewLine;
                  break;
               }
               nextState = PacketStateNewLine;
               break;
            }
            if (*pText == '\n')
            {
               if (!*pText++)
                  break;
               nextState = PacketStateNewLine;
               break;
            }

            *pLine++ = *pText++;
         }
         *pLine = '\0';
         if (pLine > packets_.back().GetTextBuffer())
         {
            pastLastNonEmptyIndex = packets_.size();
         }
      } while (*pText);

      // Remove trailing empty lines (but keep at least one line).
      if (pastLastNonEmptyIndex == 0)
         pastLastNonEmptyIndex++;
      packets_.erase(packets_.begin() + pastLastNonEmptyIndex, packets_.end());
   }
};


} // namespace internal
} // namespace logging
} // namespace mm
