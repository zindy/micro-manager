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

import java.awt.Component;
import java.awt.Font;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import javax.swing.AbstractCellEditor;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.table.TableCellEditor;

/**
 * Editor component for the position list table
 */
public class CellEditor extends AbstractCellEditor implements TableCellEditor, 
        FocusListener {
   private static final long serialVersionUID = 3L;
   // This is the component that will handle editing of the cell's value
   JTextField text_ = new JTextField();
   int editingCol_;

   public CellEditor(Font editingFont) {
      super();
      text_.setFont(editingFont);
   }
   
   public void addListener() {
      text_.addFocusListener(this);
   }

   @Override
   public void focusLost(FocusEvent e) {
      fireEditingStopped();
   }

   @Override
   public void focusGained(FocusEvent e) {

   }

   // This method is called when a cell value is edited by the user.
   @Override
   public Component getTableCellEditorComponent(JTable table, Object value,
         boolean isSelected, int rowIndex, int colIndex) {

     editingCol_ = colIndex;

      // Configure the component with the specified value
      if (colIndex == 0) {
         text_.setText((String)value);
         return text_;
      }

      return null;
   }
                                                                          
   // This method is called when editing is completed.
   // It must return the new value to be stored in the cell. 
   @Override
   public Object getCellEditorValue() {
      if (editingCol_ == 0) {
            return text_.getText();
      }
      return null;
   }
}
