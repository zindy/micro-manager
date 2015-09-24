/*
 * AUTHOR:
 * Mark Tsuchida
 *
 * Copyright (c) 2014 Regents of the University of California
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

import java.util.ArrayList;
import java.util.List;

public final class SystemInfo {
   public static void dumpAllToCoreLog(boolean includeUnchanging) {
      List<SystemInfoSection> sections = getAllSections(includeUnchanging);
      for (SystemInfoSection section : sections) {
         mmcorej.CMMCore core = org.micromanager.MMStudio.getInstance().getCore();
         core.logMessage(section.getReport());
      }
   }

   public static String getAllAsText(boolean includeUnchanging) {
      StringBuilder sb = new StringBuilder();
      List<SystemInfoSection> sections = getAllSections(includeUnchanging);
      for (SystemInfoSection section : sections) {
         sb.append(section.getReport());
      }
      return sb.toString();
   }

   public interface SystemInfoSection {
      String getTitle();
      String getReport();

      // In addition to implementing the above methods, implementing classes
      // must satisfy the following:
      // - Objects should be immutable (information should be collected at
      //   construction time)
      // - Constructors should not throw.
   }

   // Private for the time being
   private static List<SystemInfoSection> getAllSections(boolean includeUnchanging) {
      List<SystemInfoSection> sections = new ArrayList<SystemInfoSection>();

      if (includeUnchanging) {
         sections.add(new MMVersionInfoSection());
         sections.add(new ImageJInfoSection());
         sections.add(new OperatingSystemInfoSection());
         sections.add(new JVMInfoSection());
         sections.add(new JVMArgumentsInfoSection());
         sections.add(new JavaSystemPropertiesInfoSection()); // Strictly speaking, not unchanging.
         sections.add(new ProcessorInfoSection());
      }
      sections.add(new PhysicalMemoryInfoSection());
      sections.add(new JVMMemoryInfoSection());
      sections.add(new JVMThreadInfoSection());
      sections.add(new JVMDeadlockedThreadInfoSection());
      sections.add(new CoreBasicInfoSection());
      sections.add(new CorePropertyCacheInfoSection());

      return sections;
   }
}
