/*
 * Copyright (c) 2007, Regents of the University of California
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

#ifndef _PVROI_H_
#define _PVROI_H_

#include "PVCAMIncludes.h"

/***
* User selected region of interest
*/
struct PvRoi {
   uns16 x;
   uns16 newX;
   uns16 y;
   uns16 newY;
   uns16 xSize;
   uns16 newXSize;
   uns16 ySize;
   uns16 newYSize;
   uns16 binXSize;
   uns16 binYSize;

   // added this function to the ROI struct because it only applies to this data structure,
   //  and nothing else.
   void PVCAMRegion(uns16 x_, uns16 y_, uns16 xSize_, uns16 ySize_, \
                    unsigned binXSize_, unsigned binYSize_, rgn_type &newRegion)
   {
      // set to full frame
      x = x_;
      y = y_;
      xSize = xSize_;
      ySize = ySize_;

      // set our member binning information
      binXSize = (uns16) binXSize_;
      binYSize = (uns16) binYSize_;

      // save ROI-related dimentions into other data members
      newX = x/binXSize;
      newY = y/binYSize;
      newXSize = xSize/binXSize;
      newYSize = ySize/binYSize;

      // round the sizes to the proper devisible boundaries
      x = newX * binXSize;
      y = newY * binYSize;
      xSize = newXSize * binXSize;
      ySize = newYSize * binYSize;

      // set PVCAM-specific region
      newRegion.s1 = x;
      newRegion.s2 = x + xSize-1;
      newRegion.sbin = binXSize;
      newRegion.p1 = y;
      newRegion.p2 = y + ySize-1;
      newRegion.pbin = binYSize;
   }
};

#endif // _PVROI_H_
