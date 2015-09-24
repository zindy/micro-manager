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
 * Class that holds utilities related to multicolor sequence order
 * (e.g. change color slice by slice, etc.).
 * 
 * @author Jon
 */
public class MultichannelModes {
   
   private final Devices devices_;   // object holding information about selected/available devices
   private final Properties props_;  // object handling all property read/writes
   private final Devices.Keys devKey_;
   private final Properties.Keys propKey_;
   
   /**
    * Enum to store the acquisition mode along with associated preference code.
    * Wanted to store integer code instead of string so that the mode descriptions
    * can be easily changed. 
    */
   public static enum Keys {
      VOLUME("Every volume", 1),
      VOLUME_HW("Every volume (PLogic)", 2),
      SLICE_HW("Every slice (PLogic)", 3),
      NONE("None", 0);
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
      public int getPrefCode() {
         return prefCode;
      }
   };
   
   public MultichannelModes(Devices devices, Properties props,
         Devices.Keys devKey, Properties.Keys propKey, Keys defaultKey) {
      devices_ = devices;
      props_ = props;
      devKey_ = devKey;
      propKey_ = propKey;
      
      // make sure it gets set to something
      if (!props_.hasProperty(devKey, propKey)) {
         props_.setPropValue(devKey,
               propKey, defaultKey.getPrefCode());
      }
   }
   
   /**
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
         props_.setPropValue(devKey_, propKey_,
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
       * whenever something in the devices tab is changed.
       */
      private void updateSelections() {
         // save the existing selection if it exists
         int origCode = props_.getPropValueInteger(devKey_, propKey_);
         
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
            MyDialogUtils.showError("For preference " + propKey_.toString() + " the previous selection \""
                  + getKeyFromPrefCode(origCode) + "\" is not valid.  Changing to default.");
            props_.setPropValue(devKey_, propKey_, ((Keys)jcb_.getSelectedItem()).getPrefCode());
         }
      }//updateSelections


      /**
       * Returns whatever modes are available.
       * Can be expanded in the future
       * @return
       */
      private List<Keys> getValidModeKeys() {
         List<Keys> keyList = new ArrayList<Keys>();
         keyList.add(Keys.VOLUME);
         // PLogic required for hardware switching
         if (devices_.isValidMMDevice(Devices.Keys.PLOGIC) &&
               props_.getPropValueFloat(Devices.Keys.PLOGIC, Properties.Keys.FIRMWARE_VERSION) > 3.069) {
            keyList.add(Keys.VOLUME_HW);
            keyList.add(Keys.SLICE_HW);
         }
         return keyList;
      }

      
   }

}
