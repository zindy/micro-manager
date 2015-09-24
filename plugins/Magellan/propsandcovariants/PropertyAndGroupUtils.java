/*
 * AUTHOR:
 * Henry Pinkard, henry.pinkard@gmail.com
 *
 * Copyright (c) 2015 Regents of the University of California
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

package propsandcovariants;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.TreeMap;
import java.util.prefs.Preferences;
import main.Magellan;
import misc.Log;
import mmcorej.CMMCore;
import mmcorej.StrVector;

/**
 *
 * @author Henry
 */
public class PropertyAndGroupUtils {
   
   public static final String PROPERTY_LABEL_PREFIX = "Prop_label_map_";
   public static final String PROPERTY_INDEX_PREFIX = "Prop_index_map_";

   
   public static void saveStoredProperties(Preferences prefs, LinkedList<SinglePropertyOrGroup> props, TreeMap<String,String> labels) {
      //remove old ones
      LinkedList<SinglePropertyOrGroup> oldStoredProps = readStoredGroupsAndProperties(prefs);
      for (SinglePropertyOrGroup prop : oldStoredProps) {
         removePropFromPrefs(prefs, prop);
      }
      //add new ones
      for (int i = 0; i < props.size(); i++) {
          SinglePropertyOrGroup prop = props.get(i);
         addPropToPrefs(prefs, prop, labels.get(prop.toString()), i);
      }
   }
   
   public static LinkedList<SinglePropertyOrGroup> readStoredGroupsAndProperties(Preferences prefs) {
      ArrayList<SinglePropertyOrGroup> all = readConfigGroupsAndProperties(true);
      TreeMap<Integer, SinglePropertyOrGroup> stored = new TreeMap<Integer, SinglePropertyOrGroup>();
      for (SinglePropertyOrGroup item : all) {
         String label = getPropNickname(prefs, item);
         if (label != null) {
             int index = getPropIndex(prefs, item);
             if (index != -1) {
                 stored.put(index, item);
             }
         }
      }
      LinkedList<SinglePropertyOrGroup> list = new LinkedList<SinglePropertyOrGroup>();
      for (int i : stored.keySet()) {
          SinglePropertyOrGroup singlePropOrGroup = stored.get(i);      
         list.add(singlePropOrGroup);
      }
      return list;
   }


   
   private static int getPropIndex(Preferences prefs, SinglePropertyOrGroup prop) {     
      return prefs.getInt(PROPERTY_INDEX_PREFIX + prop.toString(), -1);
   }

   public static String getPropNickname(Preferences prefs, SinglePropertyOrGroup prop) {
      if (prop == null) {        
          return null;
      }
       return prefs.get(PROPERTY_LABEL_PREFIX + prop.toString(), null);
   }

   private static void addPropToPrefs(Preferences prefs, SinglePropertyOrGroup prop, String label, int index) {
      prefs.put(PROPERTY_LABEL_PREFIX + prop.toString(), label);
      prefs.putInt(PROPERTY_INDEX_PREFIX + prop.toString(), index);
   }

   private static void removePropFromPrefs(Preferences prefs, SinglePropertyOrGroup prop) {
      prefs.remove(PROPERTY_LABEL_PREFIX + prop.toString());
      prefs.remove(PROPERTY_INDEX_PREFIX + prop.toString());
   }
   
   public static ArrayList<SinglePropertyOrGroup> readConfigGroupsAndProperties(boolean includeReadOnly) {
      ArrayList<SinglePropertyOrGroup> list = readConfigGroups();
      list.addAll(PropertyAndGroupUtils.readAllProperties(includeReadOnly));
      return list;
   }
   
   private static ArrayList<SinglePropertyOrGroup> readConfigGroups() {
      ArrayList<SinglePropertyOrGroup> groups = new ArrayList<SinglePropertyOrGroup>();
      CMMCore core = Magellan.getCore();
      boolean liveMode = Magellan.getScriptInterface().isLiveModeOn();
         Magellan.getScriptInterface().enableLiveMode(false);
                  
         StrVector groupNames = core.getAvailableConfigGroups();   
      for (String group : groupNames) {
         SinglePropertyOrGroup pg = new SinglePropertyOrGroup();
         pg.readGroupValuesFromConfig(group);
         groups.add(pg);
      }
      Magellan.getScriptInterface().enableLiveMode(liveMode);
      return groups;
   }

   private static ArrayList<SinglePropertyOrGroup> readAllProperties(boolean includeReadOnly) {
      CMMCore core = Magellan.getCore();
      ArrayList<SinglePropertyOrGroup> props = new ArrayList<SinglePropertyOrGroup>();
      try {
         StrVector devices = core.getLoadedDevices();
         boolean liveMode = Magellan.getScriptInterface().isLiveModeOn();
         Magellan.getScriptInterface().enableLiveMode(false);

         for (int i = 0; i < devices.size(); i++) {
            StrVector properties = core.getDevicePropertyNames(devices.get(i));
            for (int j = 0; j < properties.size(); j++) {
               SinglePropertyOrGroup item = new SinglePropertyOrGroup();
               item.readFromCore(devices.get(i), properties.get(j), false);
               if (!item.preInit && (!item.readOnly || includeReadOnly) ) {
                  props.add(item);
               }
            }
         }
         Magellan.getScriptInterface().enableLiveMode(liveMode);
      } catch (Exception e) {
         Log.log("Problem reading properties from core");
      }
      return props;
   }

   
   
}
