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

#include "MMDevice.h"

#include <boost/utility.hpp>

#include <memory>
#include <string>
#include <vector>


/**
 * \brief Interface for serial response detection.
 *
 * This is a strategy class that provides different implementations for
 * receiving and checking responses.
 */
class ResponseDetector : boost::noncopyable
{
public:
   /**
    * \brief Create an instance for the named stragtegy.
    *
    * This version in the abstract base class calls the derived classes'
    * NewByName() functions to create the appropriate instance. If name is not
    * a known name, returns null.
    */
   static std::auto_ptr<ResponseDetector> NewByName(const std::string& name);

   virtual ~ResponseDetector() {}

   /**
    * \brief Return the method name.
    *
    * This is equal to the name passed to NewByName() to create the instance.
    */
   virtual std::string GetMethodName() const = 0;

   /**
    * \brief Receive and match to an expected response.
    * \return error code if could not receive or did not match.
    */
   virtual int RecvExpected(MM::Core* core, MM::Device* device,
         const std::string& port, const std::vector<char>& expected) = 0;

   /**
    * \brief Receive and match to one of a number of possible responses.
    * \param alternatives The possible responses to match against.
    * \param index The index of the matched alternative is returned.
    * \return error cde if could not receive or did not match.
    */
   virtual int RecvAlternative(MM::Core* core, MM::Device* device,
         const std::string& port,
         const std::vector< std::vector<char> >& alternatives,
         size_t& index) = 0;
};

/**
 * \brief Response detector that ignores all responses.
 */
class IgnoringResponseDetector : public ResponseDetector
{
public:
   static std::auto_ptr<ResponseDetector> NewByName(const std::string& name);

   virtual std::string GetMethodName() const;
   virtual int RecvExpected(MM::Core* core, MM::Device* device,
         const std::string& port, const std::vector<char>& expected);
   virtual int RecvAlternative(MM::Core* core, MM::Device* device,
         const std::string& port,
         const std::vector< std::vector<char> >& alternatives,
         size_t& index);

private:
   IgnoringResponseDetector() {}
};

/**
 * \brief Response detector for ASCII responses terminated with newlines.
 */
class TerminatorResponseDetector : public ResponseDetector
{
   std::string terminator_;
   std::string terminatorName_;

public:
   static std::auto_ptr<ResponseDetector> NewByName(const std::string& name);

   virtual std::string GetMethodName() const;
   virtual int RecvExpected(MM::Core* core, MM::Device* device,
         const std::string& port, const std::vector<char>& expected);
   virtual int RecvAlternative(MM::Core* core, MM::Device* device,
         const std::string& port,
         const std::vector< std::vector<char> >& alternatives,
         size_t& index);

private:
   TerminatorResponseDetector(const char* terminator,
         const char* terminatorName) :
      terminator_(terminator), terminatorName_(terminatorName)
   {}
   int Recv(MM::Core* core, MM::Device* device, const std::string& port,
         std::vector<char>& response);
};

/**
 * \brief Common base for binary response detectors.
 */
class BinaryResponseDetector : public ResponseDetector
{
protected:
   int Recv(MM::Core* core, MM::Device* device, const std::string& port,
         size_t recvLen, std::vector<char>& response);
};

/**
 * \brief Response detector for fixed-length binary responses.
 */
class FixedLengthResponseDetector : public BinaryResponseDetector
{
   size_t byteCount_;

public:
   static std::auto_ptr<ResponseDetector> NewByName(const std::string& name);

   virtual std::string GetMethodName() const;
   virtual int RecvExpected(MM::Core* core, MM::Device* device,
         const std::string& port, const std::vector<char>& expected);
   virtual int RecvAlternative(MM::Core* core, MM::Device* device,
         const std::string& port,
         const std::vector< std::vector<char> >& alternatives,
         size_t& index);

private:
   FixedLengthResponseDetector(size_t byteCount) : byteCount_(byteCount) {}
};

/**
 * \brief Response detector for variable-length binary responses.
 *
 * The response length is determined from the expected response(s).
 */
class VariableLengthResponseDetector : public BinaryResponseDetector
{
public:
   static std::auto_ptr<ResponseDetector> NewByName(const std::string& name);

   virtual std::string GetMethodName() const;
   virtual int RecvExpected(MM::Core* core, MM::Device* device,
         const std::string& port, const std::vector<char>& expected);
   virtual int RecvAlternative(MM::Core* core, MM::Device* device,
         const std::string& port,
         const std::vector< std::vector<char> >& alternatives,
         size_t& index);

private:
   VariableLengthResponseDetector() {}
};
