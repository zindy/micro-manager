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

import java.awt.Color;
import java.awt.Component;
import javax.swing.JLabel;
import javax.swing.JTable;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableCellRenderer;


public class CovariantValueCellRenderer implements TableCellRenderer {
   // This method is called each time a cell in a column
   // using this renderer needs to be rendered.

   JLabel lab_ = new JLabel();

   public CovariantValueCellRenderer() {
      super();
   }


   @Override
   public Component getTableCellRendererComponent(JTable table, Object value,
           boolean isSelected, boolean hasFocus, int rowIndex, int colIndex) {

      CovariantPairValuesTableModel data = (CovariantPairValuesTableModel) table.getModel();       
      Covariant cv = colIndex == 0 ?  data.getPairing().getIndependentCovariant() : data.getPairing().getDependentCovariant();
      
      lab_.setOpaque(true);
      lab_.setHorizontalAlignment(JLabel.LEFT);
      Component comp;
      
//      if (cv.hasLimits()) {
//         SliderPanel slider = new SliderPanel();
//         if (cv.getType() == CovariantType.INT) {
//            slider.setLimits(cv.getLowerLimit().intValue(), cv.getUpperLimit().intValue());
//         } else {
//            slider.setLimits(cv.getLowerLimit().doubleValue(), cv.getUpperLimit().doubleValue());
//         }
//         try {
//            slider.setText(((CovariantValue) value).toString());
//         } catch (ParseException ex) {
//            Log.log(ex);
//         }
//         slider.setToolTipText(data.getPairing().getValue(colIndex, rowIndex).toString());
//         comp = slider;
//      } else {
      try {
         lab_.setText(data.getPairing().getValue(colIndex, rowIndex).toString());
         comp = lab_;

//      }
      
      if (rowIndex == table.getSelectedRow()) {
         Component c = (new DefaultTableCellRenderer()).getTableCellRendererComponent(table, value, 
                 true, hasFocus, rowIndex, colIndex);
         comp.setForeground(c.getForeground());
         comp.setBackground(c.getBackground());
      } else {
         comp.setBackground(Color.WHITE);
         comp.setForeground(Color.black);
      }

      } catch (Exception e) {
         System.out.println();
         throw new RuntimeException();
      }
      return comp;
   }

   // The following methods override the defaults for performance reasons
   public void validate() {
   }

   public void revalidate() {
   }

   protected void firePropertyChange(String propertyName, Object oldValue, Object newValue) {
   }

   public void firePropertyChange(String propertyName, boolean oldValue, boolean newValue) {
   }
}

