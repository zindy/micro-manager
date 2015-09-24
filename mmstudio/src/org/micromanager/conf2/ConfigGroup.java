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

import java.util.Hashtable;

/**
 * Configuration group encapsulation for use in Configuration Wizard. 
 */
public class ConfigGroup {
   String name_;
   Hashtable<String, ConfigPreset> configs_;
   
   public ConfigGroup(String name) {
      name_ = new String(name);
      configs_ = new Hashtable<String, ConfigPreset>();
   }
   
   public void addConfigPreset(ConfigPreset p) {
      configs_.put(p.getName(), p);
   }
   
   public String getName() {
      return name_;
   }
   
   public void addConfigSetting(String presetName, String device, String property, String value) {
      ConfigPreset cp = configs_.get(presetName);
      if (cp == null) {
         cp = new ConfigPreset(presetName);
         configs_.put(presetName, cp);
      }
      
      cp.addSetting(new Setting(device, property, value));
   }
   
   public ConfigPreset[] getConfigPresets() {
      Object objs[] = configs_.values().toArray();
      ConfigPreset[] cps = new ConfigPreset[objs.length];
      for (int i=0; i<objs.length; i++)
         cps[i] = (ConfigPreset)objs[i];
      return cps;
   }
   
   public String toString() {
      return new String("Group: " + name_);
   }

   public void removePreset(String name) {
      configs_.remove(name);
   }

   public ConfigPreset findConfigPreset(String name) {
      return configs_.get(name);
   }

   public void setName(String name) {
      name_ = name;
   }

   public void renamePreset(ConfigPreset prs, String name) {
      configs_.remove(prs.getName());
      prs.setName(name);
      configs_.put(name, prs);
   }
   
   public void clear() {
      configs_.clear();
   }
   
}
