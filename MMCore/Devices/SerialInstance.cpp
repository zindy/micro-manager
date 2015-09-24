/*
 * Serial device instance wrapper
 *
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

#include "SerialInstance.h"


MM::PortType SerialInstance::GetPortType() const { return GetImpl()->GetPortType(); }
int SerialInstance::SetCommand(const char* command, const char* term) { return GetImpl()->SetCommand(command, term); }
int SerialInstance::GetAnswer(char* txt, unsigned maxChars, const char* term) { return GetImpl()->GetAnswer(txt, maxChars, term); }
int SerialInstance::Write(const unsigned char* buf, unsigned long bufLen) { return GetImpl()->Write(buf, bufLen); }
int SerialInstance::Read(unsigned char* buf, unsigned long bufLen, unsigned long& charsRead) { return GetImpl()->Read(buf, bufLen, charsRead); }
int SerialInstance::Purge() { return GetImpl()->Purge(); }
