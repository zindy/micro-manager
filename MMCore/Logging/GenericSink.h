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

#include "GenericEntryFilter.h"
#include "GenericLinePacket.h"
#include "GenericPacketArray.h"

#include <boost/container/vector.hpp>
#include <boost/shared_ptr.hpp>


namespace mm
{
namespace logging
{
namespace internal
{


template <class TMetadata>
class GenericSink
{
private:
   boost::shared_ptr< GenericEntryFilter<TMetadata> > filter_;

protected:
   boost::shared_ptr< GenericEntryFilter<TMetadata> > GetFilter() const
   { return filter_; }

public:
   typedef GenericPacketArray<TMetadata> PacketArrayType;

   virtual ~GenericSink() {}
   virtual void Consume(const PacketArrayType& packets) = 0;

   // Note: If setting the filter while the sink is in use, you must pause the
   // logger. See the LoggingCore member function AtomicSetSinkFilters().
   void SetFilter(boost::shared_ptr< GenericEntryFilter<TMetadata> > filter)
   { filter_ = filter; }
};


} // namespace internal
} // namespace logging
} // namespace mm
