/*
 * Optic Valley Photonics Environmental Conditioning System Gen2
 *
 * AUTHOR:
 * Jon Daniels (jon@asiimaging.com) 06/2014
 *
 * Copyright (c)  Applied Scientific Instrumentation, Eugene OR
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

#include "OVP_ECS2.h"

// The GetCRCChecksum function is placed in a separate translation unit,
// because the Boost CRC templates generate VC++ warnings that cannot be
// suppressed with a local pragma warning push/pop.
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#pragma warning (disable: 4245)
#endif

#include <boost/crc.hpp>
#include <boost/integer.hpp>

Message ECS::GetCRCChecksum(Message msg)
// returns a 2-element vector (16 bits)
// relies on the header-only Boost CRC library (boost/crc.hpp)
// when I compile I see 3 warnings related to type conversions
{
   Message crc_vector;
   boost::uint16_t crc_uint;
   boost::crc_optimal<16, 0x8005, 0xFFFF, 0, true, true> crc_modbus;
   crc_modbus.process_bytes(&msg[0], msg.size());
   crc_uint = crc_modbus.checksum();
   crc_vector.push_back(crc_uint & 0xFF);
   crc_vector.push_back((crc_uint & 0xFF00) >> 8);
   return crc_vector;
}
