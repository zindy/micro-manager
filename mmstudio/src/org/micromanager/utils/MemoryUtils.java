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

/* MemoryUtils.java
 * Created on Jun 8, 2007
 *
 * MicroManage
 */
package org.micromanager.utils;

import ij.plugin.Memory;

public class MemoryUtils {
   /**
    * Returns the currently allocated memory.
    */
   public static long currentMemory() {
      long freeMem = Runtime.getRuntime().freeMemory();
      long totMem = Runtime.getRuntime().totalMemory();
      return totMem-freeMem;
   }
   
   public static long freeMemory() {
      long maxMemory = Runtime.getRuntime().maxMemory();
      long totalMemory = Runtime.getRuntime().totalMemory();
      long freeMemory = Runtime.getRuntime().freeMemory();
      
      return maxMemory - (totalMemory - freeMemory);
   }

   /** Returns the maximum amount of memory available */
   public static long maxMemory() {
      Memory mem = new Memory();
      long maxMemory = mem.getMemorySetting();
         if (maxMemory==0L)
            maxMemory = mem.maxMemory();
      return maxMemory;
   }

}
