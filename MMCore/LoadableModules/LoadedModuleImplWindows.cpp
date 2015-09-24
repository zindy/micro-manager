/*
 * Loadable module implementation for Windows.
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

#include "LoadedModuleImplWindows.h"

#include "../Error.h"

#include <boost/algorithm/string.hpp>


static void __declspec(noreturn)
ThrowLastError()
{
   std::string errorText;

   DWORD err = GetLastError();
   LPSTR pMsgBuf(0);
   if (FormatMessageA( 
         FORMAT_MESSAGE_ALLOCATE_BUFFER | 
         FORMAT_MESSAGE_FROM_SYSTEM | 
         FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL,
         err,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
         (LPSTR)&pMsgBuf,
         0,
         NULL) && pMsgBuf)
   {
      errorText = pMsgBuf;

      // Windows error messages sometimes have trailing newlines
      boost::algorithm::trim(errorText);

      // This particular message can be rather misleading.
      if (errorText == "The specified module could not be found.") {
         errorText = "The module, or a module it depends upon, could not be found "
            "(Windows error: " + errorText + ")";
      }
   }
   if (pMsgBuf)
   {
      LocalFree(pMsgBuf);
   }

   if (errorText.empty()) {
      errorText = "Operating system error message not available";
   }

   throw CMMError(errorText);
}


LoadedModuleImplWindows::LoadedModuleImplWindows(const std::string& filename)
{
   int saveErrorMode = SetErrorMode(SEM_NOOPENFILEERRORBOX | SEM_FAILCRITICALERRORS);
   handle_ = LoadLibrary(filename.c_str());
   SetErrorMode(saveErrorMode);
   if (!handle_)
      ThrowLastError();
}


void
LoadedModuleImplWindows::Unload()
{
   if (!handle_)
      return;

   BOOL ok = FreeLibrary(handle_);
   handle_ = 0;
   if (!ok)
      ThrowLastError();
}


void*
LoadedModuleImplWindows::GetFunction(const char* funcName)
{
   if (!handle_)
      throw CMMError("Cannot get function from unloaded module");

   void* proc = GetProcAddress(handle_, funcName);
   if (!proc)
      ThrowLastError();
   return proc;
}
