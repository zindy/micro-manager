/*
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, November 7, 2006
 *
 * Copyright (c) 2006 Regents of the University of California
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

package org.micromanager.conf2;

import java.util.ArrayList;

/**
 * Encapsulation of the preset data for use in the Configuration Wizard. 
 *
 */
public class ConfigPreset {
   private String name_;
   private ArrayList<Setting> settings_;
   
   // this field is used onluy in case the configuration preset
   // belongs to the pixelSize group
   private double pixelSizeUm_ = 0.0;
   
   public ConfigPreset() {
      name_ = new String("Undefined");
      settings_ = new ArrayList<Setting>();
   }
   
   public ConfigPreset(String name) {
      name_ = name;
      settings_ = new ArrayList<Setting>();
   }
   
   public String getName() {
      return name_;
   }
   
   public boolean addSetting(Setting s) {
      for (int i=0; i<settings_.size(); i++) {
         if (getSetting(i).isEqualTo(s))
            return false;
         if (getSetting(i).matches(s)) {
            // replace existing
            settings_.set(i, s);
            return true;
         }
      }
      // add new
      settings_.add(s);
      return true;
   }
   
   public boolean matchSetting(Setting s) {
      for (int i=0; i<settings_.size(); i++) {
         if (getSetting(i).matches(s)) {
            return true;
         }
      }
      return false;
   }
   
   public boolean removeSetting(Setting s) {
      for (int i=0; i<settings_.size(); i++) {
         if (getSetting(i).isEqualTo(s)) {
            settings_.remove(i);
            return true;
         }
      }
      return false;
   }
   
   public int getNumberOfSettings() {
      return settings_.size();
   }
   
   public Setting getSetting(int i) {
      return settings_.get(i);
   }
   
   public String toString() {
      return new String("Preset: " + name_);
   }

   public void setName(String name) {
      name_ = name;
   }

   public void setPixelSizeUm(double ps) {
      pixelSizeUm_ = ps;
   }
   
   public double getPixelSize() {
      return pixelSizeUm_;
   }
}
