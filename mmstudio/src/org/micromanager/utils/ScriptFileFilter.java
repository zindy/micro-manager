/*
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, October 1, 2005
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

import java.io.File;

import javax.swing.filechooser.FileFilter;

/**
 * File filter class for Open/Save file choosers 
 */
public class ScriptFileFilter extends FileFilter {
   final private String EXT_BSH;
   final private String DESCRIPTION;
   
   public ScriptFileFilter() {
      super();
      EXT_BSH = new String("bsh");
      DESCRIPTION = new String("BeanShell files (*.bsh)");
   }
   
   @Override
   public boolean accept(File f){
      if (f.isDirectory())
         return true;
      
      if (EXT_BSH.equals(getExtension(f)))
         return true;
      return false;
   }
   
   public String getDescription(){
      return DESCRIPTION;
   }
   
   private String getExtension(File f) {
      String ext = null;
      String s = f.getName();
      int i = s.lastIndexOf('.');
      
      if (i > 0 &&  i < s.length() - 1) {
         ext = s.substring(i+1).toLowerCase();
      }
      return ext;
   }
}
