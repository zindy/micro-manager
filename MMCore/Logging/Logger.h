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

#include "GenericLogger.h"
#include "Metadata.h"


namespace mm
{
namespace logging
{

typedef internal::GenericLogger<EntryData> Logger;
typedef internal::GenericLogStream<Logger> LogStream;

} // namespace logging
} // namespace mm


// Shorthands for LogStream
//
// Usage:
//
//     LOG_INFO(myLogger) << x << y << z;

// You might think that we don't need the following macros, because we could
// just write
//
//     LogStream(myLogger, someLevel) << x << y << z;
//
// However, that would only work with C++11, where the standard operator<<()
// implementations include overloads for rvalue references (basic_ostream&&).
// In C++ pre-11, the above statement will fail for some data types of x (e.g.
// const char*). So, to make the left hand side of << an lvalue, we need to use
// a trick.

#define LOG_WITH_LEVEL(logger, level) \
   for (::mm::logging::LogStream strm((logger), (level)); \
         !strm.Used(); strm.MarkUsed()) \
      strm

#define LOG_TRACE(logger) LOG_WITH_LEVEL((logger), ::mm::logging::LogLevelTrace)
#define LOG_DEBUG(logger) LOG_WITH_LEVEL((logger), ::mm::logging::LogLevelDebug)
#define LOG_INFO(logger) LOG_WITH_LEVEL((logger), ::mm::logging::LogLevelInfo)
#define LOG_WARNING(logger) LOG_WITH_LEVEL((logger), ::mm::logging::LogLevelWarning)
#define LOG_ERROR(logger) LOG_WITH_LEVEL((logger), ::mm::logging::LogLevelError)
#define LOG_FATAL(logger) LOG_WITH_LEVEL((logger), ::mm::logging::LogLevelFatal)
