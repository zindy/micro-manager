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

import acq.FixedAreaAcquisitionSettings;
import bidc.JavaLayerImageConstructor;
import coordinates.AffineUtils;
import coordinates.XYStagePosition;
import java.awt.geom.AffineTransform;
import java.awt.geom.Point2D;
import java.util.ArrayList;
import main.Magellan;
import misc.Log;

/**
 *
 * @author Henry
 */
public class MultiPosRegion implements XYFootprint{

   private volatile Point2D.Double center_; //stored in stage space
   private volatile int overlapX_, overlapY_, rows_, cols_;
   private RegionManager manager_;
   private String name_;
   private String pixelSizeConfig_;
   private String XYDevice_;

   public MultiPosRegion(RegionManager manager, String xyDevice, int r, int c, Point2D.Double center)  {
      manager_ = manager;
      name_ = manager.getNewName();
      center_ = center;
      XYDevice_ = xyDevice;
      try {
         pixelSizeConfig_ = Magellan.getCore().getCurrentPixelSizeConfig();
      } catch (Exception ex) {
         Log.log("couldnt get pixel size config");
      }
      updateParams(r, c);
   }
   
   public String getXYDevice() {
      return XYDevice_;
   }

   @Override
   public String toString() {
      return name_;
   }

   public String getName() {
      return name_;
   }

   public void rename(String newName) {
      name_ = newName;
      manager_.updateRegionTableAndCombos();
   }

   public double getWidth_um() {
      double pixelSize = Magellan.getCore().getPixelSizeUm();
      int pixelWidth = (int) (cols_ * (JavaLayerImageConstructor.getInstance().getImageWidth() - overlapX_) + overlapX_);
      return pixelSize * pixelWidth;
   }

   public double getHeight_um() {
      double pixelSize = Magellan.getCore().getPixelSizeUm();
      int imageHeight = (int) Magellan.getCore().getImageHeight();
      int pixelHeight = rows_ * (imageHeight - overlapY_) + overlapY_;
      return pixelSize * pixelHeight;
   }

   public void updateParams(int rows, int cols) {
      updateOverlap(FixedAreaAcquisitionSettings.getStoredTileOverlapPercentage() / 100);
      rows_ = rows;
      cols_ = cols;
      manager_.updateRegionTableAndCombos();
      manager_.drawRegionOverlay(this);
   }
   
   private void updateOverlap(double overlapPercent) {
      overlapX_ = (int) (JavaLayerImageConstructor.getInstance().getImageWidth() * overlapPercent);
      overlapY_ = (int) (JavaLayerImageConstructor.getInstance().getImageHeight() * overlapPercent);
   }

   @Override
   public ArrayList<XYStagePosition> getXYPositions(double tileOverlapPercent) {
      try {
         AffineTransform transform = AffineUtils.getAffineTransform(Magellan.getCore().getCurrentPixelSizeConfig(), center_.x, center_.y);
         ArrayList<XYStagePosition> positions = new ArrayList<XYStagePosition>();
         int fullTileWidth = (int) JavaLayerImageConstructor.getInstance().getImageWidth();
         int fullTileHeight = (int) JavaLayerImageConstructor.getInstance().getImageHeight();
         updateOverlap(tileOverlapPercent / 100);
         int tileWidthMinusOverlap = fullTileWidth - overlapX_;
         int tileHeightMinusOverlap = fullTileHeight - overlapY_;
         for (int col = 0; col < cols_; col++) {
            double xPixelOffset = (col - (cols_ - 1) / 2.0) * tileWidthMinusOverlap;
            for (int row = 0; row < rows_; row++) {
               double yPixelOffset = (row - (rows_ - 1) / 2.0) * tileHeightMinusOverlap;
               Point2D.Double pixelPos = new Point2D.Double(xPixelOffset, yPixelOffset);
               Point2D.Double stagePos = new Point2D.Double();
               transform.transform(pixelPos, stagePos);
               AffineTransform posTransform = AffineUtils.getAffineTransform(pixelSizeConfig_, stagePos.x, stagePos.y);
               positions.add(new XYStagePosition(stagePos, tileWidthMinusOverlap, tileHeightMinusOverlap,
                       fullTileWidth, fullTileHeight, row, col, posTransform));
            }
         }
         return positions;
      } catch (Exception ex) {
         Log.log("Couldn't get affine transform");
         throw new RuntimeException();
      }
   }

   public int numCols() {
      return cols_;
   }

   public int numRows() {
      return rows_;
   }

   public int overlapX() {
      return overlapX_;
   }

   public int overlapY() {
      return overlapY_;
   }

   public Point2D.Double center() {
      return center_;
   }

   public void translate(double dx, double dy) {
      center_ = new Point2D.Double(center_.x + dx, center_.y + dy);
      manager_.drawRegionOverlay(this);
   }
}
