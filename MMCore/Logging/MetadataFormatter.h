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

#include <boost/date_time/posix_time/posix_time.hpp>

#include <cstring>
#include <ostream>
#include <string>


namespace mm
{
namespace logging
{
namespace internal
{


inline const char*
LevelString(LogLevel logLevel)
{
   switch (logLevel)
   {
      case LogLevelTrace: return "trc";
      case LogLevelDebug: return "dbg";
      case LogLevelInfo: return "IFO";
      case LogLevelWarning: return "WRN";
      case LogLevelError: return "ERR";
      case LogLevelFatal: return "FTL";
      default: return "???";
   }
}


// A stateful formatter for the metadata prefix and corresponding
// continuation-line prefix. Intended for single-threaded use only.
class MetadataFormatter
{
   // Reuse buffers for efficiency
   std::string buf_;
   std::ostringstream sstrm_;
   size_t openBracketCol_;
   size_t closeBracketCol_;

public:
   MetadataFormatter() : openBracketCol_(0), closeBracketCol_(0) {}

   // Format the line prefix for the first line of an entry
   void FormatLinePrefix(std::ostream& stream, const Metadata& metadata);

   // Format the line prefix for subsequent lines of an entry
   void FormatContinuationPrefix(std::ostream& stream);
};


inline void
MetadataFormatter::FormatLinePrefix(std::ostream& stream,
      const Metadata& metadata)
{
   // Pre-forming string is more efficient than writing bit by bit to stream.

   buf_ = boost::posix_time::to_iso_extended_string(
         metadata.GetStampData().GetTimestamp());
   buf_ += " tid";
   sstrm_.str(std::string());
   sstrm_ << metadata.GetStampData().GetThreadId();
   buf_ += sstrm_.str();
   buf_ += ' ';

   openBracketCol_ = buf_.size();
   buf_ += '[';

   buf_ += LevelString(metadata.GetEntryData().GetLevel());
   buf_ += ',';
   buf_ += metadata.GetLoggerData().GetComponentLabel();

   closeBracketCol_ = buf_.size();
   buf_ += ']';

   stream << buf_;
}


inline void
MetadataFormatter::FormatContinuationPrefix(std::ostream& stream)
{
   buf_.assign(closeBracketCol_ + 1, ' ');
   buf_[openBracketCol_] = '[';
   buf_[closeBracketCol_] = ']';
   stream << buf_;
}


} // namespace internal
} // namespace logging
} // namespace mm
