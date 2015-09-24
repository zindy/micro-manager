/*
 * AUTHOR:
 * Henry Pinkard, henry.pinkard@gmail.com Dec 1, 2011
 *
 * Copyright (c) 2011 Regents of the University of California
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

package mmcloneclasses.graph;

import acq.MMImageCache;


/**
 * Interface for histograms, which control Image contrast and can optionally
 * draw themselves
 * @author Henry Pinkard
 */
public interface Histograms  {

    /**
     * Creates and applies a look up table to the image based on the previously
     * set values of contrast min, max and gamma.  Also stores min max and gamma
     * in the image cache and redraws the histogram contrast cursors
     */
   public void applyLUTToImage();

   /**
    * Called just before the image is about to draw itself.  Calls 
    * calcAndDisplayHistAndStats and applyLutToImage to make sure that the image has
    * the correct LUT and that the histogram is in sync with the image
    */
   public void imageChanged();

   /**
    * Manually set the contrast min, max and gamma values for this channel (channel = 0)
    * for single channel contrast panel).  Does not redraw image or histogram
    * @param channelIndex Index of the channel to set the contrast of
    * @param min Contrast min
    * @param max Contrast max
    * @param gamma Contrast gamma
    */
   public void setChannelContrast(int channelIndex, int min, int max, double gamma);
   
   /**
    * Calculates the image histogram and optionally displays image statistics
    * (pixel min, max, mean, etc) and draws the histogram)
    * @param drawHist flag for optional display of statistics and drawing of histogram
    */
   public void calcAndDisplayHistAndStats(boolean drawHist);
   
   /*
    * Sets the contrast min and max to the stored pixel min and max values
    * (or to the appropriate values if reject outliers is selected).  Does
    * not calculate histogram or redraw image
    */
   public void autostretch();
      
   /*
    * sets the histogram display range fropdown box to the option corresponding to the
    * value histMax or to the "Camera Depth" automatic option if histMax is equal to -1
    */
   public void setChannelHistogramDisplayMax(int channelIndex, int histMax);
   
   /*
    * Called when a change in the reject outliers checkbox of reject percent spinner occurs
    * to redraw the histogram and image appropriately
    */
   public void rejectOutliersChangeAction();    

   /**
    * Equivalent to pressing the Auto button for each channel
    */
   public void autoscaleAllChannels() ;

   /*
    * Initializes all channel controls.
    */
   public void setupChannelControls(MMImageCache cache, ContrastPanel cp);

   public int getNumberOfChannels();

}
