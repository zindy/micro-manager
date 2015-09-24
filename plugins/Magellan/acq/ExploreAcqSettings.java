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

package acq;

import channels.ChannelSetting;
import channels.ChannelUtils;
import java.util.ArrayList;
import java.util.prefs.Preferences;
import javax.swing.filechooser.FileSystemView;
import main.Magellan;

/**
 * Container for settings specific to explore acquisition
 * @author Henry
 */
public class ExploreAcqSettings {
   
   private static final String EXPLORE_NAME_PREF = "Explore acq name";
   private static final String EXPLORE_DIR_PREF = "Explore acq dir";
   private static final String EXPLORE_Z_STEP = "Explore acq zStep";
   private static final String EXPLORE_TILE_OVERLAP = "Explore tile overlap";
   private static final String EXPLORE_RANK = "Explore rank";

   
   public final double zStep_;
   public final String dir_, name_;
   public final double tileOverlap_;
   public final int filterType_;
   public final ArrayList<ChannelSetting> channels_;

   public ExploreAcqSettings(double zStep, double overlapPercent, String dir, String name, int filterType, double rank, String channelGroup) {
      zStep_ = zStep;
      dir_ = dir;
      name_ = name;   
      tileOverlap_ = overlapPercent;
      filterType_ = filterType;
      //channels is all available channels for group
      channels_ = ChannelUtils.getAvailableChannels(channelGroup);
      
      Preferences prefs = Magellan.getPrefs();
      //now that explore acquisition is being run, store values
      prefs.put(EXPLORE_DIR_PREF, dir);
      prefs.put(EXPLORE_NAME_PREF, name);
      prefs.putDouble(EXPLORE_Z_STEP, zStep_);
      prefs.putDouble(EXPLORE_TILE_OVERLAP, overlapPercent);
//      prefs.putInt(EXPLORE_FILTER_METHOD, filterType);
      prefs.putDouble(EXPLORE_RANK, rank);
      
   }
   
   public static String getNameFromPrefs() {
      Preferences prefs = Magellan.getPrefs();
      return prefs.get(EXPLORE_NAME_PREF, "Untitled Explore Acquisition" );
   } 
   
   public static double getZStepFromPrefs() {
      Preferences prefs = Magellan.getPrefs();
      return prefs.getDouble(EXPLORE_Z_STEP, 1);
   }

   public static double getExploreTileOverlapFromPrefs() {
      Preferences prefs = Magellan.getPrefs();
      return prefs.getDouble(EXPLORE_TILE_OVERLAP, 0);
   }

   

}
