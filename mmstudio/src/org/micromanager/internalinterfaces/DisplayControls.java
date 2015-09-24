/*
 * AUTHOR:
 * Henry Pinkard, henry.pinkard@gmail.com, 2012
 *
 * Copyright (c) 2012 Regents of the University of California
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

package org.micromanager.internalinterfaces;

import java.awt.Panel;
import org.json.JSONObject;

public abstract class DisplayControls extends Panel {

   public DisplayControls() {};

   public DisplayControls(java.awt.LayoutManager manager) {
      super(manager);
   }
   
   abstract public void imagesOnDiskUpdate(boolean onDisk);
   
   abstract public void acquiringImagesUpdate(boolean acquiring);
   
   abstract public void setImageInfoLabel(String text);

   abstract public void newImageUpdate(JSONObject tags);
   
   abstract public void prepareForClose();

   public void setChannel(int c) {}

   public int getPosition() {
      return 0;
   }

   public void setPosition(int p) {}

   public int getNumPositions() {
      return 1;
   }
}
