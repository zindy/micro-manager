/*
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

/*
 * NOTES:
 * This code must reside in the same binary image as the rest of the Core.
 */

#ifdef WIN32 // whole file

#include "LibraryPaths.h"


#include "../CoreUtils.h"
#include "../Error.h"

#include <boost/scoped_array.hpp>
#include <Psapi.h>


static HMODULE GetHandleOfThisModule()
{
   // This function is located in this module (obviously), so get the module
   // containing the address of this function.
   HMODULE hModule;
   BOOL ok = GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
         GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
         reinterpret_cast<LPCSTR>(&GetHandleOfThisModule),
         &hModule);
   if (!ok) {
      DWORD err = GetLastError();
      // TODO FormatMessage()
      throw CMMError("Cannot get handle to DLL or executable "
            "(Windows system error code " + ToString(err) + ")");
   }

   return hModule;
}


static std::string GetPathOfModule(HMODULE hModule)
{
   for (size_t bufsize = 1024, len = bufsize; ; bufsize += 1024)
   {
      boost::scoped_array<char> filename(new char[bufsize]);
      len = GetModuleFileNameA(hModule, filename.get(),
            static_cast<DWORD>(bufsize));
      if (!len)
      {
         DWORD err = GetLastError();
         // TODO FormatMessage()
         throw CMMError("Cannot get filename of DLL or executable "
               "(Windows system error code " + ToString(err) + ")");
      }

      if (len == bufsize) // Filename may not have fit in buffer
         continue;

      return filename.get();
   }
}


namespace MMCorePrivate {

std::string GetPathOfThisModule()
{
   return GetPathOfModule(GetHandleOfThisModule());
}

} // namespace MMCorePrivate

#endif // WIN32
