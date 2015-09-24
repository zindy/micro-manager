/*
 * Callback class used to send notifications from MMCore to
 * higher levels (such as GUI)
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, 12/10/2007
 *
 * Copyright (c) 2007 Regents of the University of California
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
#include <iostream>

class MMEventCallback
{
public:
   MMEventCallback() {}
   virtual ~MMEventCallback() {}

   virtual void onPropertiesChanged()
   {
      std::cout << "onPropertiesChanged()" << std:: endl; 
   }

   virtual void onPropertyChanged(const char* name, const char* propName, const char* propValue)
   {
      std::cout << "onPropertyChanged() " << name << " " << propName << " " << propValue;
      std::cout << std:: endl; 
   }

   virtual void onConfigGroupChanged(const char* groupName, const char* newConfigName)
   {
      std::cout << "onConfigGroupChanged() " << groupName << " " << newConfigName;
      std::cout << std:: endl; 
   }

   virtual void onSystemConfigurationLoaded()
   {
      std::cout << "onSystemConfigurationLoaded() ";
      std::cout << std::endl;
   }

   virtual void onPixelSizeChanged(double newPixelSizeUm)
   {
      std::cout << "onPixelSizeChanged() " << newPixelSizeUm << std::endl;
   }

   virtual void onStagePositionChanged(char* name, double pos)
   {
      std::cout << "onStagePositionChanged()" << name << " " << pos  << "\n"; 
   }

   virtual void onXYStagePositionChanged(char* name, double xpos, double ypos)
   {
      std::cout << "onXYStagePositionChanged()" << name << " " << xpos;
      std::cout << " " <<  ypos << "\n"; 
   }

   virtual void onExposureChanged(char* name, double newExposure)
   {
      std::cout << "onExposureChanged()" << name << " " << newExposure << "\n";
   }

   virtual void onSLMExposureChanged(char* name, double newExposure)
   {
      std::cout << "onSLMExposureChanged()" << name << " " << newExposure << "\n";
   }

};
