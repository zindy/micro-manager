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

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.AbstractCellEditor;
import javax.swing.JComboBox;
import javax.swing.JTable;
import javax.swing.table.TableCellEditor;

import mmcorej.CMMCore;
import mmcorej.StrVector;


/**
 * @author Jon
 */
@SuppressWarnings("serial")
public class ChannelConfigEditor extends AbstractCellEditor implements TableCellEditor {
   private final JComboBox channelGroup_;  // this is the combo box selecting 
            //the group, NOT the combo box selecting which preset of the group
   private final JComboBox channelPreset_;  // this is the combo box used by the table 
            // to select the appropriate preset
   private final CMMCore core_;
   
   public ChannelConfigEditor(JComboBox cb, CMMCore core) {
      channelGroup_ = cb;
      channelPreset_ = new JComboBox();
      channelPreset_.addActionListener(new ConfigActionListener());
      core_ = core;
  }
   
   class ConfigActionListener implements ActionListener {
      @Override
      public void actionPerformed(ActionEvent e) {
         fireEditingStopped();
      }
   }
   
   /** 
    * Called when cell value is edited by user.
    * @param rowIndex
    * @param colIndex
    */
   @Override
   public Component getTableCellEditorComponent(JTable table, Object value,
           boolean isSelected, int rowIndex, int colIndex) {
      channelPreset_.removeAllItems();
      StrVector configs = core_.getAvailableConfigs(channelGroup_.getSelectedItem().toString());
      for (String config : configs) {
         channelPreset_.addItem(config);
      }
      return channelPreset_;
   }
   
   /** 
    * Called when editing is completed.  Must return the new value to be stored in the cell.
    */
   @Override
   public Object getCellEditorValue() {
      return channelPreset_.getSelectedItem();
   }

}

