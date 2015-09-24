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

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.List;

import javax.swing.DefaultComboBoxModel;
import javax.swing.JComboBox;

import org.micromanager.asidispim.Utils.DevicesListenerInterface;
import org.micromanager.asidispim.Utils.MyDialogUtils;


/**
 * Class that holds utilities related to SPIM acquisition mode
 * 
 * @author Jon
 */
public class AcquisitionModes {
   
   private final Devices devices_;   // object holding information about selected/available devices
   private final Prefs prefs_;
   
   /**
    * Enum to store the acquisition mode along with associated preference code.
    * Wanted to store integer code instead of string so that the mode descriptions
    * can be easily changed. 
    */
   public static enum Keys { 
      PIEZO_SLICE_SCAN("Synchronous piezo/slice scan", 1),
      SLICE_SCAN_ONLY( "Slice scan only (beam thickness)", 2),
      NO_SCAN(         "No scan (fixed sheet)", 3),
      STAGE_SCAN(      "Stage scan", 4),
      STAGE_SCAN_INTERLEAVED("Stage scan interleaved", 5),
      PIEZO_SCAN_ONLY("Piezo scan only", 6), // only used for autofocus
      NONE(            "None", 0);
      private final String text;
      private final int prefCode;
      Keys(String text, int prefCode) {
         this.text = text;
         this.prefCode = prefCode;
      }
      @Override
      public String toString() {
         return text;
      }
      int getPrefCode() {
         return prefCode;
      }
   };
   
   public AcquisitionModes(Devices devices, Prefs prefs) {
      devices_ = devices;
      prefs_ = prefs;
   }
   
   /**
    * @param prefCode
    * @return null if prefCode not found, or the Key if it is
    */
   public static Keys getKeyFromPrefCode(int prefCode) {
      for (Keys k : Keys.values()) {
         if (k.getPrefCode() == prefCode) {
            return k;
         }
      }
      return null;
   }
   
   
   public JComboBox getComboBox() {
      JComboBox jcb = new JComboBox();
      ActionListener l = new ComboBoxListener(jcb);
      jcb.addActionListener(l);
      // when devices are changed we want to regenerate the list
      devices_.addListener((DevicesListenerInterface) l);
      return jcb;
   }
   
   private class ComboBoxListener implements ActionListener, DevicesListenerInterface {
      
      private final JComboBox jcb_;
      
      public ComboBoxListener(JComboBox jcb) {
         jcb_ = jcb;
         updateSelections();  // do initial rendering
      }
      
      @Override
      public void actionPerformed(ActionEvent ae) {
         prefs_.putInt(MyStrings.PanelNames.ACQUSITION.toString(),
               Properties.Keys.PLUGIN_ACQUSITION_MODE,
               ((Keys) jcb_.getSelectedItem()).getPrefCode());
      }
      
      /**
       * called whenever one of the devices is changed in the "Devices" tab
       */
      @Override
      public void devicesChangedAlert() {
         updateSelections();
      }
      
      /**
       * Resets the items in the combo box based on devices available.
       * Besides being called on original combo box creation, it is called 
       * whenever something in the devices tab is changed
       */
      private void updateSelections() {
         // save the existing selection if it exists
         int origCode = prefs_.getInt(MyStrings.PanelNames.ACQUSITION.toString(),
               Properties.Keys.PLUGIN_ACQUSITION_MODE, 0);
         
         DefaultComboBoxModel cbModel = new DefaultComboBoxModel();
         
         List<Keys> validModeKeys = getValidModeKeys();
         Keys origItem = null;
         for (Keys k : validModeKeys) {
            cbModel.addElement(k);
            if (k.getPrefCode() == origCode) {
               origItem = k;
            }
         }
         jcb_.setModel(cbModel);
         if (origItem != null) {
            jcb_.setSelectedItem(origItem);
         } else {
            // if existing selection isn't valid now then write new selection to prefs
            MyDialogUtils.showError("For preference " + Properties.Keys.PLUGIN_ACQUSITION_MODE.toString()
                  + " the previous selection \""
                  + getKeyFromPrefCode(origCode) + "\" is not valid.  Changing to default.");
            prefs_.putInt(MyStrings.PanelNames.ACQUSITION.toString(),
                  Properties.Keys.PLUGIN_ACQUSITION_MODE, ((Keys)jcb_.getSelectedItem()).getPrefCode());
         }
      }//updateSelections


      /**
       * Returns whatever acquisition modes are available based on devices
       * and installed firmware.  Can be expanded.
       * Decided to show all options and decide later whether they are doable with the
       * existing firmware/hardware, that way the end user at least knows such features exist 
       * @return
       */
      private List<Keys> getValidModeKeys() {
         List<Keys> keyList = new ArrayList<Keys>();
         keyList.add(Keys.PIEZO_SLICE_SCAN);
         keyList.add(Keys.SLICE_SCAN_ONLY);
         keyList.add(Keys.NO_SCAN);
         keyList.add(Keys.STAGE_SCAN);
         keyList.add(Keys.STAGE_SCAN_INTERLEAVED);
         return keyList;
      }

      
   }

}
