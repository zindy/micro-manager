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
 * In practice no user code should directly implement this interface; instead
 * they should implement MMPlugin or MMProcessorPlugin.
 */
public interface MMBasePlugin {
   
   /**
    * The menu name is stored in a static string, so Micro-Manager
    * can obtain it without instantiating the plugin
    * Implement this member in your plugin
	 
      public static String menuName;
     
   */
      
   /**
    * A tool-tip description can also be in a static string. This tool-tip
    * will appear on the Micro-Manager plugin menu item.
    * Implement this member in your plugin
   
        public static String tooltipDescription = null;
   */

   /**
    * Returns a very short (few words) description of the module.
    */
   public String getDescription();
   
   /**
    * Returns verbose information about the module.
    * This may even include a short help instructions.
    */
   public String getInfo();
   
   /**
    * Returns version string for the module.
    * There is no specific required format for the version
    */
   public String getVersion();
   
   /**
    * Returns copyright information
    */
   public String getCopyright();
   
}
