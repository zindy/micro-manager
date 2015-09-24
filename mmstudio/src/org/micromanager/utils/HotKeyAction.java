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

package org.micromanager.utils;

/**
 * Storage container for HotKeyActions
 *
 * @author nico
 */
public class HotKeyAction {
      public static final int GUICOMMAND = 0;
      public static final int BEANSHELLSCRIPT = 1;
      public static final int SNAP = 0;
      public static final int TOGGLELIVE = 1;
      public static final int TOGGLESHUTTER = 2;
      public static final int ADD_TO_ALBUM = 3;
      public static final int MARK = 4;
      public static final int AUTOSTRETCH = 5;
      public static final String[] guiItems_ = {"Snap", "Toggle Live", "Toggle Shutter", "->Album", "Mark Position", "Autostretch histograms"};
      public static final int NRGUICOMMANDS = guiItems_.length;

      public int type_;  // either GUICOMMAND or BEANSHELLSCRIPT
      public int guiCommand_;
      public java.io.File beanShellScript_;
      private org.micromanager.MMStudio studio_ =
              org.micromanager.MMStudio.getInstance();
      private org.micromanager.SnapLiveManager snapLiveManager_;

      public HotKeyAction(int guiCommand) {
         type_ = GUICOMMAND;
         guiCommand_ = guiCommand;
         snapLiveManager_ = studio_.getSnapLiveManager();
      }

      public HotKeyAction(java.io.File beanshellScript) {
         type_ = BEANSHELLSCRIPT;
         beanShellScript_ = beanshellScript;
      }

      public boolean ExecuteAction() {
         if (type_ == GUICOMMAND) {
            switch (guiCommand_) {
               case SNAP:
                  studio_.snapSingleImage();
                  return true;
               case TOGGLELIVE:
                  snapLiveManager_.setLiveMode(!snapLiveManager_.getIsLiveModeOn());
                  return true;
               case TOGGLESHUTTER:
                  studio_.toggleShutter();
                  return true;
               case ADD_TO_ALBUM:
                  snapLiveManager_.snapAndAddToImage5D();
                  return true;
               case MARK:
                  studio_.markCurrentPosition();
                  return true;
               case AUTOSTRETCH:
                  studio_.autostretchCurrentWindow();
            }
         } else {
            org.micromanager.script.ScriptPanel.runFile(beanShellScript_);
            return true;
         }
         return false;
      }


}
