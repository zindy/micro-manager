/*
 * Unit tests for UserDefinedSerial
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

#include <gtest/gtest.h>

#include "StringEscapes.h"
#include "UserDefinedSerialConstants.h"

#include "MMDeviceConstants.h"


// EscapedStringFromByteString() is simple, already tested, and trustworthy, so
// use it to simplify these tests (since it is a bother to code
// std::vector<char> output).
class ParameterizedUnescapeTest : public ::testing::Test,
   public ::testing::WithParamInterface< std::pair< std::string,
   std::pair<int, std::string> > >
{
protected:
   std::string input_;
   int expectedRetVal_;
   std::string expectedReescapedOutput_;
   virtual void SetUp()
   {
      input_ = GetParam().first;
      expectedRetVal_ = GetParam().second.first;
      expectedReescapedOutput_ = GetParam().second.second;
   }
};

TEST_P(ParameterizedUnescapeTest, MatchWithReescaped)
{
   std::vector<char> output;
   int err = ByteStringFromEscapedString(input_, output);
   ASSERT_EQ(expectedRetVal_, err);
   if (err == DEVICE_OK) // output is undefined on error
   {
      ASSERT_EQ(expectedReescapedOutput_,
            EscapedStringFromByteString(output));
   }
}

template <typename T, typename U, typename V>
std::pair< std::string, std::pair<int, std::string> >
mktesttriple(T t, U u, V v)
{ return std::make_pair(t, std::make_pair(u, v)); }


INSTANTIATE_TEST_CASE_P(BasicTestCase, ParameterizedUnescapeTest,
      ::testing::Values(
         mktesttriple("", DEVICE_OK, ""),
         mktesttriple("x", DEVICE_OK, "x"),
         mktesttriple("xy", DEVICE_OK, "xy"),
         mktesttriple("xyz", DEVICE_OK, "xyz"),
         mktesttriple("xx", DEVICE_OK, "xx"),
         mktesttriple("xxx", DEVICE_OK, "xxx")
      ));

INSTANTIATE_TEST_CASE_P(SingleEscapeTestCase, ParameterizedUnescapeTest,
      ::testing::Values(
         mktesttriple("\\'", DEVICE_OK, "'"),
         mktesttriple("\\\"", DEVICE_OK, "\""),
         mktesttriple("\\?", DEVICE_OK, "?"),
         mktesttriple("\\\\", DEVICE_OK, "\\\\"),
         mktesttriple("\\a", DEVICE_OK, "\\x07"),
         mktesttriple("\\b", DEVICE_OK, "\\x08"),
         mktesttriple("\\f", DEVICE_OK, "\\x0c"),
         mktesttriple("\\n", DEVICE_OK, "\\n"),
         mktesttriple("\\r", DEVICE_OK, "\\r"),
         mktesttriple("\\t", DEVICE_OK, "\\x09"),
         mktesttriple("\\v", DEVICE_OK, "\\x0b"),
         mktesttriple("\\r\\nX\\rY\\n\\r", DEVICE_OK, "\\r\\nX\\rY\\n\\r"),
         mktesttriple("Z\\r\\nX\\rY\\n\\r", DEVICE_OK, "Z\\r\\nX\\rY\\n\\r")
      ));

INSTANTIATE_TEST_CASE_P(EscapeErrorTestCase, ParameterizedUnescapeTest,
      ::testing::Values(
         // The output won't be checked if the error is correctly reported
         mktesttriple("\\", ERR_TRAILING_BACKSLASH, "NC"),
         mktesttriple("\\\\\\", ERR_TRAILING_BACKSLASH, "NC"),
         mktesttriple("\\X", ERR_UNKNOWN_ESCAPE_SEQUENCE, "NC"),
         mktesttriple("\\Z", ERR_UNKNOWN_ESCAPE_SEQUENCE, "NC"),
         mktesttriple("\\8", ERR_UNKNOWN_ESCAPE_SEQUENCE, "NC"),
         mktesttriple("\\9", ERR_UNKNOWN_ESCAPE_SEQUENCE, "NC"),
         mktesttriple("\\xZ", ERR_EMPTY_HEX_ESCAPE_SEQUENCE, "NC")
      ));

INSTANTIATE_TEST_CASE_P(OctalEscapeTestCase, ParameterizedUnescapeTest,
      ::testing::Values(
         mktesttriple("\\0", DEVICE_OK, "\\x00"),
         mktesttriple("\\0x", DEVICE_OK, "\\x00x"),
         mktesttriple("\\00", DEVICE_OK, "\\x00"),
         mktesttriple("\\00x", DEVICE_OK, "\\x00x"),
         mktesttriple("\\000", DEVICE_OK, "\\x00"),
         mktesttriple("\\000x", DEVICE_OK, "\\x00x"),
         mktesttriple("\\0000", DEVICE_OK, "\\x000"),
         mktesttriple("\\00000", DEVICE_OK, "\\x0000"),
         mktesttriple("x\\0x", DEVICE_OK, "x\\x00x"),
         mktesttriple("\\1", DEVICE_OK, "\\x01"),
         mktesttriple("\\7", DEVICE_OK, "\\x07"),
         mktesttriple("\\01", DEVICE_OK, "\\x01"),
         mktesttriple("\\10", DEVICE_OK, "\\x08"),
         mktesttriple("\\17", DEVICE_OK, "\\x0f"),
         mktesttriple("\\71", DEVICE_OK, "9"),
         mktesttriple("\\70", DEVICE_OK, "8"),
         mktesttriple("\\123", DEVICE_OK, "S"),
         mktesttriple("\\1234", DEVICE_OK, "S4")
      ));

INSTANTIATE_TEST_CASE_P(HexEscapeTestCase, ParameterizedUnescapeTest,
      ::testing::Values(
         mktesttriple("\\x0", DEVICE_OK, "\\x00"),
         mktesttriple("\\x0x", DEVICE_OK, "\\x00x"),
         mktesttriple("\\x00", DEVICE_OK, "\\x00"),
         mktesttriple("\\x00x", DEVICE_OK, "\\x00x"),
         mktesttriple("\\x000", DEVICE_OK, "\\x000"),
         mktesttriple("\\x000x", DEVICE_OK, "\\x000x"),
         mktesttriple("\\x0000", DEVICE_OK, "\\x0000"),
         mktesttriple("\\x00000", DEVICE_OK, "\\x00000"),
         mktesttriple("x\\x0x", DEVICE_OK, "x\\x00x"),

         mktesttriple("\\x7f", DEVICE_OK, "\\x7f"),
         mktesttriple("\\x7F", DEVICE_OK, "\\x7f")
      ));

INSTANTIATE_TEST_CASE_P(EightBitTestCase, ParameterizedUnescapeTest,
      ::testing::Values(
         mktesttriple("\\x80", DEVICE_OK, "\\x80"),
         mktesttriple("\\xaB", DEVICE_OK, "\\xab"),
         mktesttriple("\\xAb", DEVICE_OK, "\\xab"),
         mktesttriple("\\xff", DEVICE_OK, "\\xff")
      ));


int main(int argc, char **argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
