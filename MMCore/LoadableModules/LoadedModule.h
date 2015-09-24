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

#pragma once

#include <boost/utility.hpp>
#include <string>


class LoadedModuleImpl;

class LoadedModule /* final */ : boost::noncopyable
{
public:
   explicit LoadedModule(const std::string& filename);
   ~LoadedModule();

   void Unload(); // For developer use only
   void* GetFunction(const char* funcName);

private:
   LoadedModuleImpl* pImpl_;
   const std::string filename_;
};
