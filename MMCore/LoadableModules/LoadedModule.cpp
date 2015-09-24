/*
 * Loadable module
 *
 * AUTHOR:
 * Mark Tsuchida
 *
 * Copyright (c) 2013 Regents of the University of California,
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

#include "LoadedModule.h"

#include "LoadedModuleImpl.h"
#include "../CoreUtils.h"
#include "../Error.h"


LoadedModule::LoadedModule(const std::string& filename) :
   filename_(filename)
{
   try
   {
      pImpl_ = LoadedModuleImpl::NewPlatformImpl(filename_);
   }
   catch (const CMMError& e)
   {
      throw CMMError("Failed to load module " + ToQuotedString(filename_), e);
   }
}


LoadedModule::~LoadedModule()
{
   delete pImpl_;
}


void
LoadedModule::Unload()
{
   try
   {
      pImpl_->Unload();
   }
   catch (const CMMError& e)
   {
      throw CMMError("Cannot unload module " + ToQuotedString(filename_), e);
   }
}


void*
LoadedModule::GetFunction(const char* funcName)
{
   try
   {
      return pImpl_->GetFunction(funcName);
   }
   catch (const CMMError& e)
   {
      throw CMMError("Cannot find function " + ToString(funcName) + "() in module " +
            ToQuotedString(filename_), e);
   }
}
