/*
 * AUTHOR:
 * Henry Pinkard, henry.pinkard@gmail.com, 2012
 *
 * Copyright (c) 2012 Regents of the University of California
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

package org.micromanager.utils;

import java.util.Comparator;

public class ImageLabelComparator implements Comparator<String> {

   private final boolean slicesFirst_;
   private final boolean timeFirst_;

   public ImageLabelComparator() {
      this(false, false);
   }

   public ImageLabelComparator(boolean slicesFirst, boolean timeFirst) {
      super();
      slicesFirst_ = slicesFirst;
      timeFirst_ = timeFirst;
   }

   public boolean getSlicesFirst() {
      return slicesFirst_;
   }

   public boolean getTimeFirst() {
      return timeFirst_;
   }

   @Override
   public int compare(String s1, String s2) {
      //c_s_f_p
      String[] indices1 = s1.split("_");
      String[] indices2 = s2.split("_");
      if (timeFirst_) {
         int position1 = Integer.parseInt(indices1[3]), position2 = Integer.parseInt(indices2[3]);
         if (position1 != position2) {
            return position1 - position2;
         }
         int frame1 = Integer.parseInt(indices1[2]), frame2 = Integer.parseInt(indices2[2]);
         if (frame1 != frame2) {
            return frame1 - frame2;
         }
      } else {
         int frame1 = Integer.parseInt(indices1[2]), frame2 = Integer.parseInt(indices2[2]);
         if (frame1 != frame2) {
            return frame1 - frame2;
         }
         int position1 = Integer.parseInt(indices1[3]), position2 = Integer.parseInt(indices2[3]);
         if (position1 != position2) {
            return position1 - position2;
         }
      }
      if (slicesFirst_) {
         int channel1 = Integer.parseInt(indices1[0]), channel2 = Integer.parseInt(indices2[0]);
         if (channel1 != channel2) {
            return channel1 - channel2;
         }
         return Integer.parseInt(indices1[1]) - Integer.parseInt(indices2[1]);
      } else {
         int slice1 = Integer.parseInt(indices1[1]), slice2 = Integer.parseInt(indices2[1]);
         if (slice1 != slice2) {
            return slice1 - slice2;
         }
         return Integer.parseInt(indices1[0]) - Integer.parseInt(indices2[0]);
      }
   }
}
