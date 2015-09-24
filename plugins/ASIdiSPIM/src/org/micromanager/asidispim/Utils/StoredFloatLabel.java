/*
 * AUTHOR:
 * Nico Stuurman, Jon Daniels
 *
 * Copyright (c) 2014 Regents of the University of California, & ASI
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

package org.micromanager.asidispim.Utils;

import java.text.ParseException;

import javax.swing.JLabel;

import org.micromanager.asidispim.Data.Prefs;
import org.micromanager.utils.NumberUtils;


/**
 * Utility class that ties a JLabel to a value stored in the preferences
 * @author Nico
 */
@SuppressWarnings("serial")
public class StoredFloatLabel extends JLabel {
   private final String prefNode_;
   private final String prefKey_;
   private final Prefs prefs_;
   private final String units_;

   /**
    * Creates a JLabel and overrides the setText methods so that 
    * changed in the Label will be written to the preferences
    * Adds a setFloat method for convenience
    * 
    * @param prefNode - Node used to store the value in preferences
    * @param prefKey - Key used to store the value in preferences
    * @param defaultValue - default value in case nothing is found in prefs
    * @param prefs - Global preferences object used in this plugin
    * @param gui - MM ScriptInterface instance
    * @param units - string to be displayed after the float, usually containing units
    */
   public StoredFloatLabel(String prefNode, String prefKey, float defaultValue, 
         Prefs prefs, String units) {
      super();
      prefNode_ = prefNode;
      prefKey_ = prefKey;
      prefs_ = prefs;
      units_ = units;
      super.setText("" + (prefs_.getFloat(prefNode, prefKey, defaultValue)) + units);
   }

   /**
    * Sets the text of the JLabel and stores the value in Preferences
    * @param txt 
    */
   @Override
   public void setText(String txt) {
      super.setText(txt);
      if (prefNode_ != null && prefKey_ != null) {
         try {
            prefs_.putFloat(prefNode_, prefKey_,
                  ((Double) NumberUtils.displayStringToDouble(txt)).floatValue());
         } catch (ParseException e) {
            MyDialogUtils.showError(e);
         }
      }
   }

   /**
    * Convenience method to display a float and store value in prefs 
    * @param val 
    */
   public void setFloat(float val) {
      super.setText(NumberUtils.doubleToDisplayString(val) + units_);
      if (prefNode_ != null && prefKey_ != null) {
         prefs_.putFloat(prefNode_, prefKey_, val);
      }
   }

   /**
    * @return - displayed value as a float
    */
   public float getFloat() {
      String txt = getText();
      float result = 0;
      try {
         result = (float) NumberUtils.displayStringToDouble(txt);
      } catch (ParseException pe) {
         // do nothing
      }
      return result;
   }
   
}
