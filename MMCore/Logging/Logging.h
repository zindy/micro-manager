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

#include "GenericStreamSink.h"
#include "GenericEntryFilter.h"
#include "GenericLoggingCore.h"
#include "GenericSink.h"

#include "Logger.h"
#include "Metadata.h"
#include "MetadataFormatter.h"


namespace mm
{
namespace logging
{


typedef internal::GenericLoggingCore<Metadata> LoggingCore;

typedef internal::GenericSink<Metadata> LogSink;
typedef internal::GenericStdErrLogSink<Metadata, internal::MetadataFormatter>
   StdErrLogSink;
typedef internal::GenericFileLogSink<Metadata, internal::MetadataFormatter>
   FileLogSink;


typedef internal::GenericEntryFilter<Metadata> EntryFilter;

class LevelFilter : public EntryFilter
{
   LogLevel minLevel_;

public:
   LevelFilter(LogLevel minLevel) : minLevel_(minLevel) {}

   virtual bool Filter(const Metadata& metadata) const
   { return metadata.GetEntryData().GetLevel() >= minLevel_; }
};


} // namespace logging
} // namespace mm
