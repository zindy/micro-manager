/*
 * AUTHOR:
 * Nico Stuurman
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

package org.micromanager.splitview;

import com.google.common.eventbus.Subscribe;

import ij.process.ByteProcessor;
import ij.process.ImageProcessor;
import ij.process.ShortProcessor;
import mmcorej.TaggedImage;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.micromanager.acquisition.TaggedImageQueue;
import org.micromanager.api.DataProcessor;
import org.micromanager.api.ScriptInterface;
import org.micromanager.utils.MDUtils;
import org.micromanager.utils.MMScriptException;
import org.micromanager.utils.ReportingUtils;
import org.micromanager.events.SummaryMetadataEvent;

/**
 * DataProcessor that splits images as instructed in SplitViewFrame
 *
 * @author nico
 */
public class SplitViewProcessor extends DataProcessor<TaggedImage> {

   private SplitViewFrame myFrame_;
   private String orientation_ = SplitViewFrame.LR;

   @Override
   public void makeConfigurationGUI() {
      if (myFrame_ == null) {
         try {
            myFrame_ = new SplitViewFrame(this, gui_);
            gui_.addMMBackgroundListener(myFrame_);
         }
         catch (Exception ex) {
            ReportingUtils.logError("Failed to make GUI for SplitViewProcessor: " + ex);
         }
      }
      myFrame_.setVisible(true);
      gui_.registerForEvents(this);
   }

   @Override
   public void dispose() {
      if (myFrame_ != null) {
         myFrame_.dispose();
         myFrame_ = null;
      }
      gui_.unregisterForEvents(this);
   }

   public void setOrientation(String orientation) {
      orientation_ = orientation;
   }

   private String getChannelSuffix(int channelIndex) {
      String token;
      if (orientation_.equals(SplitViewFrame.LR)) {

         if ((channelIndex % 2) == 0) {
            token = "Left";
         } else {
            token = "Right";
         }
      } else { // orientation is "TB"
         if ((channelIndex % 2) == 0) {
            token = "Top";
         } else {
            token = "Bottom";
         }
      }
      return token;
   }

   @Override
   public void process() {

      
      TaggedImage taggedImage = poll();
      try {
         
         if (TaggedImageQueue.isPoison(taggedImage)) {
            produce(taggedImage);
            return;
         }

         if (taggedImage != null && taggedImage.tags != null) {
            ImageProcessor tmpImg;
            int imgDepth = MDUtils.getDepth(taggedImage.tags);
            int width = MDUtils.getWidth(taggedImage.tags);
            int height = MDUtils.getHeight(taggedImage.tags);
            int channelIndex = MDUtils.getChannelIndex(taggedImage.tags);

            //System.out.println("Processed one");

            if (imgDepth == 1) {
               tmpImg = new ByteProcessor(width, height);
            } else if (imgDepth == 2) {
               tmpImg = new ShortProcessor(width, height);
            } else // TODO throw error
            {
               produce(taggedImage);
               return;
            }

            tmpImg.setPixels(taggedImage.pix);

            height = calculateHeight(height);
            width = calculateWidth(width);

            tmpImg.setRoi(0, 0, width, height);
            
            
            // first channel

            // Weird way of copying a JSONObject
            JSONObject tags = new JSONObject(taggedImage.tags.toString());
            MDUtils.setWidth(tags, width);
            MDUtils.setHeight(tags, height);
            MDUtils.setChannelIndex(tags, channelIndex * 2);

            tags.put("Channel", MDUtils.getChannelName(taggedImage.tags) + getChannelSuffix(channelIndex*2));
            
            TaggedImage firstIm = new TaggedImage(tmpImg.crop().getPixels(), tags);

            // second channel
            JSONObject tags2 = new JSONObject(tags.toString());
            tags2.put("Channel", MDUtils.getChannelName(taggedImage.tags)  + getChannelSuffix(channelIndex*2+1));

            if (orientation_.equals(SplitViewFrame.LR)) {
               tmpImg.setRoi(width, 0, width, height);
            } else if (orientation_.equals(SplitViewFrame.TB)) {
               tmpImg.setRoi(0, height, width, height);
            }
            MDUtils.setWidth(tags2, width);
            MDUtils.setHeight(tags2, height);
            MDUtils.setChannelIndex(tags2, channelIndex * 2 + 1);

            TaggedImage secondIm = new TaggedImage(tmpImg.crop().getPixels(), tags2);

            produce(secondIm);
            produce(firstIm);
         }
      } catch (MMScriptException ex) {
         ReportingUtils.logError(ex);
         produce(taggedImage);
      } catch (JSONException ex) {
         ReportingUtils.logError(ex);
         produce(taggedImage);
      }
   }

   public int calculateWidth(int width) {
      int newWidth = width;
      if (orientation_.equals(SplitViewFrame.LR)) {
         newWidth = width / 2;
      }
      return newWidth;
   }

   public int calculateHeight(int height) {
      int newHeight = height;
      if (orientation_.equals(SplitViewFrame.TB)) {
         newHeight = height / 2;
      }
      return newHeight;
   }

   /**
    * HACK: we need to adjust acquisition summary metadata to reflect the
    * changed image dimensions and number of channels.
    */
   @Subscribe
   public void onSummaryMetadata(SummaryMetadataEvent event) {
      try {
         // Adjust the image size and number of channels.
         JSONObject summary = event.getSummaryMetadata();
         // Pad arrays to have twice as many elements by copying earlier
         // elements over.
         for (String arrTag : new String[] {"ChContrastMin", "ChContrastMax",
            "ChNames", "ChColors"}) {
            JSONArray contents = summary.getJSONArray(arrTag);
            int len = contents.length();
            for (int i = 0; i < len; ++i) {
               contents.put(contents.get(i));
            }
         }
         // Adjust channel colors. Note color array length has already been
         // padded by the above block.
         JSONArray colors = summary.getJSONArray("ChColors");
         for (int i = 0; i < colors.length(); ++i) {
            if (i % 2 == 0) {
               colors.put(i, myFrame_.getColor1().getRGB());
            }
            else {
               colors.put(i, myFrame_.getColor2().getRGB());
            }
         }
         summary.put("Channels", summary.getInt("Channels") * 2);
         // Adjust image dimensions and ROI.
         JSONArray roi = summary.getJSONArray("ROI");
         if (orientation_.equals(SplitViewFrame.TB)) {
            summary.put("Height", summary.getInt("Height") / 2);
            roi.put(3, roi.getInt(3) / 2);
         }
         else {
            summary.put("Width", summary.getInt("Width") / 2);
            roi.put(2, roi.getInt(2) / 2);
         }
      }
      catch (JSONException e) {
         ReportingUtils.logError(e, "Error updating summary metadata");
      }
   }
}
