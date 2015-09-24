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

import imagedisplay.DisplayPlus;
import java.util.ArrayList;
import propsandcovariants.CovariantPairingsManager;

/**
 *
 * class to keep track of the surfaces/regions
 */
public class RegionManager {

   private ArrayList<MultiPosRegion> regions_ = new ArrayList<MultiPosRegion>();
   private ArrayList<SurfaceRegionComboBoxModel> comboBoxModels_ = new ArrayList<SurfaceRegionComboBoxModel>();
   private RegionTableModel tableModel_;
   private static RegionManager singletonInstance_;
   
   public RegionManager() {
      singletonInstance_ = this;
   }
   
   public static RegionManager getInstance() {
      return singletonInstance_;
   }
   
   public int getIndex(SurfaceInterpolator surface) {
      return regions_.indexOf(surface);
   }
   
   public MultiPosRegion getRegion(int index) {
      if (index < 0 || index >= regions_.size()) {
         return null;
      }
      return regions_.get(index);
   }

   public RegionTableModel createGridTableModel() {
      tableModel_ = new RegionTableModel(this);
      return tableModel_;
   }

   public void addToModelList(SurfaceRegionComboBoxModel model) {
      comboBoxModels_.add(model);
   }
   
   public void removeFromModelList(SurfaceRegionComboBoxModel model) {
      comboBoxModels_.remove(model);
   }

   public void deleteAll() {
      regions_.clear();
      for (SurfaceRegionComboBoxModel combo : comboBoxModels_) {
         combo.setSelectedIndex(-1);
      }
      updateRegionTableAndCombos();
   }
   
   public void delete(int index) {
      regions_.remove(index);
      for (SurfaceRegionComboBoxModel combo : comboBoxModels_) {
         if (index == 0 && regions_.isEmpty()) {
            combo.setSelectedIndex(-1); //set selectionto null cause no surfaces left
         } else if (combo.getSelectedIndex() == 0) {
            //do noting, so selection stays at top of list
         } else if (index <= combo.getSelectedIndex()) {
            combo.setSelectedIndex(combo.getSelectedIndex() - 1); //decrment selection so combo stays on same object
         }
      }
      updateRegionTableAndCombos();
   }
   
   public void addNewRegion(MultiPosRegion region) {
      regions_.add(region);
      updateRegionTableAndCombos();
   }
   
   public int getNumberOfRegions() {
      return regions_.size();
   }
  
   public String getNewName() {
      String base = "New Region";
      int index = 1;
      String potentialName = base + " " + index;
      while (true) {
         boolean uniqueName = true;
         for (MultiPosRegion region : regions_) {
            if (region.getName().equals(potentialName)) {
               index++;
               potentialName = base + " " + index;
               uniqueName = false;
            }
         }
         if (uniqueName) {
            break;
         }
      }
      return potentialName;
   }

   /**
    * redraw overlay for all displays showing this surface
    * @param region 
    */
   public void drawRegionOverlay(MultiPosRegion region) {
      DisplayPlus.redrawRegionOverlay(region);
   }
   
   public void updateRegionTableAndCombos() {
      for (SurfaceRegionComboBoxModel m : comboBoxModels_) {
         m.update();
      }
      tableModel_.fireTableDataChanged();
      CovariantPairingsManager.getInstance().surfaceorRegionNameChanged();
   }
}
