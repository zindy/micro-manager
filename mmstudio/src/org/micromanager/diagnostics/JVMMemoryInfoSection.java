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

class JVMMemoryInfoSection implements SystemInfo.SystemInfoSection {
   public String getTitle() { return "JVM memory information"; }

   public String getReport() {
      StringBuilder sb = new StringBuilder();

      java.lang.management.MemoryMXBean memMXB = java.lang.management.ManagementFactory.getMemoryMXBean();
      sb.append("JVM heap memory usage: ").append(formatMemUsage(memMXB.getHeapMemoryUsage())).append('\n');
      sb.append("JVM non-heap memory usage: ").append(formatMemUsage(memMXB.getNonHeapMemoryUsage()));

      return sb.toString();
   }

   private String formatMemUsage(java.lang.management.MemoryUsage usage) {
      StringBuilder sb = new StringBuilder();
      sb.append("used = ").append(formatMemSize(usage.getUsed())).
         append("; committed = ").append(formatMemSize(usage.getCommitted())).
         append("; max = ").append(formatMemSize(usage.getMax()));
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
