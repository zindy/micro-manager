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

package channels;

import java.awt.Color;
import java.util.ArrayList;
import java.util.Arrays;
import main.Magellan;
import misc.GlobalSettings;
import misc.Log;
import mmcorej.StrVector;

/**
 *
 * @author Henry
 */
public class ChannelUtils {

   private static final String PREF_EXPOSURE = "EXPOSURE";
   private static final String PREF_COLOR = "COLOR";
   private static final String PREF_USE = "USE";
   private static final Color[] DEFAULT_COLORS = {new Color(160, 32, 240), Color.blue, Color.green, Color.yellow, Color.red, Color.pink};

   private static String[] getChannelConfigs(String channelGroup) {
      if (channelGroup == null || channelGroup.equals("")) {
         return new String[0];
      }
      StrVector configs = Magellan.getCore().getAvailableConfigs(channelGroup);
      String[] names = new String[(int) configs.size()];
      for (int i = 0; i < names.length; i++) {
         names[i] = configs.get(i);
      }
      return names;
   }
   
   public static void storeChannelInfo(ArrayList<ChannelSetting> channels) {
      for (ChannelSetting c : channels) {
         GlobalSettings.getInstance().storeDoubleInPrefs(PREF_EXPOSURE + c.name_, c.exposure_);
         GlobalSettings.getInstance().storeIntInPrefs(PREF_COLOR + c.name_, c.color_.getRGB());
         GlobalSettings.getInstance().storeBooleanInPrefs(PREF_USE + c.name_, c.use_);
      }
   }

   public static ArrayList<ChannelSetting> getAvailableChannels(String channelGroup) {
      int numCamChannels = (int) (GlobalSettings.getInstance().getDemoMode() ? 6 : Magellan.getCore().getNumberOfCameraChannels());
      ArrayList<ChannelSetting> channels = new ArrayList<ChannelSetting>();
      double exposure = 10;
      try {
         exposure = Magellan.getCore().getExposure();
      } catch (Exception ex) {
         Log.log("Couldnt get camera exposure form core", true);
      }
      if (numCamChannels <= 1) {
         for (String config : getChannelConfigs(channelGroup)) {
            Color color = new Color(GlobalSettings.getInstance().getIntInPrefs(PREF_COLOR + config,
                    DEFAULT_COLORS[Arrays.asList(getChannelConfigs(channelGroup)).indexOf(config)].getRGB()));
            boolean use = GlobalSettings.getInstance().getBooleanInPrefs(PREF_USE + config, true);
            channels.add(new ChannelSetting(channelGroup, config, config, exposure, color, use, true));
         }
      } else { //multichannel camera
         for (int i = 0; i < numCamChannels; i++) {
            String cameraChannelName = GlobalSettings.getInstance().getDemoMode() ?
                    new String[]{"Violet","Blue","Green","Yellow","Red","FarRed"}[i]
                    : Magellan.getCore().getCameraChannelName(i);
            if (getChannelConfigs(channelGroup).length == 0) {
               Color color = new Color(GlobalSettings.getInstance().getIntInPrefs(PREF_COLOR + cameraChannelName,
                       DEFAULT_COLORS[i].getRGB()));
               boolean use = GlobalSettings.getInstance().getBooleanInPrefs(PREF_USE + cameraChannelName, true);
               channels.add(new ChannelSetting(channelGroup, null, cameraChannelName, exposure, color, use, i == 0));
            } else {
               for (String config : getChannelConfigs(channelGroup)) {
                  Color color = new Color(GlobalSettings.getInstance().getIntInPrefs(PREF_COLOR + cameraChannelName + "-" + config,
                          DEFAULT_COLORS[i].getRGB()));
                  boolean use = GlobalSettings.getInstance().getBooleanInPrefs(PREF_USE + cameraChannelName + "-" + config, true);
                  channels.add(new ChannelSetting(channelGroup, config, cameraChannelName + "-" + config, exposure, color, use, i == 0));
               }
            }
         }
      }
      return channels;
   }
}
