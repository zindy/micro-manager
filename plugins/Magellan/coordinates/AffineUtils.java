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

package coordinates;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
import java.awt.geom.AffineTransform;
import java.util.TreeMap;
import java.util.prefs.Preferences;
import misc.JavaUtils;
import misc.Log;
import org.micromanager.MMStudio;

/**
 *
 * @author Henry
 */
public class AffineUtils {
   
   private static TreeMap<String, AffineTransform> affineTransforms_ = new TreeMap<String,AffineTransform>();
   
   public static String transformToString(AffineTransform transform) {
      double[] matrix = new double[4];
      transform.getMatrix(matrix);
      return matrix[0] +"_"+matrix[1]+"_"+matrix[2]+"_"+matrix[3];
   }
   
   public static AffineTransform stringToTransform(String s) {
      double[] mat = new double[4];
      String[] vals = s.split("_");
      for (int i = 0; i < 4; i ++) {
         mat[i] = Double.parseDouble(vals[i]);
      }
      return new AffineTransform(mat);
   }
   
   
   //called when an affine transform is updated
   public static void transformUpdated(String pixelSizeConfig, AffineTransform transform) {
      if (transform != null) {
         affineTransforms_.put(pixelSizeConfig, transform);
      }
   }
   
   //Only read from preferences one time, so that an inordinate amount of time isn't spent in native system calls
   public static AffineTransform getAffineTransform(String pixelSizeConfig, double xCenter, double yCenter) {
      try {
         AffineTransform transform = null;
         if (affineTransforms_.containsKey(pixelSizeConfig)) {
            transform = affineTransforms_.get(pixelSizeConfig);
            //copy transform so multiple referneces with different translations cause problems
            double[] newMat = new double[6];
            transform.getMatrix(newMat);
            transform = new AffineTransform(newMat);
         } else {
            //Get affine transform from prefs
            Preferences prefs = Preferences.userNodeForPackage(MMStudio.class);
            transform = JavaUtils.getObjectFromPrefs(prefs, "affine_transform_" + pixelSizeConfig, (AffineTransform) null);
            affineTransforms_.put(pixelSizeConfig, transform);
         }
         //set map origin to current stage position
         double[] matrix = new double[6];
         transform.getMatrix(matrix);
         matrix[4] = xCenter;
         matrix[5] = yCenter;
         return new AffineTransform(matrix);
      } catch (Exception ex) {
         Log.log(ex);
         Log.log("Couldnt get affine transform");
         return null;
      }
   }

 
}
