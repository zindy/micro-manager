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


class ParameterizedEscapeTest : public ::testing::Test,
   public ::testing::WithParamInterface< std::pair<std::vector<char>,
   const char*> >
{
protected:
   std::vector<char> input_;
   std::string expectedOutput_;
   virtual void SetUp()
   {
      input_ = GetParam().first;
      expectedOutput_ = GetParam().second;
   }
};

TEST_P(ParameterizedEscapeTest, MatchWithExpected)
{
   ASSERT_EQ(expectedOutput_, EscapedStringFromByteString(input_));
}

inline std::vector<char> NullFollowedByX()
{
   std::vector<char> ret(2, '\0');
   ret[1] = 'X';
   return ret;
}

INSTANTIATE_TEST_CASE_P(BasicTestCase, ParameterizedEscapeTest,
   ::testing::Values(
      // Trivial
      std::make_pair(std::vector<char>(), ""),
      std::make_pair(std::vector<char>(1, 'x'), "x"),
      std::make_pair(std::vector<char>(2, 'x'), "xx"),
      std::make_pair(std::vector<char>(3, 'x'), "xxx"),
      // Comma (not allowed in property values)
      std::make_pair(std::vector<char>(1, ','), "\\x2c"),
      // Backslash
      std::make_pair(std::vector<char>(1, '\\'), "\\\\"),
      std::make_pair(std::vector<char>(2, '\\'), "\\\\\\\\"),
      // Single-character escapes
      std::make_pair(std::vector<char>(1, '\n'), "\\n"),
      std::make_pair(std::vector<char>(2, '\n'), "\\n\\n"),
      // Null char
      std::make_pair(std::vector<char>(1, '\0'), "\\x00"),
      std::make_pair(std::vector<char>(2, '\0'), "\\x00\\x00"),
      std::make_pair(NullFollowedByX(), "\\x00X"),
      // Non-null control char
      std::make_pair(std::vector<char>(1, '\1'), "\\x01"),
      std::make_pair(std::vector<char>(2, '\1'), "\\x01\\x01"),
      // Edge cases
      std::make_pair(std::vector<char>(1, '~'), "~"),
      std::make_pair(std::vector<char>(1, 127), "\\x7f"),
      std::make_pair(std::vector<char>(1, ' '), " "),
      std::make_pair(std::vector<char>(1, '\x1f'), "\\x1f"),
      // High range
      std::make_pair(std::vector<char>(1, '\x80'), "\\x80"),
      std::make_pair(std::vector<char>(1, '\xff'), "\\xff"),
      // Regression (codes between decimal 20 and hex 20)
      std::make_pair(std::vector<char>(1, '\x13'), "\\x13"),
      std::make_pair(std::vector<char>(1, '\x14'), "\\x14"),
      std::make_pair(std::vector<char>(1, '\x1e'), "\\x1e")
   ));


int main(int argc, char **argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
