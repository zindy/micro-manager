/*
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, November 10, 2005
 *
 * Copyright (c) 2006 Regents of the University of California
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

package org.micromanager.utils;
import java.awt.Color;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

/**
 * Channel acquisition protocol. 
 */
@SuppressWarnings("unused")
public class ChannelSpec {
   public static final String DEFAULT_CHANNEL_GROUP = "Channel";
   public static final double Version = 1.0;

   public Boolean doZStack = true;
   public String config = ""; // Configuration setting name
   public double exposure = 10.0; // ms
   public double zOffset = 0.0; // um
   public Color color = Color.gray;
   public ContrastSettings contrast;
   public int skipFactorFrame = 0;
   public boolean useChannel = true;
   public String camera = "";
   
   public ChannelSpec(){
      contrast = new ContrastSettings(0, 65535);
      color = Color.WHITE;
   }
   
   /**
    * Serialize to JSON encoded string
    */
   public static String toJSONStream(ChannelSpec cs) {
      Gson gson = new GsonBuilder().setPrettyPrinting().create();
      return gson.toJson(cs);
   }
   
   /**
    * De-serialize from JSON encoded string
    */
   public static ChannelSpec fromJSONStream(String stream) {
      Gson gson = new Gson();
      ChannelSpec cs = gson.fromJson(stream, ChannelSpec.class);
      return cs;
   }
   
   // test serialization
   public synchronized static void main(String[] args) {
      
      // encode
      ChannelSpec cs = new ChannelSpec();
      String stream = ChannelSpec.toJSONStream(cs);
      System.out.println("Encoded:\n" + stream);
      
      // decode
      ChannelSpec resultCs = ChannelSpec.fromJSONStream(stream);
      System.out.println("Decoded:\n" + ChannelSpec.toJSONStream(resultCs));
   }
   
}

