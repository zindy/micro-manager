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

import ij.IJ;
import java.util.Arrays;
import java.util.Comparator;
import java.util.TreeSet;
import misc.Log;
import org.apache.commons.math3.geometry.euclidean.twod.Euclidean2D;
import org.apache.commons.math3.geometry.euclidean.twod.Vector2D;
import org.apache.commons.math3.geometry.partitioning.Region;

/**
 *
 * @author Henry
 */
public class SingleResolutionInterpolation {
   
   private final int pixPerInterpPoint_;
   private final boolean[][] interpDefined_;
   private final float[][] interpolation_;
   private final float[][] normals_; //stored in degrees
   private final double boundXMin_, boundXMax_, boundYMin_, boundYMax_;
   private Region<Euclidean2D> convexHullRegion_;
   //for extrapolation
   private TreeSet<Vector2D> convexHullVertices_;
   private Point3d[] allPoints_;
   
   public SingleResolutionInterpolation(int pixPerPoint, boolean[][] defined, float[][] interp, float[][] normals, double boundXMin, double boundXMax, double boundYMin, double boundYMax, 
           Region<Euclidean2D> ch, Vector2D[] convexHullVertices, Point3d[] allPoints ) {
      pixPerInterpPoint_ = pixPerPoint;
      interpDefined_ = defined;
      interpolation_ = interp;      
      normals_ = normals;
      boundXMax_ = boundXMax;
      boundYMax_ = boundYMax;
      boundXMin_ = boundXMin;
      boundYMin_ = boundYMin;
      convexHullRegion_ = ch;
      //keep them sorted for fast searching
      convexHullVertices_ = new TreeSet<Vector2D>(new Comparator<Vector2D>() {
         @Override
         public int compare(Vector2D o1, Vector2D o2) {
            if (o1.getX() != o2.getX()) {
               return o1.getX() < o2.getX() ? -1 : 1;
            }
            if (o1.getY() == o2.getY()) {
               return 0;
            }
            return o1.getY() < o2.getY() ? -1 : 1;
         }
      });
      convexHullVertices_.addAll(Arrays.asList(convexHullVertices));
      allPoints_ = allPoints;
   }
   
   public int getPixelsPerInterpPoint() {
      return pixPerInterpPoint_;
   }
   
 
   /**
    * Get the angle of the surface normal at this point to vertical
    * If extrpolated value, return 0
    * @param x
    * @param y
    * @return 
    */
   public float getNormalAngleToVertical(double x, double y) {
      if (!isInsideConvexHull(x, y)) {
         return 0;
      }
      int numInterpPointsX = normals_[0].length;
      int numInterpPointsY = normals_.length;
      int xIndex = (int) Math.round(((x - boundXMin_) / (boundXMax_ - boundXMin_)) * (numInterpPointsX - 1));
      int yIndex = (int) Math.round(((y - boundYMin_) / (boundYMax_ - boundYMin_)) * (numInterpPointsY - 1));
      if (xIndex >= 0 && yIndex >= 0 && xIndex < normals_[0].length && yIndex < normals_.length) {
         return normals_[yIndex][xIndex];
      }
      return 0;
   }

      public boolean isInterpDefined(double x, double y) {
      if (!isInsideConvexHull(x, y)) {
         return false;
      }
      //try to get the value from the calulated interpolation
      int numInterpPointsX = interpolation_[0].length;
      int numInterpPointsY = interpolation_.length;
      int xIndex = (int) Math.round(((x - boundXMin_) / (boundXMax_ - boundXMin_)) * (numInterpPointsX - 1));
      int yIndex = (int) Math.round(((y - boundYMin_) / (boundYMax_ - boundYMin_)) * (numInterpPointsY - 1));
      if (xIndex >= 0 && yIndex >= 0 && xIndex < interpolation_[0].length && yIndex < interpolation_.length) {
         return interpDefined_[yIndex][xIndex];

      }
      return false;
   }
   
   public float getInterpolatedValue(double x, double y) {
         //try to get the value from the calulated interpolation
         int numInterpPointsX = interpolation_[0].length;
         int numInterpPointsY = interpolation_.length;
         int xIndex = (int) Math.round(((x - boundXMin_) / (boundXMax_ - boundXMin_)) * (numInterpPointsX - 1));
         int yIndex = (int) Math.round(((y - boundYMin_) / (boundYMax_ - boundYMin_)) * (numInterpPointsY - 1));
//         if (xIndex >= 0 && yIndex >= 0 && xIndex < interpolation_[0].length && yIndex < interpolation_.length ){
            return interpolation_[yIndex][xIndex];               
//         }
   }
      
      

   public float getExtrapolatedValue(double x, double y) {
      //find closest convex hull vertex
      Vector2D closest = null;
      double minDistance = Integer.MAX_VALUE;
      for (Vector2D vertex : convexHullVertices_) {
         double distance = vertex.distance(new Vector2D(x, y));
         if (distance < minDistance) {
            minDistance = distance;
            closest = vertex;
         }
      }
      //find 3d point with same xy as convex hull vertex and use its z coordinate
      for (Point3d p : allPoints_) {
         if (closest.equals(new Vector2D(p.x, p.y))) {
            return (float) p.z;
         }
      }
      //if I ever get this error, either the two lists are out of sync or creating vecotrs causes some loss of precision
      IJ.log("Couldn't find 3d point with same XY as convex hull");
      throw new RuntimeException();
   }

   private boolean isInsideConvexHull(double x, double y) {
      if (convexHullRegion_ == null) {
         return false;
      }
      return convexHullRegion_.checkPoint(new Vector2D(x, y)) != Region.Location.OUTSIDE;
   }
}
