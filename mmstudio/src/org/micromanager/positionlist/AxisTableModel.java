/*
 * Copyright (c) 2005-2015 Regents of the University of California
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

package org.micromanager.positionlist;

import com.google.common.eventbus.EventBus;

import java.util.prefs.Preferences;

import javax.swing.JTable;
import javax.swing.table.AbstractTableModel;

/**
 * Model holding axis data, used to determine which axis will be recorded
 */
class AxisTableModel extends AbstractTableModel {
   private static final long serialVersionUID = 1L;
   private boolean isEditable_ = true;
   private AxisList axisList_;
   private JTable axisTable_;
   private EventBus bus_;
   private Preferences prefs_;
   public final String[] COLUMN_NAMES = new String[] {
         "Use",
         "Stage name"
   };
  
   public AxisTableModel(AxisList list, JTable table, EventBus bus, 
         Preferences prefs) {
      axisList_ = list;
      axisTable_ = table;
      bus_ = bus;
      prefs_ = prefs;
   }

   @Override
   public int getRowCount() {
      return axisList_.getNumberOfPositions();
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
   public Object getValueAt(int rowIndex, int columnIndex) {
      AxisData aD = axisList_.get(rowIndex);
      if (aD != null) {
         if (columnIndex == 0) {
            return aD.getUse();
         } else if (columnIndex == 1) {
            return aD.getAxisName();
         }
      }
      return null;
   }
   @Override
   public Class<?> getColumnClass(int c) {
      return getValueAt(0, c).getClass();
   }
   public void setEditable(boolean state) {
      isEditable_ = state;
      if (state) {
         for (int i=0; i < getRowCount(); i++) {
            
         }
      }
   }
   @Override
   public boolean isCellEditable(int rowIndex, int columnIndex) {
      if (columnIndex == 0) {
         return isEditable_;
      }
      return false;
   }
   @Override
   public void setValueAt(Object value, int rowIndex, int columnIndex) {
      if (columnIndex == 0) { // I.e. action was in the column with checkboxes
         axisList_.get(rowIndex).setUse((Boolean) value);
         prefs_.putBoolean(axisList_.get(rowIndex).getAxisName(), 
               (Boolean) value); 
         bus_.post(new MoversChangedEvent());
      }
      fireTableCellUpdated(rowIndex, columnIndex);
      axisTable_.clearSelection();
   }
}
