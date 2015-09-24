/*
 * AUTHOR:
 * Nico Stuurman, Jon Daniels
 *
 * Copyright (c) 2013 Regents of the University of California, & ASI
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

package org.micromanager.asidispim.Data;

import java.util.prefs.BackingStoreException;
import java.util.prefs.Preferences;

import org.micromanager.asidispim.Utils.MyDialogUtils;


/**
 * Central class for reading/writing preferences.
 * 
 * Our "node" isn't the same as a Java preference node, but we
 * implement as a prefix to the preference name.
 * 
 * @author Jon
 */
public class Prefs {

   private final Preferences myPrefs_;
   
   /**
    * Constructor
    * @param prefs
    */
   public Prefs(Preferences prefs) {
      myPrefs_ = prefs;
   }// constructor

   // Can also use the name of the property as the key
   // for calls to pref functions.  This is preferred if 
   // there is a single corresponding property.  
   // But sometimes the same property needs to be stored
   // in several preferences, such as beam/sheet control,
   // or else there is no corresponding property.
   // In that case use pref keys.
   public static enum Keys {
      JOYSTICK("Joystick"), 
      RIGHT_WHEEL("Right Wheel"),
      LEFT_WHEEL("Left Wheel"),
      CAMERA("Camera"),
      TAB_INDEX("tabIndex"),
      WIN_LOC_X("xlocation"),
      WIN_LOC_Y("ylocation"),
      SHEET_BEAM_ENABLED("SheetBeamEnabled"),
      SHEET_SCAN_ENABLED("SheetScanEnabled"),
      EPI_BEAM_ENABLED("EpiBeamEnabled"),
      EPI_SCAN_ENABLED("EpiScanEnabled"),
      ENABLE_BEAM_SETTINGS("EnableBeamSettings"),
      CHANNEL_USE_CHANNEL("ColorUse"),
      CHANNEL_CONFIG("ColorConfig"),
      NRCHANNELROWS("nrChannels"),
      AUTOFOCUSFITFUNCTION("AutofocusFitFunction"),
      NONE("None");
      private final String text;
      Keys(String text) {
         this.text = text;
      }
      @Override
      public String toString() {
         return text;
      }
   };
   
   private String getPrefKey(String node, Prefs.Keys key) {
      return node + "_" + key.toString();
   }
   
   private String getPrefKey(String node, Properties.Keys key) {
      return node + "_" + key.toString();
   }
   
   private String getPrefKey(String node, String strKey) {
      return node + "_" + strKey;
   }
   
   // used by BeamSubPanel only
   public void putBoolean(String node, Prefs.Keys key, boolean value) {
      myPrefs_.putBoolean(getPrefKey(node, key), value);
   }
   
   // used by BeamSubPanel only
   public boolean getBoolean(String node, Prefs.Keys key, boolean defaultValue) {
      return myPrefs_.getBoolean(getPrefKey(node, key), defaultValue);
   }
   
   public void putString(String node, Prefs.Keys key, String value) {
      myPrefs_.put(getPrefKey(node, key), value);
   }
   
   public String getString(String node, Prefs.Keys key, String defaultValue) {
      return myPrefs_.get(getPrefKey(node, key), defaultValue);
   }
   
   public void putInt(String node, Prefs.Keys key, int value) {
      myPrefs_.putInt(getPrefKey(node, key), value);
   }
   
   public int getInt(String node, Prefs.Keys key, int defaultValue) {
      return myPrefs_.getInt(getPrefKey(node, key), defaultValue);
   }

   public void putFloat(String node, Prefs.Keys key, float value) {
      myPrefs_.putFloat(getPrefKey(node, key), value);
   }
   
   public float getFloat(String node, Prefs.Keys key, float defaultValue) {
      return myPrefs_.getFloat(getPrefKey(node, key), defaultValue);
   }
   
   
   // same thing but using Property key instead of Prefs key
   
   public void putBoolean(String node, Properties.Keys key, boolean value) {
      myPrefs_.putBoolean(getPrefKey(node, key), value);
   }
   
   public boolean getBoolean(String node, Properties.Keys key, boolean defaultValue) {
      return myPrefs_.getBoolean(getPrefKey(node, key), defaultValue);
   }
   
   public void putString(String node, Properties.Keys key, String value) {
      myPrefs_.put(getPrefKey(node, key), value);
   }
   
   public String getString(String node, Properties.Keys key, String defaultValue) {
      return myPrefs_.get(getPrefKey(node, key), defaultValue);
   }
   
   public void putInt(String node, Properties.Keys key, int value) {
      myPrefs_.putInt(getPrefKey(node, key), value);
   }
   
   public int getInt(String node, Properties.Keys key, int defaultValue) {
      return myPrefs_.getInt(getPrefKey(node, key), defaultValue);
   }

   public void putFloat(String node, Properties.Keys key, float value) {
      myPrefs_.putFloat(getPrefKey(node, key), value);
   }
   
   public float getFloat(String node, Properties.Keys key, float defaultValue) {
      return myPrefs_.getFloat(getPrefKey(node, key), defaultValue);
   }
   
   
   
   // same thing, but using String as key
   
   public void putString(String node, String strKey, String value) {
      myPrefs_.put(getPrefKey(node, strKey), value);
   }
   
   public String getString(String node, String strKey, String defaultValue) {
      return myPrefs_.get(getPrefKey(node, strKey), defaultValue);
   }
   
   public void putFloat(String node, String strKey, float value) {
      myPrefs_.putFloat(getPrefKey(node, strKey), value);
   }
   
   public float getFloat(String node, String strKey, float defaultValue) {
      return myPrefs_.getFloat(getPrefKey(node, strKey), defaultValue);
   }
   
   
   /**
    * Searches for key in preferences.
    * 
    * @param node
    * @param key
    * @return
    */
   public boolean keyExists(String node, Properties.Keys key) {
      String allKeys[] = null;
      try {
         // this gets all the keys; our "node" is a privately-implemented thing, 
         // not the same as Java nodes which would be a sub-folder within regedit
         allKeys = myPrefs_.keys();
      } catch (BackingStoreException e) {
         MyDialogUtils.showError(e);
      }
      String lookFor = getPrefKey(node, key);
      for (String cur : allKeys) {
         if(cur.equals(lookFor)) {
            return true;
         }
      }
      return false;
   }
}
