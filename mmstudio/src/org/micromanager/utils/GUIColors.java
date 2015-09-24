/*
 * AUTHOR:
 * Nico Stuurman, nico@cmp.ucsf.edu, July, 2007
 *
 * Copyright (c) 2007 Regents of the University of California
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

import java.awt.Color;
import java.util.HashMap;

/*
 * This class controls the colors of the user interface
 * Changes/additions here should manifest themselves in the UI
 */
public class GUIColors {
   final public String STYLE_DAY = "Day";
   final public String STYLE_NIGHT = "Night";
   // List of display options
   final public String[] styleOptions = {STYLE_DAY, STYLE_NIGHT};

   // background color of the UI
   public HashMap<String, Color> background;
   // background color of pads in the UI
   public HashMap<String, Color> padBackground;
   // TODO: implement font color

   public GUIColors() {
      // Possible: make UI to let user set these colors
      background = new HashMap<String, Color>();
      background.put(STYLE_DAY, java.awt.SystemColor.control);
      background.put(STYLE_NIGHT, java.awt.Color.gray);

      padBackground = new HashMap<String, Color>();
      padBackground.put(STYLE_DAY, java.awt.Color.white);
      padBackground.put(STYLE_NIGHT, java.awt.SystemColor.control);
   }

}
