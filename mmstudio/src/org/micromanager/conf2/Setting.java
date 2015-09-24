/*
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, October 29, 2006
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

package org.micromanager.conf2;

/**
 * Data structure for device settings.
 *
 */
public class Setting {
   public String deviceName_;
   public String propertyName_;
   public String propertyValue_;
   
   public Setting() {
      deviceName_ = new String("NoName");
      propertyName_ = new String("Undefined");
      propertyValue_ = new String();
   }
   
   public Setting(String devName, String propName, String propVal) {
      deviceName_ = devName;
      propertyName_ = propName;
      propertyValue_ = propVal;
   }
   
   /**
    * Comapres two settings based on their content.
    */
   public boolean isEqualTo(Setting s) {
      if (deviceName_.compareTo(s.deviceName_) == 0 &&
          propertyName_.compareTo(s.propertyName_) == 0 &&
          propertyValue_.compareTo(s.propertyValue_) == 0) {
         return true;
      }
      return false;
   }
   
   /**
    * Two settings match if deviceName and propertyName are the same. The value
    * is not taken into account.
    */
   public boolean matches(Setting s) {
      if (deviceName_.compareTo(s.deviceName_) == 0 &&
            propertyName_.compareTo(s.propertyName_) == 0) {
           return true;
      }
      return false;
   }
   
   public String toString() {
      return new String(deviceName_ + ":" + propertyName_ + "=" + propertyValue_);
   }
}
