/*
 * Loadable module implementation for OS X and Linux.
 *
 * AUTHOR:
 * Mark Tsuchida,
 * based on parts of CPluginManager by Nenad Amodaj
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

#include "LoadedModuleImplUnix.h"

#include "../Error.h"

#include <dlfcn.h>


static void __attribute__((noreturn))
ThrowDlError()
{
   const char* errorText = dlerror();
   if (!errorText)
      errorText = "Operating system error message not available";
   throw CMMError(errorText);
}


LoadedModuleImplUnix::LoadedModuleImplUnix(const std::string& filename)
{
   int mode = RTLD_NOW | RTLD_LOCAL;

   // Hack to make Andor adapter on Linux work
   // TODO Check if this is still necessary, and if so, why. If it is
   // necessary, add a more generic 'enable-lazy' mechanism.
   if (filename.find("libmmgr_dal_Andor.so") != std::string::npos)
      mode = RTLD_LAZY | RTLD_LOCAL;

   handle_ = dlopen(filename.c_str(), mode);
   if (!handle_)
      ThrowDlError();
}


void
LoadedModuleImplUnix::Unload()
{
   if (!handle_)
      return;

   int err = dlclose(handle_);
   handle_ = 0;
   if (err)
      ThrowDlError();
}


void*
LoadedModuleImplUnix::GetFunction(const char* funcName)
{
   if (!handle_)
      throw CMMError("Cannot get function from unloaded module");

   void* proc = dlsym(handle_, funcName);
   if (!proc)
      ThrowDlError();
   return proc;
}
