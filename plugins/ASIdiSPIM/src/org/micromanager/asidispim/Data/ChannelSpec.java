/*
 * AUTHOR:
 * Nico Stuurman, Jon Daniels
 *
 * Copyright (c) 2013 Regents of the University of California, & ASI
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


/**
 * Representation of information in channel table of 
 * diSPIM plugin.  Based on org.micromanager.utils.ChannelSpec.java. 
 */
public class ChannelSpec {
   public static final String DEFAULT_CHANNEL_GROUP = "Channel";
   public static final double Version = 0.1;

   // fields that are used
   public boolean useChannel_; // whether or not to use this group
   public String group_; // configuration group
   public String config_; // Configuration setting name
   
   // not used yet but may be useful in future
   //   public double exposure = 10.0; // ms
   //   public double zOffset = 0.0; // um
   //   public Color color = Color.gray;

   
   public ChannelSpec(boolean useChannel, String group, String config){
      this.useChannel_ = useChannel;
      this.group_ = group;
      this.config_ = config;
   }
   
         
   
}
