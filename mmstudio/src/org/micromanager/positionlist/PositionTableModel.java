/*
 * AUTHOR:
 * Chris Weisiger
 *
 * Copyright (c) 2014 Regents of the University of California
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

import javax.swing.table.AbstractTableModel;

import org.micromanager.api.MultiStagePosition;
import org.micromanager.api.StagePosition;
import org.micromanager.api.PositionList;

class PositionTableModel extends AbstractTableModel {
   private static final long serialVersionUID = 1L;
   public final String[] COLUMN_NAMES = new String[] {
         "Label",
         "Position [um]"
   };
   private PositionList posList_;
   private MultiStagePosition curMsp_;

   public void setData(PositionList pl) {
      posList_ = pl;
   }

   public PositionList getPositionList() {
      return posList_;
   }

   @Override
   public int getRowCount() {
      return posList_.getNumberOfPositions() + 1;
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
      MultiStagePosition msp;
      if (rowIndex == 0) {
         msp = curMsp_;
      }
      else {
         msp = posList_.getPosition(rowIndex -1);
      }
      if (columnIndex == 0) {
         return msp.getLabel();
      } else if (columnIndex == 1) {
         StringBuilder sb = new StringBuilder();
         for (int i=0; i<msp.size(); i++) {
            StagePosition sp = msp.get(i);
            if (i!=0) {
               sb.append(";");
            }
            sb.append(sp.getVerbose());
         }
         return sb.toString();
      } else {
         return null;
      }
   }
   @Override
   public boolean isCellEditable(int rowIndex, int columnIndex) {
      if (rowIndex == 0) {
         return false;
      }
      if (columnIndex == 0) {
         return true;
      }
      return false;
   }
   @Override
   public void setValueAt(Object value, int rowIndex, int columnIndex) {
      if (columnIndex == 0) {
         MultiStagePosition msp = posList_.getPosition(rowIndex - 1);
         if (msp != null) {
            msp.setLabel(((String) value).replaceAll("[^0-9a-zA-Z_]", "-"));
         }
      }
   }

   public void setCurrentMSP(MultiStagePosition msp) {
      curMsp_ = msp;
   }
}
