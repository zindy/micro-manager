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

package imagedisplay;

import acq.MMImageCache;
import acq.MagellanTaggedImage;
import ij.ImagePlus;
import ij.process.ByteProcessor;
import ij.process.ColorProcessor;
import ij.process.FloatProcessor;
import ij.process.ImageProcessor;
import ij.process.ShortProcessor;
import java.awt.image.ColorModel;
import json.JSONObject;
import misc.Log;
import misc.MD;

/**
 * This stack class provides the ImagePlus with images from the MMImageCache.
 * 
 */
public class AcquisitionVirtualStack extends ij.VirtualStack {

   final private MMImageCache imageCache_;
   final protected VirtualAcquisitionDisplay vad_;
   final protected int width_, height_, type_;
   private final int nSlices_;
   private int positionIndex_ = 0;

   public AcquisitionVirtualStack(int width, int height, int type,
           ColorModel cm, MMImageCache imageCache, int nSlices,
           VirtualAcquisitionDisplay acq) {
      super(width, height, cm, "");
      imageCache_ = imageCache;
      width_ = width;
      height_ = height;
      nSlices_ = nSlices;

      vad_ = acq;
      type_ = type;
   }

   public void setPositionIndex(int pos) {
      positionIndex_ = pos;
   }

   public int getPositionIndex() {
      return positionIndex_;
   }

   public VirtualAcquisitionDisplay getVirtualAcquisitionDisplay() {
      return vad_;
   }

   
   private MagellanTaggedImage getMagellanTaggedImage(int flatIndex) {
      int[] pos;
      // If we don't have the ImagePlus yet, then we need to assume
      // we are on the very first image.
      ImagePlus imagePlus = vad_.getImagePlus();
      if (imagePlus == null) {
         return getMagellanTaggedImage(0,0,0);
      } else {
         pos = imagePlus.convertIndexToPosition(flatIndex);
      }
      int chanIndex = vad_.grayToRGBChannel(pos[0] - 1);
      int frame = pos[2] - 1;
      int slice = pos[1] - 1;

      return getMagellanTaggedImage(chanIndex, slice, frame);
   }

   //This method is the ultimate source of tagged images/metadata to update the display, but has no
   //relevance to image data on disk. It is protected so that this class can be overriden and a differnet image
   //used for display compared to the the underlying data
   protected MagellanTaggedImage getMagellanTaggedImage(int chanIndex, int slice, int frame) {
      int nSlices;
      ImagePlus imagePlus = vad_.getImagePlus();
      if (imagePlus == null) {
         nSlices = 1;
      } else {
         nSlices = imagePlus.getNSlices();
      }
      try {
         MagellanTaggedImage img;
         img = imageCache_.getImage(chanIndex, slice, frame, positionIndex_);
         int backIndex = slice - 1, forwardIndex = slice + 1;
         int frameSearchIndex = frame;
         //If some but not all channels have z stacks, find the closest slice for the given
         //channel that has an image.  Also if time point missing, go back until image is found
         while (img == null) {
            img = imageCache_.getImage(chanIndex, slice, frameSearchIndex, positionIndex_);
            if (img != null) {
               break;
            }

            if (backIndex >= 0) {
               img = imageCache_.getImage(chanIndex, backIndex, frameSearchIndex, positionIndex_);
               if (img != null) {
                  break;
               }
               backIndex--;
            }
            if (forwardIndex < nSlices) {
               img = imageCache_.getImage(chanIndex, forwardIndex, frameSearchIndex, positionIndex_);
               if (img != null) {
                  break;
               }
               forwardIndex++;
            }

            if (backIndex < 0 && forwardIndex >= nSlices) {
               frameSearchIndex--;
               backIndex = slice - 1;
               forwardIndex = slice + 1;
               if (frameSearchIndex < 0) {
                  break;
               }
            }
         }

         return img;
      } catch (Exception e) {
         Log.log(e);
         return null;
      }
   }
   
   //this method is available so that image tags can be synchrnized with the pixels displayed in the viewer,
   //since alternate images are filled in when some are missing (for example, when a z stack is not collecte din one channel
   //or when frames are skipped)
   public JSONObject getImageTags(int flatIndex) {
      MagellanTaggedImage img = getMagellanTaggedImage(flatIndex);
      if (img == null) {
         return null;
      }
      return img.tags;
      
   }

   @Override
   public Object getPixels(int flatIndex) {
      Object pixels = null;
      MagellanTaggedImage image = getMagellanTaggedImage(flatIndex);
      if (image == null || image.pix == null) {
         pixels = makeProcessor(type_, width_, height_).getPixels();
      } else if (image != null && image.tags == null) {
         pixels = image.pix;
      } else if (MD.isGRAY(image.tags)) {
         pixels = image.pix;
      }
//         else if (MD.isRGB32(image)) {
//            pixels = ImageUtils.singleChannelFromRGB32((byte[]) image.pix, (flatIndex - 1) % 3);
//         } else if (MD.isRGB64(image)) {
//            pixels = ImageUtils.singleChannelFromRGB64((short[]) image.pix, (flatIndex - 1) % 3);
//         }

      return pixels;
   }

   @Override
   public ImageProcessor getProcessor(int flatIndex) {
      return makeProcessor(type_, width_, height_, getPixels(flatIndex));
   }

   @Override
   public int getSize() {
      // returns the stack size of VirtualAcquisitionDisplay unless this size is -1
      // which occurs in constructor while hyperImage_ is still null. In this case
      // returns the number of slices speciefiec in AcquisitionVirtualStack constructor
      int size = vad_.getStackSize();
      if (size == -1) {
         return nSlices_;
      }
      return size;
   }

   @Override
   public String getSliceLabel(int n) {
      MagellanTaggedImage img = getMagellanTaggedImage(n);
      if (img == null) {
         return "";
      }
      JSONObject md = img.tags;
      try {
         return md.get("Acquisition-PixelSizeUm") + " um/px";
         //return MD.getChannelName(md) + ", " + md.get("Acquisition-ZPositionUm") + " um(z), " + md.get("Acquisition-TimeMs") + " s";
      } catch (Exception ex) {
         return "";
      }
   }
   
   public static ImageProcessor makeProcessor(int type, int w, int h, Object imgArray) {
      if (imgArray == null) {
         return makeProcessor(type, w, h);
      } else {
         switch (type) {
            case ImagePlus.GRAY8:
               return new ByteProcessor(w, h, (byte[]) imgArray, null);
            case ImagePlus.GRAY16:
               return new ShortProcessor(w, h, (short[]) imgArray, null);
            case ImagePlus.GRAY32:
               return new FloatProcessor(w,h, (float[]) imgArray, null);
            case ImagePlus.COLOR_RGB:
               // Micro-Manager RGB32 images are generally composed of byte
               // arrays, but ImageJ only takes int arrays.
               if (imgArray instanceof byte[]) {
                  imgArray = convertRGB32BytesToInt((byte[]) imgArray);
               }
               return new ColorProcessor(w, h, (int[]) imgArray);
            default:
               return null;
         }
      }
   }
   
   public static ImageProcessor makeProcessor(int type, int w, int h) {
      if (type == ImagePlus.GRAY8) {
         return new ByteProcessor(w, h);
      } else if (type == ImagePlus.GRAY16) {
         return new ShortProcessor(w, h);
      } else if (type == ImagePlus.GRAY32) {
         return new FloatProcessor(w,h);
      } else if (type == ImagePlus.COLOR_RGB) {
         return new ColorProcessor(w, h);
      } else {
         return null;
      }
   }

   public static int[] convertRGB32BytesToInt(byte[] pixels) {
      int[] ints = new int[pixels.length / 4];
      for (int i = 0; i < ints.length; ++i) {
         ints[i] = pixels[4 * i]
                 + (pixels[4 * i + 1] << 8)
                 + (pixels[4 * i + 2] << 16);
      }
      return ints;
   }
}

