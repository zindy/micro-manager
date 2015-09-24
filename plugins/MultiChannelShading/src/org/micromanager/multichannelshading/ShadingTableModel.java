/*
 * AUTHOR:
 * Kurt Thorn, Nico Stuurman
 *
 * Copyright (c) 2014 Regents of the University of California
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

package org.micromanager.multichannelshading;

import java.util.ArrayList;
import java.util.List;
import java.util.prefs.BackingStoreException;
import java.util.prefs.Preferences;
import javax.swing.table.AbstractTableModel;
import org.micromanager.api.ScriptInterface;
import org.micromanager.utils.MMException;
import org.micromanager.utils.ReportingUtils;

/**
 *
 * @author nico
 */
public class ShadingTableModel extends AbstractTableModel {
   private final ScriptInterface gui_;
   public final int PRESET = 0;
   public final int IMAGEFILE = 1;
   public final int LOADBUTTON = 2;
   public final String[] COLUMN_NAMES = new String[] {
         "Preset",
         "Image File",
         ""
   };
   private String channelGroup_;
   private final Preferences prefs_;
   private Preferences channelPrefs_;
   private List<String> presetList_;
   private List<String> fileList_;
   private final ImageCollection imageCollection_;
   
   public ShadingTableModel(ScriptInterface gui, ImageCollection 
           imageCollection) {
      gui_ = gui;
      imageCollection_ = imageCollection;
      prefs_ = Preferences.userNodeForPackage(this.getClass());
      presetList_ = new ArrayList<String>();
      fileList_ = new ArrayList<String>();
   }
   
   
   @Override
   public int getRowCount() {
      if (fileList_.size() < presetList_.size()) {
         return fileList_.size();
      } 
      return presetList_.size();
   }

   @Override
   public int getColumnCount() {
      return COLUMN_NAMES.length;
   }
   
   @Override
   public String getColumnName(int columnIndex) {
      return COLUMN_NAMES[columnIndex];
   }
   
   @Override
   public Object getValueAt(int row, int column) {
      switch (column) {
         case 0: 
            if (presetList_.size() <= row) {
               return "None";
            } else {
               return presetList_.get(row);
            }
         case 1: 
            if (fileList_.size() <= row) {
               return "";
            } else {
               return fileList_.get(row);
            }
         case 2: 
            //return new JButton("...");
      }
      return null;     
   }
   
   
   @Override
   public boolean isCellEditable(int rowIndex, int columnIndex) {
      return true;
   }
   
   @Override
   public void setValueAt(Object value, int row, int column) {
      switch (column) {
         case 0: 
            presetList_.set(row, (String) value);
            updateFlatFieldImage(row);
            break;
         case 1: 
            fileList_.set(row, (String) value);
            updateFlatFieldImage(row);
            break;
      }     
   }
   
   public void setChannelGroup(String newGroup) {
      try {
         if (channelGroup_ != null) {
            channelPrefs_ = prefs_.node(channelGroup_);
            channelPrefs_.clear();
            for (int i = 0; i < presetList_.size() && i < fileList_.size(); i++) {
               channelPrefs_.put(presetList_.get(i), fileList_.get(i));
            }
         }
         imageCollection_.clearFlatFields();
         channelGroup_ = newGroup;
         // restore mapping from preferences
         fileList_.clear();
         presetList_.clear();
         channelPrefs_ = prefs_.node(channelGroup_);

         for (String key : channelPrefs_.keys()) {
            String file = channelPrefs_.get(key, "");
            if (file.length() > 0) {
               imageCollection_.addFlatField(key, file);
               presetList_.add(key);
               fileList_.add(file);
            }
         }
      } catch (BackingStoreException ex) {
         gui_.logError(ex);
      } catch (MMException ex) {
         gui_.showError(ex);
      }
      fireTableDataChanged();
   }
   
   public String getChannelGroup() {
      return channelGroup_;
   }

   public void addRow() {
      String[] availablePresets = getAvailablePresets();
      if (availablePresets != null && availablePresets.length > 0) {
         presetList_.add(availablePresets[0]);
         fileList_.add("");
         fireTableDataChanged();
      }
      // TODO: handle error 
   }
   
   public String[] getAvailablePresets() {
      String[] presets = gui_.getMMCore().getAvailableConfigs(channelGroup_).
              toArray();
      String[] usedPresets = getUsedPresets();
      String[] availablePresets = new String[presets.length - usedPresets.length];
      for (String preset : presets) {
         boolean found = false;
         int j = 0;
         for (int i = 0; i < usedPresets.length; i++) {
            if (preset.equals(usedPresets[i])) {
               found = true;
            }
         }
         if (!found) {
            availablePresets[j] = preset;
            j++;
         }
      }
      return availablePresets;
   }
   
   public String[] getUsedPresets() {
      String[] presets = new String[presetList_.size()];
      for (int i = 0; i < presetList_.size(); i++) {
         presets[i] = presetList_.get(i);
      }
      return presets;
   }
   
   public String[] getUsedPresets(int excludedRow) {
      String[] presets = new String[presetList_.size() - 1];
      int j = 0;
      for (int i = 0; i < presetList_.size(); i++) {
         if (i != excludedRow) {
            presets[j] = presetList_.get(i);
            j++;
         }
      }
      return presets;
   }
   
   public int getNumberOfPresetsInCurrentGroup() {
      return (int) gui_.getMMCore().getAvailableConfigs(channelGroup_).size();
   }
   
   public int getUnusedNumberOfPresetsInCurrentGroup() {
      return getNumberOfPresetsInCurrentGroup() - getUsedPresets().length;
   }
   
   public ImagePlusInfo getFlatFieldImage (String channelGroup, String preset) {
      if (channelGroup.equals(channelGroup_)) {
         return imageCollection_.getFlatField(preset);
      }
      return null;
   }
   
   /**
    * Removes selected rows from the tablemodel
    * calls fireTableDataChanged to update the UI
    * @param selectedRows - array containing selected row numbers
    */
   public void removeRow(int[] selectedRows) {
      // Since we have ordered lists, rebuild them
      List<String> presetList = new ArrayList<String>();
      List<String> fileList = new ArrayList<String>();
      for (int i = 0; i < presetList_.size(); i++ ) {
         boolean removeRow = false;
         for (int j = 0; j < selectedRows.length; j++) {
            if (i == selectedRows[j]) {
               removeRow = true;
               channelPrefs_.remove(presetList_.get(i));
               imageCollection_.removeFlatField(presetList_.get(i));
            }
         }
         if (!removeRow) {
            presetList.add(presetList_.get(i));
            fileList.add(fileList_.get(i));
         }
      }
      presetList_ = presetList;
      fileList_ = fileList;
      
      fireTableDataChanged();
   }

   private void updateFlatFieldImage(int row) {   
      if (fileList_.get(row) != null && !fileList_.get(row).isEmpty()) {
         String preset = presetList_.get(row);
         if (preset != null) {
            try {
               imageCollection_.addFlatField(preset, fileList_.get(row));
            } catch (MMException ex) {
               ReportingUtils.showError(ex);
            }
            channelPrefs_.put(preset, fileList_.get(row));
         }
      }
         
   }
   
}
