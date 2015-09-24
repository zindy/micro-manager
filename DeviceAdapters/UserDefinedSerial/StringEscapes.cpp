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

#include "StringEscapes.h"

#include "UserDefinedSerialConstants.h"

#include "MMDeviceConstants.h"

#include <boost/format.hpp>

#include <string>
#include <vector>


// Change type without changing bits
inline char UnsignedToSignedByte(unsigned char ch)
{ return *reinterpret_cast<char*>(&ch); }


// Change type without changing bits
inline unsigned char SignedToUnsignedByte(char ch)
{ return *reinterpret_cast<unsigned char*>(&ch); }


std::string
EscapedStringFromByteString(const std::vector<char>& bytes)
{
   std::string result;
   result.reserve(4 * bytes.size());

   for (std::vector<char>::const_iterator
         it = bytes.begin(), end = bytes.end(); it != end; ++it)
   {
      if (*it >= 0x20 && *it < 0x7f && *it != '\\' &&
            std::string(MM::g_FieldDelimiters).find(*it) == std::string::npos)
      {
         result.push_back(*it);
      }
      else
      {
         switch (*it)
         {
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            default:
               {
                  // First convert to unsigned char to prevent sign extension
                  // upon converting to unsigned int.
                  unsigned char byte = SignedToUnsignedByte(*it);
                  result += (boost::format("\\x%02x") %
                     static_cast<unsigned int>(byte)).str();
               }
               break;
         }
      }
   }

   return result;
}


// Helper for ParseAfterBackslash().
// When called, i is index of first octal digit after backslash. When returning
// DEVICE_OK, i is index of next char to read, bytes has newly read byte
// appended.
inline int
ParseOctalEscape(const std::string& input, size_t& i,
      std::vector<char>& bytes)
{
   // Start reading up to 3 octal digits
   unsigned char byte = 0;
   for (size_t start = i; i < input.size() && i < start + 3; ++i)
   {
      char ch = input[i];
      if (ch >= '0' && ch < '8')
      {
         byte *= 8;
         byte += ch - '0';
      }
      else
      {
         break;
      }
   }
   --i; // "Unread" the non-digit or 4th char
   bytes.push_back(UnsignedToSignedByte(byte));
   return DEVICE_OK;
}


// Helper for ParseAfterBackslash().
// When called, i is index of first hex digit after backslash-x. When returning
// DEVICE_OK, i is index of next char to read, bytes has newly read byte
// appended.
inline int
ParseHexEscape(const std::string& input, size_t& i,
      std::vector<char>& bytes)
{
   // Start reading up to 2 hexadecimal digits.
   // In the C specification, there is no limit to the number of
   // digits in a hexadecimal escape sequence; it is terminated at the
   // first non-hex-digit. We differ from this in that we only look
   // for up to 2 digits. (Otherwise there is no way to type hex
   // escapes into a text field.)
   unsigned char byte = 0;
   size_t start = i;
   for ( ; i < input.size() && i < start + 2; ++i)
   {
      char ch = input[i];
      if (ch >= '0' && ch <= '9')
      {
         byte *= 16;
         byte += ch - '0';
      }
      else if (ch >= 'A' && ch <= 'F')
      {
         byte *= 16;
         byte += ch - 'A' + 10;
      }
      else if (ch >= 'a' && ch <= 'f')
      {
         byte *= 16;
         byte += ch - 'a' + 10;
      }
      else
      {
         break;
      }
   }
   --i; // "Unread" the non-digit or 3rd char
   if (i < start)
      return ERR_EMPTY_HEX_ESCAPE_SEQUENCE;
   bytes.push_back(UnsignedToSignedByte(byte));
   return DEVICE_OK;
}


// Helper for ByteStringFromEscapedString().
// When called, input is whole input string, i is index of char after
// backslash, bytes is bytes read so far. When returning DEVICE_OK, i is index
// of next char to read, bytes has newly read byte appended.
inline int
ParseAfterBackslash(const std::string& input, size_t& i,
      std::vector<char>& bytes)
{
   char ch = input[i];
   if (ch >= '0' && ch < '8')
   {
      int err = ParseOctalEscape(input, i, bytes);
      if (err != DEVICE_OK)
         return err;
   }
   else if (ch == 'x') // 'x' is intentionally case-sensitive
   {
      int err = ParseHexEscape(input, ++i, bytes);
      if (err != DEVICE_OK)
         return err;
   }
   else
   {
      switch (ch)
      {
         case '\'': bytes.push_back('\''); break;
         case '\"': bytes.push_back('\"'); break;
         case '?': bytes.push_back('?'); break;
         case '\\': bytes.push_back('\\'); break;
         case 'a': bytes.push_back('\a'); break;
         case 'b': bytes.push_back('\b'); break;
         case 'f': bytes.push_back('\f'); break;
         case 'n': bytes.push_back('\n'); break;
         case 'r': bytes.push_back('\r'); break;
         case 't': bytes.push_back('\t'); break;
         case 'v': bytes.push_back('\v'); break;
         default: return ERR_UNKNOWN_ESCAPE_SEQUENCE;
      }
   }
   return DEVICE_OK;
}


int
ByteStringFromEscapedString(const std::string& input,
      std::vector<char>& bytes)
{
   bytes.reserve(bytes.size() + input.size());

   bool seenBackslash = false;
   for (size_t i = 0; i < input.size(); ++i)
   {
      if (seenBackslash)
      {
         int err = ParseAfterBackslash(input, i, bytes);
         if (err != DEVICE_OK)
            return err;
         seenBackslash = false;
      }
      else
      {
         char ch = input[i];
         if (ch == '\\')
         {
            seenBackslash = true;
         }
         else
         {
            bytes.push_back(ch);
         }
      }
   }
   if (seenBackslash)
   {
      return ERR_TRAILING_BACKSLASH;
   }

   return DEVICE_OK;
}
