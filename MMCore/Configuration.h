/*
 * Set of properties defined as a high level command
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, 09/08/2005
 *
 * Copyright (c) 2006 Regents of the University of California
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

#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#ifdef WIN32
// disable exception scpecification warnings in MSVC
#pragma warning( disable : 4290 )
#endif

#include <string>
#include <vector>
#include <map>
#include "Error.h"


/**
 * Property setting defined as triplet:
 * device - property - value.
 */
struct PropertySetting
{
   /**
    * Constructor for the struct specifying the entire contents.
    * @param deviceLabel
    * @param prop
    * @param value 
    */
    PropertySetting(const char* deviceLabel, const char* prop, const char* value, bool readOnly = false) :
      deviceLabel_(deviceLabel), propertyName_(prop), value_(value), readOnly_(readOnly)
      {
        key_ = generateKey(deviceLabel, prop);
      }

    PropertySetting() : readOnly_(false) {}
    ~PropertySetting() {}

   /**
    * Returns the device label.
    */
   std::string getDeviceLabel() const {return deviceLabel_;}
   /**
    * Returns the property name.
    */
   std::string getPropertyName() const {return propertyName_;}
   /**
    * Returns the read-only status.
    */
   bool getReadOnly() const {return readOnly_;}
   /**
    * Returns the property value.
    */
   std::string getPropertyValue() const {return value_;}

   std::string getKey() const {return key_;}

   static std::string generateKey(const char* device, const char* prop);

   std::string getVerbose() const;
   bool isEqualTo(const PropertySetting& ps);

private:
   std::string deviceLabel_;
   std::string propertyName_;
   std::string value_;
   std::string key_;
   bool readOnly_;
};

/**
 * Property pair defined as dublet:
 * dproperty - value.
 */
struct PropertyPair
{
   /**
    * Constructor for the struct specifying the entire contents.
    * @param prop
    * @param value 
    */
   PropertyPair(const char* prop, const char* value) :
      propertyName_(prop), value_(value) {}

   PropertyPair() {}
   ~PropertyPair() {}
   /**
    * Returns the property name.
    */
   std::string getPropertyName() const {return propertyName_;}
   /**
    * Returns the property value.
    */
   std::string getPropertyValue() const {return value_;}

private:
   std::string propertyName_;
   std::string value_;
};

/**
 * Encapsulation of the configuration information. Designed to be wrapped
 * by SWIG. A collection of configuration settings.
 */
class Configuration
{
public:

   Configuration() {}
   ~Configuration() {}

   /**
    * Adds new property setting to the existing contents.
    */
   void addSetting(const PropertySetting& setting);
   void deleteSetting(const char* device, const char* prop);

   bool isPropertyIncluded(const char* device, const char* property);
   bool isSettingIncluded(const PropertySetting& ps);
   bool isConfigurationIncluded(const Configuration& cfg);

   PropertySetting getSetting(size_t index) const throw (CMMError);
   PropertySetting getSetting(const char* device, const char* prop);
   
   /**
    * Returns the number of settings.
    */
   size_t size() const {return settings_.size();}
   std::string getVerbose() const;
 
private:
   std::vector<PropertySetting> settings_;
   std::map<std::string, int> index_;
};

/**
 * Encapsulation of the proeprty collection. Designed to be wrapped
 * by SWIG. A collection of property pairs.
 */
class PropertyBlock
{
public:

   PropertyBlock() {}
   ~PropertyBlock() {}

   void addPair(const PropertyPair& pair);
   PropertyPair getPair(size_t index) const throw (CMMError);
   /**
    * Returns the number of contained property parts.
    */
   size_t size() const {return pairs_.size();}
   std::string getValue(const char* key) const throw (CMMError);
 
private:
   std::map<std::string, PropertyPair> pairs_;
};

#endif //_CONFIGURATION_H_
