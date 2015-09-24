/*
 * API for MM plugins. Each module has to implement this interface.
 *
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, 2008
 *
 * Copyright (c) 2008 100X Imaging Inc, www.100ximaging.com
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
 * Implement this interface to create Micro-Manager plugins. Compiled jars
 * may be dropped into Micro-Manager's mmplugin directory, and if correctly
 * implemented, will appear in the Micro-Manager plugins menu.
 * You should look at the MMBasePlugin.java file as well for other functions
 * and member fields that should be implemented.
 */
public interface MMPlugin extends MMBasePlugin {
   /**
    * The main app calls this method to remove the module window
    */
   public void dispose();
   
   /**
    * The main app passes its ScriptInterface to the module. This
    * method is typically called after the module is instantiated.
    * @param app - ScriptInterface implementation
    */
   public void setApp(ScriptInterface app);
   
   /**
    * Open the module window
    */
   public void show();
      
}
