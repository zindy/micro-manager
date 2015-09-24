/*
 * AUTHOR:
 * Arthur Edelstein, Nenad Amodaj
 *
 * Copyright (c) 2013 Regents of the University of California
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

import java.util.ArrayList;

import org.micromanager.utils.ChannelSpec;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

public class SequenceSettings {
   
   // version ID for the sequence settings
   public static final double Version = 1.0;

   // acquisition protocol
   public int numFrames = 1;                                                 // number of frames
   public double intervalMs = 0.0;                                           // frame interval
   public ArrayList<Double> customIntervalsMs = null;                        // sequence of custom intervals or null
   public ArrayList<ChannelSpec> channels = new ArrayList<ChannelSpec>();    // an array of ChannelSpec settings (one for each channel)
   public ArrayList<Double> slices = new ArrayList<Double>();                // slice Z coordinates
   public boolean relativeZSlice = false;                                    // are Z coordinates relative or absolute
   public boolean slicesFirst = false;                                       // slice coordinate changes first
   public boolean timeFirst = false;                                         // frame coordinate changes first
   public boolean keepShutterOpenSlices = false;                             // do we keep shutter open during slice changes
   public boolean keepShutterOpenChannels = false;                           // do we keep shutter open channel changes
   public boolean useAutofocus = false;                                      // are we going to run autofocus before acquiring each position/frame
   public int skipAutofocusCount = 0;                                        // how many af opportunities to skip                          
   public boolean save = false;                                              // save to disk?
   public String root = null;                                                // root directory name
   public String prefix = null;                                              // acquisition name
   public double zReference = 0.0;                                           // referent z position for relative moves                                            
   public String comment = "";                                               // comment text
   public String channelGroup = "";                                          // which group is used to define fluorescence channels
   public boolean usePositionList = false;                                   // true if we want to have multiple positions
   public int cameraTimeout = 20000; // Minimum camera timeout, in ms, for sequence acquisitions (actual timeout depends on exposure time and other factors)
      
   public static String toJSONStream(SequenceSettings settings) {
      Gson gson = new GsonBuilder().setPrettyPrinting().create();
      return gson.toJson(settings);      
   }
   
   public static SequenceSettings fromJSONStream(String stream) {
      Gson gson = new Gson();
      return gson.fromJson(stream, SequenceSettings.class);
   }
   
   // test serialization
   public synchronized static void main(String[] args) {
      
      // encode
      SequenceSettings s = new SequenceSettings();
      String channelGroup = "Channel";

      s.numFrames = 20;

      s.slices = new ArrayList<Double>();
      s.slices.add(-1.0);
      s.slices.add(0.0);
      s.slices.add(1.0);     
      s.relativeZSlice = true;

      s.channels = new ArrayList<ChannelSpec>();
      ChannelSpec ch1 = new ChannelSpec();
      ch1.config = "DAPI";
      ch1.exposure = 5.0;
      s.channels.add(ch1);
      ChannelSpec ch2 = new ChannelSpec();
      ch2.config = "FITC";
      ch2.exposure = 15.0;
      s.channels.add(ch2);

      s.prefix = "ACQ-TEST-B";
      s.root = "C:/AcquisitionData";
      s.channelGroup = channelGroup;

      String stream = SequenceSettings.toJSONStream(s);
      System.out.println("Encoded:\n" + stream);
      
      // decode
      SequenceSettings resultSs = SequenceSettings.fromJSONStream(stream);
      System.out.println("Decoded:\n" + SequenceSettings.toJSONStream(resultSs));
   }
   
}
