/*
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, November 29, 2006
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

import java.util.prefs.Preferences;

/**
 * Utility class for the PropertyBrowser to specify which devices
 * are currently visible.
 *
 */
public class ShowFlags {
   public boolean cameras_ = true;
   public boolean shutters_ = true;
   public boolean stages_ = true;
   public boolean state_ = true;
   public boolean other_ = true;
   
   private static final String SHOW_CAMERAS = "show_cameras";
   private static final String SHOW_SHUTTERS = "show_shutters";
   private static final String SHOW_STAGES = "show_stages";
   private static final String SHOW_STATE = "show_state";
   private static final String SHOW_OTHER = "show_other";
   
   public void load(Preferences prefs) {
      cameras_ = prefs.getBoolean(SHOW_CAMERAS, cameras_);
      shutters_ = prefs.getBoolean(SHOW_SHUTTERS, shutters_);
      stages_ = prefs.getBoolean(SHOW_STAGES, stages_);
      state_ = prefs.getBoolean(SHOW_STATE, state_);
      other_ = prefs.getBoolean(SHOW_OTHER, other_);
   }
   
   public void save(Preferences prefs) {
      prefs.putBoolean(SHOW_CAMERAS, cameras_);
      prefs.putBoolean(SHOW_SHUTTERS, shutters_);
      prefs.putBoolean(SHOW_STAGES, stages_);
      prefs.putBoolean(SHOW_STATE, state_);
      prefs.putBoolean(SHOW_OTHER, other_);
   }
 }
