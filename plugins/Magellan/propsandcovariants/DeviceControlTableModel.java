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

import propsandcovariants.PropertyAndGroupUtils;
import java.util.LinkedList;
import java.util.prefs.Preferences;
import javax.swing.table.AbstractTableModel;
import main.Magellan;
import misc.Log;
import misc.NumberUtils;
import mmcorej.CMMCore;


/**
 *
 * @author Henry
 */
public class DeviceControlTableModel extends AbstractTableModel   {
     
   
   private LinkedList<SinglePropertyOrGroup> storedGroupsAndProps_;
   
   private CMMCore core_;
   private Preferences prefs_;

   
   public DeviceControlTableModel(Preferences prefs) {
      core_ = Magellan.getCore();
       storedGroupsAndProps_ = PropertyAndGroupUtils.readStoredGroupsAndProperties(prefs);
      prefs_ = prefs;
   }
   
   public void updateStoredProps() {
      storedGroupsAndProps_ = PropertyAndGroupUtils.readStoredGroupsAndProperties(prefs_);      
   }
   
   @Override
   public int getRowCount() {
     return storedGroupsAndProps_.size();
   }

   @Override
   public int getColumnCount() {
      return 2;
   }
   
   @Override
   public void setValueAt(Object value, int row, int col) {
      if (col == 1) {
         SinglePropertyOrGroup item = storedGroupsAndProps_.get(row);
         setValueInCore(item, value);
         core_.updateSystemStateCache();
         Magellan.getScriptInterface().refreshGUIFromCache();
         fireTableCellUpdated(row, col);
      }
   }

   @Override
   public Object getValueAt(int rowIndex, int columnIndex) {
      SinglePropertyOrGroup item = storedGroupsAndProps_.get(rowIndex);
      if (columnIndex == 0) {
         //prop label
         return PropertyAndGroupUtils.getPropNickname(prefs_, storedGroupsAndProps_.get(rowIndex));
      } else {
         //prop value
         return item.value;
      }
   }

   @Override
   public boolean isCellEditable(int nRow, int nCol) {
      if (nCol == 0) {
         return false;
      } else {
         return !storedGroupsAndProps_.get(nRow).readOnly;
      }
   }
  
   private void setValueInCore(SinglePropertyOrGroup item, Object value) {
      try {
         if (item.isGroup()) {
            core_.setConfig(item.name, value.toString());
         } else if (item.isInteger()) {
            core_.setProperty(item.device, item.name, NumberUtils.intStringDisplayToCore(value));
         } else if (item.isFloat()) {
            core_.setProperty(item.device, item.name, NumberUtils.doubleStringDisplayToCore(value));
         } else {
            core_.setProperty(item.device, item.name, value.toString());
         }
         item.value = value.toString();
         if (!item.isGroup()) {
            core_.waitForDevice(item.device);
         }
      } catch (Exception e) {
         Log.log(e);         
      }

   }

   public SinglePropertyOrGroup getPropertyItem(int rowIndex) {
      return storedGroupsAndProps_.get(rowIndex);
   }

}
