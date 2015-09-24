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

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import javax.swing.AbstractCellEditor;
import javax.swing.JComboBox;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.table.TableCellEditor;
import mmcloneclasses.utils.SliderPanel;

/**
 * Cell editing using either JTextField or JComboBox depending on whether the
 * property enforces a set of allowed values.
 */
public class CovariantValueCellEditor extends AbstractCellEditor implements TableCellEditor {

   private static final long serialVersionUID = 1L;
   // This is the component that will handle the editing of the cell value
   JTextField text_ = new JTextField();
   JComboBox combo_ = new JComboBox();
   SliderPanel slider_ = new SliderPanel();
   Covariant item_;

   public CovariantValueCellEditor() {
      super();

      // end editing on selection change
      combo_.addActionListener(new ActionListener() {

         @Override
         public void actionPerformed(ActionEvent e) {
            fireEditingStopped();
         }
      });

      slider_.addEditActionListener(new ActionListener() {

         @Override
         public void actionPerformed(ActionEvent e) {
            fireEditingStopped();
         }
      });

      slider_.addSliderMouseListener(new MouseAdapter() {

         @Override
         public void mouseReleased(MouseEvent e) {
            fireEditingStopped();
         }
      });

      text_.addKeyListener(new KeyAdapter() {

         @Override
         public void keyPressed(KeyEvent e) {
            if (e.getKeyCode() == KeyEvent.VK_ENTER) {
               fireEditingStopped();
            }
         }
      });

      text_.addFocusListener(new FocusAdapter() {

         @Override
         public void focusLost(FocusEvent e) {
            // fireEditingStopped();
         }
      });
   }

   // This method is called when a cell value is edited by the user.
   @Override
   public Component getTableCellEditorComponent(JTable table, Object value,
           boolean isSelected, int rowIndex, int colIndex) {

      CovariantPairValuesTableModel data = (CovariantPairValuesTableModel) table.getModel();
      item_ = colIndex == 0 ?  data.getPairing().getIndependentCovariant() : data.getPairing().getDependentCovariant();

      // Configure the component with the specified value:
      if (!item_.isDiscrete()) {
//         if (item_.hasLimits()) {
//            if (item_.getType() == CovariantType.INT) {
//               slider_.setLimits( item_.getLowerLimit().intValue(), item_.getUpperLimit().intValue());
//            } else {
//               slider_.setLimits( item_.getLowerLimit().doubleValue(), item_.getUpperLimit().doubleValue());
//            }
//            try {
//               slider_.setText(((CovariantValue) value).toString());
//            } catch (ParseException ex) {
//               Log.log(ex);
//            }
//            return slider_;
//         } else {
            text_.setText(((CovariantValue) value).toString());
            return text_;
//         }
      } else {
         ActionListener[] l = combo_.getActionListeners();
         for (int i = 0; i < l.length; i++) {
            combo_.removeActionListener(l[i]);
         }
         combo_.removeAllItems();
         for (int i = 0; i < item_.getAllowedValues().length; i++) {
            combo_.addItem(item_.getAllowedValues()[i]);
         }
         combo_.setSelectedItem(data.getPairing().getValue(colIndex, rowIndex));

         // end editing on selection change
         combo_.addActionListener(new ActionListener() {

            @Override
            public void actionPerformed(ActionEvent e) {
               fireEditingStopped();
            }
         });

         return combo_;
      }
   }
   
   

   // This method is called when editing is completed.
   // It must return the new value to be stored in the cell.
   @Override
   public Object getCellEditorValue() {
      if (!item_.isDiscrete()) {
//         if (item_.hasLimits()) {
//            return slider_.getText();
//         } else {
            return text_.getText();
//         }
      } else {
         return combo_.getSelectedItem();
      }
      
   }
}

