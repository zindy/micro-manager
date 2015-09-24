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

import acq.MultipleAcquisitionManager;
import javax.swing.table.AbstractTableModel;

/**
 *
 * @author Henry
 */
public class CovariantPairingsTableModel extends AbstractTableModel {
   
   CovariantPairingsManager manager_;
   
   public CovariantPairingsTableModel( ) {
       super();
      manager_ = CovariantPairingsManager.getInstance();
      manager_.registerCovariantPairingsTableModel(this);
   }
   
    @Override
   public String getColumnName(int index) {
      return index == 0 ? "Active" : "Pairing (Independent Variable : Dependent Variable)";
   }

   @Override
   public int getRowCount() {
      return manager_.getNumPairings();
   }

   @Override
   public int getColumnCount() {
      return 2;
   }
   
   @Override
   public Class getColumnClass(int column) {
      return column == 0 ? Boolean.class : String.class;
   }

   @Override
   public boolean isCellEditable(int rowIndex, int colIndex) {
      return colIndex == 0;
   }

   @Override
   public Object getValueAt(int rowIndex, int columnIndex) {    
      if (rowIndex == -1) {
         return null;
      } else if (columnIndex == 0) {
         return manager_.isPairActiveForCurrentAcq(rowIndex);
      }  else {
         return manager_.getPair(rowIndex);
      }
   }

   @Override
   public void setValueAt(Object value, int row, int col) {
      if (col == 0) {
         manager_.enablePairingForCurrentAcq(row, (Boolean) value);
      } 
   }

   public boolean isAnyPairingActive() {
      for (int i = 0; i < manager_.getNumPairings(); i++) {
         if (manager_.isPairActiveForCurrentAcq(i) ) {
            return true;
         }
      }
      return false;
   }
}
