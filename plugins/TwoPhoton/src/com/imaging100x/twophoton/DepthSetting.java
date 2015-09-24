/*
 * AUTHOR:
 * Nenad Amodaj
 *
 * Copyright (c) 2011 Nenad Amodaj
 * Copyright (c) 2009 100X Imaging Inc
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.imaging100x.twophoton;

public class DepthSetting implements Comparable<DepthSetting> {
   public double eomVolts1_;
   public double eomVolts2_;
   public PMTSetting pmts[];
   public double deltaZ;
   public double z;
   
   public DepthSetting() {
      eomVolts1_ = 0.0;
      eomVolts2_ = 0.0;
      deltaZ = 0.0;
      z = 0.0;
      pmts = new PMTSetting[0]; 
   }
   
   public void resizePMT(int size) {
      pmts = new PMTSetting[size];
      for (int i=0; i<size; i++)
         pmts[i] = new PMTSetting();
   }

   public int compareTo(DepthSetting ds) {
      if (ds.z > z)
         return 1;
      else if (ds.z < z)
         return -1;
         
      return 0;
   }

}
