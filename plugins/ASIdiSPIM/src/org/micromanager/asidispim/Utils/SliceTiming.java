/*
 * AUTHOR:
 * Nico Stuurman, Jon Daniels
 *
 * Copyright (c) 2014 Regents of the University of California, & ASI
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


/**
 * Associative container or "plain old data structure" for slice timing information.
 * Public elements so they can be get/set directly, like C/C++ struct
 * Note that this container doesn't work with collections (https://www.artima.com/lejava/articles/equality.html)
 * @author Jon
 *
 */
public class SliceTiming {
   public float scanDelay;
   public int scanNum;
   public int scanPeriod;
   public float laserDelay;
   public float laserDuration;
   public float cameraDelay;
   public float cameraDuration;
   public float cameraExposure;  // used to set exposure in Micro-Manager, not the controller timing
   public float sliceDuration;   // depends on first 7 values by formula, up to users to keep updated

   /**
    * Chooses some reasonable defaults (may not be controller defaults).
    */
   public SliceTiming() {
      scanDelay = 0;
      scanNum = 1;
      scanPeriod = 10;
      laserDelay = 0;
      laserDuration = 1;
      cameraDelay = 0;
      cameraDuration = 1;
      cameraExposure = 1;
      sliceDuration = 2;
   }
   
   /**
    * copy constructor (unused?)
    */
   public SliceTiming(SliceTiming orig) {
      scanDelay = orig.scanDelay;
      scanNum = orig.scanNum;
      scanPeriod = orig.scanPeriod;
      laserDelay = orig.laserDelay;
      laserDuration = orig.laserDuration;
      cameraDelay = orig.cameraDelay;
      cameraDuration = orig.cameraDuration;
      cameraExposure = orig.cameraExposure;
      sliceDuration = orig.sliceDuration;
   }
   
   @Override
   public boolean equals(Object obj) {
      if ((obj instanceof SliceTiming)) {
         SliceTiming s = (SliceTiming) obj;
         return(scanDelay == s.scanDelay
               && scanNum == s.scanNum
               && scanPeriod == s.scanPeriod
               && laserDelay == s.laserDelay
               && laserDuration == s.laserDuration
               && cameraDelay == s.cameraDelay
               && cameraDuration == s.cameraDuration
               && cameraExposure == s.cameraExposure
               && sliceDuration == s.sliceDuration);
      } else {
         return false;
      }
      
      
   }

}
