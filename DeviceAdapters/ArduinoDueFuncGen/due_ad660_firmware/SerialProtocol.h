/*
 * Serial protocol constants for Arduino Duo + AD660 function generator
 *
 * AUTHOR:
 * Mark Tsuchida
 *
 * Copyright (c) 2013-2014 Regents of the University of California
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

// This file is intended to be included both by the firmware source and the
// device adapter source.

// Error codes
enum {
   DFGERR_OK,
   DFGERR_BAD_ALLOC,
   DFGERR_BAD_CMD,
   DFGERR_BAD_PARAMS,
   DFGERR_TOO_FEW_PARAMS,
   DFGERR_TOO_MANY_PARAMS,
   DFGERR_CMD_TOO_LONG,
   DFGERR_EXPECTED_UINT,
   DFGERR_FREQ_OUT_OF_RANGE,
   DFGERR_INVALID_BANK,
   DFGERR_INVALID_CHANNEL,
   DFGERR_INVALID_PHASE_OFFSET,
   DFGERR_INVALID_SAMPLE_VALUE,
   DFGERR_INVALID_TABLE,
   DFGERR_OVERFLOW,
   DFGERR_TIMEOUT,
   DFGERR_NO_WAVEFORM,
   DFGERR_BUSY,
   END_DFGERR
};

const unsigned DFG_SERIAL_MAGIC_ID = 18569;
