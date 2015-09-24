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

import java.util.ArrayList;
import javax.swing.table.AbstractTableModel;
import surfacesandregions.SurfaceManager;

/**
 *
 * @author Henry
 */
public class EligibleCovariantTableModel extends AbstractTableModel {

   private ArrayList<SinglePropertyOrGroup> propsAndGroups_;
   private ArrayList<SurfaceData> surfaceData_;
   private boolean independent_;
   
   public EligibleCovariantTableModel(boolean includeStats) {
      propsAndGroups_ = PropertyAndGroupUtils.readConfigGroupsAndProperties(includeStats);
      surfaceData_ = includeStats ? SurfaceManager.getInstance().getSurfaceData() : new ArrayList<SurfaceData>();
      independent_ = includeStats;
   }

   @Override
   public String getColumnName(int index) {
      return independent_ ? "Independent variable" : "Dependent variable";
   }

   @Override
   public int getRowCount() {
      return surfaceData_.size() + propsAndGroups_.size();
   }

   @Override
   public int getColumnCount() {
      return 1;
   }

   @Override
   public boolean isCellEditable(int rowIndex, int colIndex) {
      return false;
   }
   
   @Override
   public Object getValueAt(int rowIndex, int columnIndex) {
      if (rowIndex < surfaceData_.size()) {
         return surfaceData_.get(rowIndex);
      }
      return propsAndGroups_.get(rowIndex - surfaceData_.size());
   }
   
   
}
