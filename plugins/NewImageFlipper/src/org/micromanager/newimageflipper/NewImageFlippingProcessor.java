/*
 * AUTHOR:
 * Arthur Edelstein, Nico Stuurman
 *
 * Copyright (c) 2011, 2012 Regents of the University of California
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

package org.micromanager.newimageflipper;

import ij.ImagePlus;
import ij.process.ImageProcessor;
import mmcorej.TaggedImage;
import org.json.JSONException;
import org.json.JSONObject;
import org.micromanager.MMStudio;
import org.micromanager.acquisition.TaggedImageQueue;
import org.micromanager.api.DataProcessor;
import org.micromanager.utils.ImageUtils;
import org.micromanager.utils.MDUtils;
import org.micromanager.utils.MMScriptException;
import org.micromanager.utils.ReportingUtils;

public class NewImageFlippingProcessor extends DataProcessor<TaggedImage> {

   static public enum Rotation {

      R0, R90, R180, R270
   }
   String camera_;
   boolean isMirrored_;
   Rotation rotation_;
   NewImageFlipperControls controls_;

   /**
    * Polls for tagged images, and processes them if they are from the selected 
    * camera.
    * 
    */
   @Override
   public void process() {
      try {
         TaggedImage nextImage = poll();
         if (nextImage != TaggedImageQueue.POISON) {
            try {
               String camera = nextImage.tags.getString("Core-Camera");
               if (!camera.equals(camera_)) {
                  if (nextImage.tags.has("Camera")) {
                     camera = nextImage.tags.getString("Camera");
                  }
               }
               if (!camera.equals(camera_)) {
                  produce(nextImage);
                  return;

               }

               produce(proccessTaggedImage(nextImage, isMirrored_,
                       rotation_));

            } catch (JSONException ex) {
               produce(TaggedImageQueue.POISON);
               ReportingUtils.logError(ex);
            } catch (MMScriptException ex) {
               produce(TaggedImageQueue.POISON);
               ReportingUtils.logError(ex);
            }
         } else {
            // Must produce Poison (sentinel) image to terminate tagged image pipeline
            produce(nextImage);
         }
      } catch (Exception ex) {
         ReportingUtils.logError(ex);
      }
   }

   /**
    * Executes image transformation
    * First mirror the image if requested, than rotate as requested
    * 
    * @param nextImage - TaggedImage to be transformed
    * @param mirror - Whether or not to mirror
    * @param rotation - Rotation (R0, R90, R180, R270)
    * @return - Transformed tagged image, otherwise a copy of the input
    * @throws JSONException
    * @throws MMScriptException 
    */
   public static TaggedImage proccessTaggedImage(TaggedImage nextImage,
           boolean mirror, Rotation rotation) throws JSONException, MMScriptException {

      int width = MDUtils.getWidth(nextImage.tags);
      int height = MDUtils.getHeight(nextImage.tags);
      // Since the Micro-Manager ImageCache can not handle images of differing sizes
      // make sure that the produced image has the same width and height as the 
      // original.
      if ( (width != height) && (rotation == Rotation.R90 ||
              rotation == Rotation.R270) ) {
         throw new MMScriptException("NewImageFlipper: Flipping would produce"
                 + " image of different size which can not be handled by Micro-Manager");
      }
      
      
      String type = MDUtils.getPixelType(nextImage.tags);
      int ijType = ImagePlus.GRAY8;
      if (type.equals("GRAY16")) {
         ijType = ImagePlus.GRAY16;
      }

      ImageProcessor proc = ImageUtils.makeProcessor(ijType, width, height, nextImage.pix);

      if (mirror) {
         proc.flipHorizontal();
      }
      int rotationTag = 0;
      if (rotation == Rotation.R90) {
         proc = proc.rotateRight();
         rotationTag = 90;
      }
      if (rotation == Rotation.R180) {
         proc = proc.rotateRight();
         proc = proc.rotateRight();
         rotationTag = 180;
      }
      if (rotation == Rotation.R270) {
         proc = proc.rotateLeft();
         rotationTag = 270;
      }
      JSONObject newTags = nextImage.tags;
      newTags.put("ImageFlipper-Rotation", rotationTag);
      newTags.put("ImageFlipper-Mirror", mirror ? "On" : "Off");
      
      // make sure that the produced image has the same width and height as the 
      // original.
      if ( (width != proc.getWidth()) || (height != proc.getHeight()) ) { 
         throw new MMScriptException("NewImageFlipper: Flipping would produce"
                 + " image of different size which can not be handled by Micro-Manager");
      }
      MDUtils.setWidth(newTags, proc.getWidth()); 
      MDUtils.setHeight(newTags, proc.getHeight());

      return new TaggedImage(proc.getPixels(), newTags);
   }

   /**
    * Update which camera is to be processed.
    * @param camera - Camera name
    */
   public void setCamera(String camera) {
      camera_ = camera;
   }


   /**
    * Update the rotation parameter.
    * @param rotation - Rotation (R0, R90, R180, R270)
    */
   public void setRotation(Rotation rotation) {
      rotation_ = rotation;
   }

   /**
    * Update the mirroring.
    * @param isMirrored - true if image should be mirrored.
    */
   public void setIsMirrored(boolean isMirrored) {
      isMirrored_ = isMirrored;
   }

   /** 
    * Generate the configuration UI for this processor.
    */
   @Override
   public void makeConfigurationGUI() {
      if (controls_ == null) {
         controls_ = new NewImageFlipperControls(this);
         MMStudio.getInstance().addMMBackgroundListener(controls_);
      }
      else {
         controls_.updateCameras();
      }
      controls_.setVisible(true);
   }

   /** 
    * Dispose of the GUI generated by this processor.
    */
   @Override
   public void dispose() {
      // Ensure that the controls actually exist first.
      if (controls_ != null) {
         controls_.dispose();
         // And make certain we don't think the controls are still valid.
         controls_ = null;
      }
   }

}
