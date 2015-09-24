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

package surfacesandregions;

import javax.swing.event.ListDataEvent;
import javax.swing.event.ListDataListener;
import javax.swing.table.AbstractTableModel;
import surfacesandregions.MultiPosRegion;
import surfacesandregions.RegionManager;

/**
 *
 * @author Henry
 */
class RegionTableModel extends AbstractTableModel implements ListDataListener {

   private final String[] COLUMNS = {"Name", "XY Device", "# Rows", "# Cols", "Width (µm)", "Height (µm)"};
   private RegionManager manager_;
   
   public RegionTableModel(RegionManager manager) {
      manager_ = manager;
   }
   
   @Override
   public int getRowCount() {
      return manager_.getNumberOfRegions();
   }
   
   @Override
   public String getColumnName(int index) {
      return COLUMNS[index];
   }

   @Override
   public int getColumnCount() {
      return COLUMNS.length;
   }

   @Override
   public boolean isCellEditable(int rowIndex, int colIndex) {
      if (colIndex == 0) {
         return true;
      }
      return false;
   }
   
   @Override 
   public void setValueAt(Object value, int row, int col) {
      if (col == 0) {
         manager_.getRegion(row).rename((String) value);
      }
   }
   
   @Override
   public Object getValueAt(int rowIndex, int columnIndex) {
      MultiPosRegion region = manager_.getRegion(rowIndex);
      if (columnIndex == 0) {
         return manager_.getRegion(rowIndex).getName();
      } else if (columnIndex == 1) {
         return manager_.getRegion(rowIndex).getXYDevice();
      } else if (columnIndex == 2) {
         return region.numRows();
      } else if (columnIndex == 3) {
         return region.numCols();
      } else if (columnIndex == 4) {
         return region.getWidth_um();
      } else {
         return region.getHeight_um();
      }
   }

   @Override
   public void intervalAdded(ListDataEvent e) {
      this.fireTableDataChanged();
   }

   @Override
   public void intervalRemoved(ListDataEvent e) {
      this.fireTableDataChanged();
   }

   @Override
   public void contentsChanged(ListDataEvent e) {
      this.fireTableDataChanged();
   }
   
}
