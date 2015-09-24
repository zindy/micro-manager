/*
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, December 2, 2006
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

import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.prefs.Preferences;

import javax.swing.CellEditor;
import javax.swing.InputMap;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.KeyStroke;
import javax.swing.ListSelectionModel;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableModel;

import org.micromanager.conf2.DevicesPage.DeviceTable_TableModel;
import org.micromanager.utils.GUIUtils;
import org.micromanager.utils.ReportingUtils;

/**
 * Wizard page to set device delays.
 *
 */
public class DelayPage extends PagePanel {
   private static final long serialVersionUID = 1L;

   class DelayTableModel extends AbstractTableModel {
      private static final long serialVersionUID = 1L;

      public final String[] COLUMN_NAMES = new String[] {
            "Name",
            "Adapter",
            "Delay [ms]"
      };
      
      MicroscopeModel model_;
      ArrayList<Device> devices_;
      
      public DelayTableModel(MicroscopeModel model) {
         devices_ = new ArrayList<Device>();
         Device allDevices[] = model.getDevices();
         for (int i=0; i<allDevices.length; i++) {
            if (allDevices[i].usesDelay())
               devices_.add(allDevices[i]);
         }
         model_ = model;
      }
      
      public void setMicroscopeModel(MicroscopeModel mod) {
         Device allDevices[] = mod.getDevices();
         for (int i=0; i<allDevices.length; i++) {
            if (allDevices[i].usesDelay())
            devices_.add(allDevices[i]);
         }
         model_ = mod;
      }
      
      public int getRowCount() {
         return devices_.size();
      }
      public int getColumnCount() {
         return COLUMN_NAMES.length;
      }
      public String getColumnName(int columnIndex) {
         return COLUMN_NAMES[columnIndex];
      }
      
      public Object getValueAt(int rowIndex, int columnIndex) {
         
         if (columnIndex == 0)
            return devices_.get(rowIndex).getName();
         else if (columnIndex == 1)
            return devices_.get(rowIndex).getAdapterName();
         else
            return new Double(devices_.get(rowIndex).getDelay());
      }
      public void setValueAt(Object value, int row, int col) {
         if (col == 2) {
            try {
               devices_.get(row).setDelay(Double.parseDouble((String)value));
               fireTableCellUpdated(row, col);
            } catch (Exception e) {
               ReportingUtils.logError(e);
            }
         }
      }
     
      public boolean isCellEditable(int nRow, int nCol) {
         if(nCol == 2)
            return true;
         else
            return false;
      }
      
      public void refresh() {
         Device allDevices[] = model_.getDevices();
         for (int i=0; i<allDevices.length; i++) {
            if (allDevices[i].usesDelay())
            devices_.add(allDevices[i]);
         }
         this.fireTableDataChanged();
      }
   }

   
   private JTable deviceTable_;
   /**
    * Create the panel
    */
   public DelayPage(Preferences prefs) {
      super();
      title_ = "Set delays for devices without synchronization capabilities";
      helpText_ = "Some devices can't signal when they are done with the command, so that we have to guess by manually setting the delay. " +
      "This means that the device will signal to be busy for the specified delay time after extecuting each command." +
      " Devices that may require setting the delay manually are mostly shutters or filter wheels. " +
      "\n\nIf device has normal synchronization capabilities, or you are not sure about it, leave this parameter at 0.";
      setHelpFileName("conf_delays_page.html");
      prefs_ = prefs;
      setLayout(null);

      final JScrollPane scrollPane = new JScrollPane();
      scrollPane.setBounds(22, 21, 517, 337);
      add(scrollPane);

      deviceTable_ = new JTable();
      deviceTable_.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      InputMap im = deviceTable_.getInputMap(JTable.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT);
      im.put( KeyStroke.getKeyStroke( KeyEvent.VK_ENTER, 0 ), "none" );
      scrollPane.setViewportView(deviceTable_);
      GUIUtils.setClickCountToStartEditing(deviceTable_,1);
      GUIUtils.stopEditingOnLosingFocus(deviceTable_);
   }

   public boolean enterPage(boolean next) {      
      rebuildTable();
      return true;
  }

   public boolean exitPage(boolean next) {
      CellEditor ce = deviceTable_.getCellEditor();
      if (ce != null) {
        deviceTable_.getCellEditor().stopCellEditing();
      }
      // apply delays to hardware
      try {

         model_.applyDelaysToHardware(core_);
      } catch (Exception e) {
         ReportingUtils.logError(e);
         if (next)
            return false; // refuse to go to the next page
      }
      return true;
   }
   
   private void rebuildTable() {
      TableModel tm = deviceTable_.getModel();
      DelayTableModel tmd;
      if (tm instanceof DeviceTable_TableModel) {
         tmd = (DelayTableModel) deviceTable_.getModel();
         tmd.refresh();
      } else {
         tmd = new DelayTableModel(model_);
         deviceTable_.setModel(tmd);
      }
      tmd.fireTableStructureChanged();
      tmd.fireTableDataChanged();
   }
   
   public void refresh() {
      rebuildTable();
   }

   public void loadSettings() {
   }

   public void saveSettings() {
   }

}
