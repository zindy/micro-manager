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

import javax.swing.DefaultComboBoxModel;

/**
 *
 * @author Henry
 */
public class SurfaceRegionComboBoxModel extends DefaultComboBoxModel {
   
   private RegionManager rManager_;
   private SurfaceManager sManager_;
   private int selectedIndex_ = -1;

   public SurfaceRegionComboBoxModel(SurfaceManager sManager, RegionManager rManager)  {
      sManager_ = sManager;
      rManager_ = rManager;
   }

   public int getSelectedIndex() {
      return selectedIndex_;
   }

   public void setSelectedIndex(int selectedIndex) {
      selectedIndex_ = selectedIndex;
   }
   
   @Override
   public void setSelectedItem(Object anItem) {
     selectedIndex_ = -1;
     int offset = 0;
      if (rManager_ != null) {
         offset = rManager_.getNumberOfRegions();
         for (int i = 0; i < rManager_.getNumberOfRegions(); i++) {
            if (rManager_.getRegion(i).equals(anItem)) {
               selectedIndex_ = i;
               return;
            }
         }
      }
      if (sManager_ != null) {
         for (int i = 0; i < sManager_.getNumberOfSurfaces(); i++) {
            if (sManager_.getSurface(i).equals(anItem)) {
               selectedIndex_ = i + offset;
               return;
            }
         }
      }
   }

   @Override
   public Object getSelectedItem() {
      return getElementAt(selectedIndex_);
   }

   @Override
   public int getSize() {
      return (rManager_ != null ? rManager_.getNumberOfRegions() : 0) + (sManager_ != null ? sManager_.getNumberOfSurfaces() : 0); 
   }

   @Override
   public Object getElementAt(int index) {
      if (index == -1) {
         return null;
      }
      if (rManager_ == null) {
         //surfaces only
         return sManager_.getSurface(index);
      } else if (sManager_ == null) {
         return rManager_.getRegion(index);
      } else {
         //regions first, then surfaces
         if (index >= rManager_.getNumberOfRegions()) {
            return sManager_.getSurface(index - rManager_.getNumberOfRegions());
         } else {
            return rManager_.getRegion(index);
         }
      }
   }

   public void update() {
      if (sManager_ != null) {
         super.fireContentsChanged(sManager_, -1, -1);
      }
      if (rManager_ != null) {
         super.fireContentsChanged(rManager_, -1, -1);
      }
   }
   
}
