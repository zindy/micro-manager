/*
 * AUTHOR:
 * Mark Tsuchida
 *
 * Copyright (c) 2013-2014 Regents of the University of California
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

package org.micromanager.diagnostics;

class PhysicalMemoryInfoSection implements SystemInfo.SystemInfoSection {
   public String getTitle() { return "Physical memory information"; }

   public String getReport() {
      StringBuilder sb = new StringBuilder();

      java.lang.management.OperatingSystemMXBean osMXB =
         java.lang.management.ManagementFactory.getOperatingSystemMXBean();

      try { // Use HotSpot extensions if available
         Class<?> sunOSMXBClass = Class.forName("com.sun.management.OperatingSystemMXBean");

         java.lang.reflect.Method totalMemMethod = sunOSMXBClass.getMethod("getTotalPhysicalMemorySize");
         long totalRAM = ((Long) totalMemMethod.invoke(osMXB)).longValue();
         sb.append("Total physical memory (caveats apply if JVM is 32-bit): ").
            append(formatMemSize(totalRAM)).append('\n');

         try {
            java.lang.reflect.Method committedMemMethod = sunOSMXBClass.getMethod("getCommittedVirtualMemorySize");
            long committedVM = ((Long) committedMemMethod.invoke(osMXB)).longValue();
            sb.append("Committed virtual memory size: ").
               append(formatMemSize(committedVM)).append('\n');
         }
         catch (Exception e) {
            sb.append("Committed virtual memory size: unavailable\n");
         }

         java.lang.reflect.Method freeMemMethod = sunOSMXBClass.getMethod("getFreePhysicalMemorySize");
         long freeRAM = ((Long) freeMemMethod.invoke(osMXB)).longValue();
         sb.append("Free physical memory (may be meaningless if JVM is 32-bit): ").
            append(formatMemSize(freeRAM)).append('\n');
      }
      catch (Exception e) {
         // Possible exceptions: ClassNotFoundException, NoSuchMethodException,
         // IllegalAccessException, java.lang.reflect.InvocationTargetException
         sb.append("Physical memory information: unavailable");
      }

      return sb.toString();
   }

   private String formatMemSize(long size) {
      if (size == -1) {
         return "unavailable";
      }
      if (size < 1024) {
         return Long.toString(size) + " bytes";
      }

      double bytes = size;
      java.text.NumberFormat format = new java.text.DecimalFormat("#.0");

      if (size < 1024 * 1024) {
         return Long.toString(size) + " (" + format.format(bytes / 1024) + " KiB)";
      }
      if (size < 1024 * 1024 * 1024) {
         return Long.toString(size) + " (" + format.format(bytes / (1024 * 1024)) + " MiB)";
      }
      return Long.toString(size) + " (" + format.format(bytes / (1024 * 1024 * 1024)) + " GiB)";
   }
}
