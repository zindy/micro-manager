/*
 * AUTHOR:
 * Nico Stuurman, Jon Daniels
 *
 * Copyright (c) 2013 Regents of the University of California, & ASI
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

package org.micromanager.asidispim.Utils;

import org.apache.commons.math3.util.Precision;


/**
 * @author Jon
 */
public class MyNumberUtils {
   
   public MyNumberUtils() {
   }
   
   
   /**
    * Does "equality" test on floats using commons-math3 library
    * and epsilon of 10*maxUlps
    * (before r14313 used locally-defined epsilon of 1e-12)
    * @param f1
    * @param f2
    * @return
    */
   public static boolean floatsEqual(float f1, float f2) {
      return Precision.equals(f1, f2, 10);
   }
   
   /**
    * "rounds up" to nearest increment of 0.25, e.g. 0 goes to 0 but 0.01 goes to 0.25
    * @param f
    * @return
    */
   public static float ceilToQuarterMs(float f) {
      return (float) (Math.ceil(f*4)/4);
   }
   
   /**
    * "rounds up" to nearest increment of 0.25
    * @param f
    * @return
    */
   public static float roundToQuarterMs(float f) {
      return ((float) Math.round(f*4))/4;
   }
   
   
}
