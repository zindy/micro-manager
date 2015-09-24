/*
 * AUTHOR:
 * Nico Stuurman, Jon Daniels
 *
 * Copyright (c) 2015 Regents of the University of California, & ASI
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

package org.micromanager.asidispim.Data;

import org.micromanager.asidispim.Utils.SliceTiming;

/**
 * Associative container or "plain old data structure" for slice timing information.
 * Public elements so they can be get/set directly, like C/C++ struct
 * Note that this container doesn't work with collections (https://www.artima.com/lejava/articles/equality.html)
 * @author Nico & Jon
 */

public class AcquisitionSettings {
   // piezo scanning, vibration, stage scanning, i.e. what is 
   //                 moved between slices
   public AcquisitionModes.Keys spimMode;
   
   // true iff using stage scanning (for convenience b/c have this in spimMode)
   public boolean isStageScanning;
   
   // whether or not we use time points
   public boolean useTimepoints;
   
   // number of timepoints for this acquisition
   public int numTimepoints;
   
   // time between starts of timepoints in seconds
   public double timepointInterval;
   
   // if true, use multiple positions
   public boolean useMultiPositions;
   
   // if true, use channels
   public boolean useChannels;
   
   // MultiChannel mode
   public MultichannelModes.Keys channelMode;
   
   // number of channels for this acquisition
   public int numChannels;
   
   // array of channels
   public ChannelSpec[] channels;
   
   // channel group
   public String channelGroup;
   
   // whether or not to use autofocus during acquisition
   public boolean useAutofocus;
   
   // number of Sides from which we take data (diSPIM: 1 or 2)
   public int numSides;
   
   // firstSide to take data from (A or B)
   public boolean firstSideIsA;
   
   // wait in ms before starting each side
   public float delayBeforeSide;
   
   // number of slices for this acquisition
   public int numSlices;

   // spacing between slices in microns
   public float stepSizeUm;
   
   // if true, automatically minimize slice period
   public boolean minimizeSlicePeriod;
   
   // requested slice period (only matters if minimizeSlicePeriod_ is false
   public float desiredSlicePeriod;
   
   // requested time for laser to be on each slice
   public float desiredLightExposure;
   
   // true to use current Z position (e.g. autofocus), 
   // false to use specified acquisition center
   public boolean centerAtCurrentZ;
   
   // low level controller timing parameters
   public SliceTiming sliceTiming;
   
   // camera in overlap, edge, etc. mode
   public CameraModes.Keys cameraMode;
   
   // acceleration time of X axis (only applies to stage scanning)
   public float accelerationX;

   // if true, the tiger controller will coordinate multiple timepoints
   // instead of having plugin trigger each one
   // true/false determined by the interval between timpoints vs. overhead
   // to trigger each one separately
   public boolean hardwareTimepoints;
   
}
