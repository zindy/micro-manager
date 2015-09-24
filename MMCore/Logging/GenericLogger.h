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

#include <boost/function.hpp>
#include <boost/utility.hpp>

#include <sstream>
#include <string>


namespace mm
{
namespace logging
{
namespace internal
{


template <typename TEntryData>
class GenericLogger
{
   boost::function<void (TEntryData, const char*)> impl_;

public:
   typedef TEntryData EntryDataType;

   GenericLogger(boost::function<void (TEntryData, const char*)> f) :
      impl_(f)
   {}

   void operator()(TEntryData entryData, const char* message) const
   { impl_(entryData, message); }

   void operator()(TEntryData entryData, const std::string& message) const
   { impl_(entryData, message.c_str()); }
};


/**
 * Log an entry upon destruction.
 */
template <class TLogger>
class GenericLogStream : public std::ostringstream, boost::noncopyable
{
public:
   typedef typename TLogger::EntryDataType EntryDataType;

   const TLogger& logger_;
   EntryDataType level_;
   bool used_;

public:
   GenericLogStream(const TLogger& logger, EntryDataType level) :
      logger_(logger),
      level_(level),
      used_(false)
   {}

   // Supporting functions for the LOG_* macros. See the macro definitions.
   bool Used() const { return used_; }
   void MarkUsed() { used_ = true; }

   virtual ~GenericLogStream()
   {
      logger_(level_, str());
   }
};

} // namespace internal
} // namespace logging
} // namespace mm
