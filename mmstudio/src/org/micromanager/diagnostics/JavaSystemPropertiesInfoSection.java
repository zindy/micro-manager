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

class JavaSystemPropertiesInfoSection implements SystemInfo.SystemInfoSection {
   public String getTitle() { return "Java system properties"; }

   public String getReport() {
      String pathSep = System.getProperty("path.separator");
      if (pathSep.length() == 0) {
         pathSep = null;
      }

      StringBuilder sb = new StringBuilder();
      sb.append("Java system properties:\n");

      java.util.Properties sysProps = System.getProperties();
      java.util.List<String> propKeys = new java.util.ArrayList<String>();
      java.util.Enumeration<Object> e = sysProps.keys();
      while (e.hasMoreElements()) {
         propKeys.add((String) e.nextElement());
      }
      java.util.Collections.sort(propKeys);
      for (String k : propKeys) {
         if (pathSep != null && pathListProperties_.contains(k)) {
            sb.append("  ").append(k).append(" (split at \'").append(pathSep).append("\') =\n");
            String[] paths = sysProps.getProperty(k).split(pathSep);
            for (String path : paths) {
               sb.append("    ").append(path).append('\n');
            }
         }
         else {
            sb.append("  ").append(k).append(" = ").append(sysProps.getProperty(k)).append('\n');
         }
      }

      sb.append("(End Java system properties)");
      return sb.toString();
   }

   private static final java.util.Set<String> pathListProperties_;
   static {
      pathListProperties_ = new java.util.HashSet<String>();
      pathListProperties_.add("java.class.path");
      pathListProperties_.add("java.library.path");
      pathListProperties_.add("sun.boot.class.path");
      pathListProperties_.add("sun.boot.library.path");
   }
}
