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

package org.micromanager.conf2;

import javax.swing.tree.DefaultMutableTreeNode;

class DeviceTreeNode extends DefaultMutableTreeNode {
   private static final long serialVersionUID = 1L;
   boolean showLib_;

   public DeviceTreeNode(String value, boolean byLib) {
      super(value);
      showLib_ = byLib;
   }

   @Override
   public String toString() {
      String ret = "";
      Object uo = getUserObject();
      if (null != uo) {
         if (uo.getClass().isArray()) {
            Object[] userData = (Object[]) uo;
            if (2 < userData.length) {
               if (showLib_)
                  ret = ((Boolean)userData[3] ? "*" : "") + userData[1].toString() + " | " + userData[2].toString();
               else
                  ret = ((Boolean)userData[3] ? "*" : "") + userData[0].toString() + " | " + userData[2].toString();
            }
         } else {
            ret = uo.toString();
         }
      }
      return ret;
   }

   // if user clicks on a container node, just return a null array instead of
   // the user data

   public Object[] getUserDataArray() {
      Object[] ret = null;

      Object uo = getUserObject();
      if (null != uo) {
         if (uo.getClass().isArray()) {
            // retrieve the device info tuple
            Object[] userData = (Object[]) uo;
            if (1 < userData.length) {
               ret = userData;
            }
         }

      }
      return ret;
   }
}
