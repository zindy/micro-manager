/*
 * Control devices using user-specified serial commands
 *
 * AUTHOR:
 * Mark Tsuchida
 *
 * Copyright (c) 2014 Regents of the University of California
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

#pragma once

#include <string>
#include <vector>


/**
 * \brief Convert bytes to C-style escaped string.
 *
 * The format mostly conforms to C/C++ string escape sequences, with one
 * exception: it is assumed that hex escapes (backslash followed by x followed
 * by hex digits) are limited to two hex digits (in C strings, consecuitive hex
 * digits become part of the escape sequence until a non-hex-ditig is
 * encountered).
 *
 * For code points that have human-friendly escape sequences (newlines, tabs,
 * etc.), only CR and LF are converted to that form; all other control
 * characters are converted to two-digit hex escapes.
 *
 * As a special exception, the comma is converted to a hex escape (backslash-
 * x2c), so as not to violate Micro-Manager's rules for property values.
 *
 * Hex escapes are emitted in lower case.
 */
std::string EscapedStringFromByteString(const std::vector<char>& bytes);

/**
 * \brief Convert C-style escaped string to bytes.
 *
 * Most C/C++ string escape sequences are accepted according to the spec, with
 * two differences: Unicode escapes (backslash followed by U or u) are not
 * allowed, and hex escapes (backslash followed by x followed by hex digits)
 * are limited to 2 hex digits.
 *
 * Standard C/C++ string hex escapes specify that hex digits are read until a
 * non-hex-digit is encountered. This implementation, in contrast, concludes
 * the hex escape after at most two digits, even if the next character is a
 * valid hex digit. This decision was made so that arbitrary byte values could
 * be represented in a GUI text field, without having to escape all subsequent
 * characters.
 *
 * \return error code if an unknown or incomplete escape sequence is
 * encountered.
 */
int ByteStringFromEscapedString(const std::string& escaped,
      std::vector<char>& bytes);
