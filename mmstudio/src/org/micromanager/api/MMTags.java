/*
 * Copyright (c) 2005-2015 Regents of the University of California
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

/**
 * The list of tags used by the Micro-manager
 */
public class MMTags {
   
   /**
    * Meta-tags referring to main sections of the metadata
    */
   public class Root {
      public static final String SUMMARY = "Summary"; // key for the Summary metadata      
   }
   
   /**
    * Summary tags
    */
   public class Summary {
      public static final String PREFIX = "Prefix"; // Acquisition name
      public static final String DIRECTORY = "Directory"; // Acquisition directory

      public static final String WIDTH = "Width"; // image width
      public static final String HEIGHT = "Height"; // image height
      public static final String FRAMES = "Frames"; // number of frames
      public static final String SLICES = "Slices"; // number of (z) slices
      public static final String CHANNELS = "Channels"; // number of channels
      public static final String POSITIONS = "Positions"; // number of positions

      public static final String PIXSIZE = "PixelSize_um";
      public static final String PIX_TYPE = "PixelType";
      public static final String IJ_TYPE = "IJType";
      
      public static final String PIXEL_ASPECT = "PixelAspect";
      public static final String SOURCE = "Source";
      public static final String COLORS = "ChColors";
      public static final String CHANNEL_MINS = "ChContrastMin";
      public static final String CHANNEL_MAXES = "ChContrastMax";
      public static final String NAMES = "ChNames";
      public static final String BIT_DEPTH = "BitDepth";
      public static final String OBJECTIVE_LABEL = "Objective-Label";
      public static final String ELAPSED_TIME = "ElapsedTime-ms";

      public static final String SLICES_FIRST = "SlicesFirst";
      public static final String TIME_FIRST = "TimeFirst";

   }
   
   public class Image {
      public static final String WIDTH = "Width"; // image width
      public static final String HEIGHT = "Height"; // image height
      public static final String CHANNEL = "Channel";
      public static final String FRAME = "Frame";
      public static final String SLICE = "Slice";
      public static final String CHANNEL_INDEX = "ChannelIndex";
      public static final String SLICE_INDEX = "SliceIndex";
      public static final String FRAME_INDEX = "FrameIndex";
      public static final String CHANNEL_NAME = "Channel";
      public static final String POS_NAME = "PositionName";
      public static final String POS_INDEX = "PositionIndex";
      public static final String XUM = "XPositionUm";
      public static final String YUM = "YPositionUm";
      public static final String ZUM = "ZPositionUm";
      public static final String IJ_TYPE = "IJType";
      public static final String TIME = "Time";
      public static final String PIX_TYPE = "PixelType";
      public static final String BIT_DEPTH = "BitDepth";
      public static final String ELAPSED_TIME_MS = "ElapsedTime-ms";
   }
   
   public class Values {
      public static final String PIX_TYPE_GRAY_16 = "GRAY16";
      public static final String PIX_TYPE_GRAY_8 = "GRAY8";
      public static final String PIX_TYPE_RGB_32 = "RGB32";
      public static final String PIX_TYPE_RGB_64 = "RGB64";

      public static final String CHANNEL_DEFAULT = "Default";
   }

}
