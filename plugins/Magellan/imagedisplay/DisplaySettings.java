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

import java.awt.Color;
import json.JSONArray;
import json.JSONObject;
import misc.MD;


public class DisplaySettings {

   public static JSONObject getDisplaySettingsFromSummary(JSONObject summaryMetadata) throws Exception {
      JSONObject displaySettings = new JSONObject();

      //create empty display and comments object  
      JSONArray channels = new JSONArray();            
      JSONObject comments = new JSONObject();
      displaySettings.put("Channels", channels);  
      String summary = "";
      comments.put("Summary", summary);
      displaySettings.put("Comments", comments);
      
      int numDisplayChannels;
      JSONArray chColors = null, chMaxes = null, chMins = null, chNames = null;
      if (summaryMetadata.has("ChNames")) {
         chNames = MD.getJSONArrayMember(summaryMetadata, "ChNames");
         // HACK: derive the number of channels from the number of channel
         // names. 
         numDisplayChannels = chNames.length();
      } else {
         numDisplayChannels = MD.getNumChannels(summaryMetadata);
         if (MD.isRGB(summaryMetadata)) {
            numDisplayChannels *= 3;
         }
      }
      if (summaryMetadata.has("ChColors")) {
          chColors = MD.getJSONArrayMember(summaryMetadata, "ChColors");
      } 
      if (summaryMetadata.has("ChContrastMin")) {
         chMins = MD.getJSONArrayMember(summaryMetadata, "ChContrastMin");
      } 
      if ( summaryMetadata.has("ChContrastMax")) {
         chMaxes = MD.getJSONArrayMember(summaryMetadata, "ChContrastMax");
      }      
      
      for (int k = 0; k < numDisplayChannels; ++k) {
         String name = chNames != null ? chNames.getString(k) :"channel " + k;
         int color = (chColors != null && k < chColors.length()) ? 
                 chColors.getInt(k) : Color.white.getRGB();
         int min = (chMins != null && chMins.length() > k) ? chMins.getInt(k) : 0;
         int bitDepth = 16;
         if (summaryMetadata.has("BitDepth")) {
            bitDepth = MD.getBitDepth(summaryMetadata);
         } else if (summaryMetadata.has("PixelType")) {
            if (MD.isGRAY8(summaryMetadata) ) { 
//                    || MD.isRGB32(summaryMetadata)) {
               bitDepth = 8;
            }
         }
         int max = (chMaxes != null && chMaxes.length() > k) ? chMaxes.getInt(k) : (int) (Math.pow(2, bitDepth) - 1);
         JSONObject channelObject = new JSONObject();
         channelObject.put("Color", color);
         channelObject.put("Name", name);
         channelObject.put("Gamma", 1.0);
         channelObject.put("Min", min);
         channelObject.put("Max", max);
         channels.put(channelObject);
      }
      return displaySettings;
   }
}

