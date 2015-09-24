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

import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableColumnModel;

/**
 *
 * @author Henry
 */
public class CovariantPairValuesTableModel extends AbstractTableModel{

   private CovariantPairing pair_;
   
   public CovariantPairValuesTableModel( ) {
        super();
        CovariantPairingsManager manager = CovariantPairingsManager.getInstance();
        manager.registerCovariantValuesTableModel(this);
    }

   public void setPair(CovariantPairing pair) {
      pair_ = pair;
      fireTableDataChanged();
      //update column headings
      fireTableStructureChanged();
   } 
   
   public CovariantPairing getPairing() {
      return pair_;
   }
   
   @Override
   public String getColumnName(int index) {
      if (pair_ == null) {
         return index == 0 ? " " : " ";
      }
      return index == 0 ? pair_.getIndependentName(true) : pair_.getDependentName(true);
   }

   @Override
   public int getRowCount() {
      return pair_ == null ? 0 : pair_.getNumPairings();
   }

   @Override
   public int getColumnCount() {
      return 2;
   }

   @Override
   public boolean isCellEditable(int rowIndex, int colIndex) {
      return true;
   }

   @Override
   public Object getValueAt(int rowIndex, int columnIndex) {    
      return pair_ == null ? null : pair_.getValue(columnIndex, rowIndex);     
   }

   @Override
   public void setValueAt(Object value, int row, int col) {
      if (value instanceof String) {
         pair_.setValue(col, row, (String) value);
      } else {
         pair_.setValue(col, row, (CovariantValue) value);
      }
      //account for resorting of values
      fireTableDataChanged();
   }

   public void updateColumnNames(TableColumnModel columnModel) {
      columnModel.getColumn(0).setHeaderValue(getColumnName(0));
      columnModel.getColumn(1).setHeaderValue(getColumnName(1));      
   }

   
}
