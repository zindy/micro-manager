/*
 * Exception class for core errors
 *
 * AUTHOR:
 * Nenad Amodaj (original header-only version)
 * Mark Tsuchida, 12/04/2013 (made chainable)
 *
 * Copyright (c) 2006 Regents of the University of California,
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

/*
 * NOTES:
 *            API documentation is in the header file.
 */

#include "Error.h"


CMMError::CMMError(const std::string& msg, Code code) :
   message_(msg),
   code_(code),
   underlying_(0)
{}


CMMError::CMMError(const char* msg, Code code) :
   message_(msg ? msg : "(null message)"),
   code_(code),
   underlying_(0)
{}


CMMError::CMMError(const std::string& msg, Code code, const CMMError& underlyingError) :
   message_(msg),
   code_(code),
   underlying_(new CMMError(underlyingError))
{}


CMMError::CMMError(const char* msg, Code code, const CMMError& underlyingError) :
   message_(msg ? msg : "(null message)"),
   code_(code),
   underlying_(new CMMError(underlyingError))
{}


CMMError::CMMError(const std::string& msg, const CMMError& underlyingError) :
   message_(msg),
   code_(MMERR_GENERIC),
   underlying_(new CMMError(underlyingError))
{}


CMMError::CMMError(const char* msg, const CMMError& underlyingError) :
   message_(msg ? msg : "(null message)"),
   code_(MMERR_GENERIC),
   underlying_(new CMMError(underlyingError))
{}


CMMError::CMMError(const CMMError& other) :
   message_(other.message_),
   code_(other.code_),
   underlying_(0)
{
   if (other.getUnderlyingError())
      underlying_.reset(new CMMError(*(other.getUnderlyingError())));
}


std::string
CMMError::getMsg() const
{
   if (message_.empty())
      return "Error (code " + boost::lexical_cast<std::string>(code_) + ")";
   return message_;
}


std::string
CMMError::getFullMsg() const
{
   if (getUnderlyingError())
      return getMsg() + " [ " + underlying_->getFullMsg() + " ]";
   return getMsg();
}


CMMError::Code
CMMError::getSpecificCode() const
{
   if (code_ == MMERR_OK || code_ == MMERR_GENERIC)
   {
      if (getUnderlyingError())
         return getUnderlyingError()->getSpecificCode();
      else
         return MMERR_GENERIC;
   }
   return code_;
}


const CMMError*
CMMError::getUnderlyingError() const
{
   return underlying_.get();
}
