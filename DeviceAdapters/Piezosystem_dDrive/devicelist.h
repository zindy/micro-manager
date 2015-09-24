/*
 * Implementation of the Piezosystem device adapter.
 *
 * AUTHOR:
 * Chris Belter, cbelter@piezojena.com 15/07/13.  XYStage and ZStage by Chris Belter
 *
 * Copyright (c) 2013 Piezosystem Jena, Germany
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

#ifndef _DEVICELIST_H_
#define _DEVICELIST_H_

#include <string>

class devicelist
{
public:
	devicelist(void);
	~devicelist(void);

	bool isStage(std::string name);
	bool isShutter(std::string name);
	bool isXYStage(std::string name);
	bool isTritor(std::string name);
	bool isMirror(std::string name);
	bool isMirror1(std::string name);
	bool isMirror2(std::string name);
	bool isMirror3(std::string name);
};



#endif //_DEVICELIST_H_
