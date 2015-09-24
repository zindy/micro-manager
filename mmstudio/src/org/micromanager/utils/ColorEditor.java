/*
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, November 10, 2005
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

package org.micromanager.utils;

import java.awt.Color;
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.AbstractCellEditor;
import javax.swing.JButton;
import javax.swing.JColorChooser;
import javax.swing.JDialog;
import javax.swing.JTable;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableCellEditor;

/**
 * Color chooser for channel data.
 */
public class ColorEditor extends AbstractCellEditor implements TableCellEditor,
ActionListener {
   private static final long serialVersionUID = -5497293610937812813L;
   Color currentColor;
   JButton button;
   JColorChooser colorChooser;
   JDialog dialog;
   protected static final String EDIT = "edit";
   int column_;
   AbstractTableModel model_;

   public ColorEditor(AbstractTableModel model, int column) {
      //Set up the editor (from the table's point of view),
      //which is a button.
      //This button brings up the color chooser dialog,
      //which is the editor from the user's point of view.
      button = new JButton();
      button.setActionCommand(EDIT);
      button.addActionListener(this);
      button.setBorderPainted(false);
      column_ = column;
      model_ = model;

      //Set up the dialog that the button brings up.
      colorChooser = new JColorChooser();
      dialog = JColorChooser.createDialog(button,
            "Pick a Color",
            true,  //modal
            colorChooser,
            this,  //OK button handler
            null); //no CANCEL button handler
   }

   /**
    * Handles events from the editor button and from
    * the dialog's OK button.
    */
   @Override
   public void actionPerformed(ActionEvent e) {
      if (EDIT.equals(e.getActionCommand())) {
         //The user has clicked the cell, so
         //bring up the dialog.
         button.setBackground(currentColor);
         colorChooser.setColor(currentColor);
         dialog.setVisible(true);

         // Make the renderer reappear.
         fireEditingStopped();
         // Fire an event to enable saving the new color in the colorprefs
         // Don't know how to fire just for this row:
         for (int row=0; row < model_.getRowCount(); row++) {
            model_.fireTableCellUpdated(row, column_);
         }

      } else { //User pressed dialog's "OK" button.
         currentColor = colorChooser.getColor();
      }
   }

   //Implement the one CellEditor method that AbstractCellEditor doesn't.
   @Override
   public Object getCellEditorValue() {
      return currentColor;
   }

   //Implement the one method defined by TableCellEditor.
   @Override
   public Component getTableCellEditorComponent(JTable table,
         Object value,
         boolean isSelected,
         int row,
         int column) {
      currentColor = (Color)value;
      return button;
   }
}
