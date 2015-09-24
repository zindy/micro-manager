/*
 * Describes a single stage position. The stage can have up to three
 * axes.
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, March 8, 2007
 *
 * Copyright (c) 2007 Regents of the University of California
 * Copyright (c) 2008 100X Imaging Inc
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

package org.micromanager.api;

import org.micromanager.utils.NumberUtils;


public class StagePosition {
   /**
    * NOTE: despite what the name fields in this class may suggest, every 
    * StagePosition in practice has only 1 or 2 axes, and thus only uses either
    * the x, or the x and y fields. That is, the "x" field means "z" when
    * using a 1-axis positioner! 
    */
   public double x;
   public double y;
   public double z;
   public String stageName;
   public int numAxes;
   
   public StagePosition() {
      stageName = "Undefined";
      x = 0.0;
      y = 0.0;
      z = 0.0;
      numAxes=1;
   }
   
   public static StagePosition newInstance(StagePosition aPos) {
      StagePosition sp = new StagePosition();
      sp.x = aPos.x;
      sp.y = aPos.y;
      sp.z = aPos.z;
      sp.numAxes = aPos.numAxes;
      sp.stageName = aPos.stageName;
      return sp;
   }
   
   public String getVerbose() {
      if (numAxes == 1)
         return stageName + "(" + NumberUtils.doubleToDisplayString(x) + ")";
      else if (numAxes == 2)
         return stageName + "(" + NumberUtils.doubleToDisplayString(x) +
      "," + NumberUtils.doubleToDisplayString(y) + ")";
      else
         return stageName + "(" + NumberUtils.doubleToDisplayString(x) +
      "," + NumberUtils.doubleToDisplayString(y) +
      "," + NumberUtils.doubleToDisplayString(z) + ")";

   }
}
