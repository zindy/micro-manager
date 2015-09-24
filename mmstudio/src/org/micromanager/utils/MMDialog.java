/*
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, October 1, 2006
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

import java.awt.Frame;
import java.awt.Rectangle;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.prefs.Preferences;

import javax.swing.JDialog;

import org.micromanager.MMStudio;

/**
 * Base class for the Micro-Manager dialogs.
 * Saves and restores window size and position.
 */
public class MMDialog extends JDialog {
   private static final long serialVersionUID = -3144618980027203294L;
   private Preferences mmDialogPrefs_;
   private final String prefPrefix_;
   private static final String WINDOW_X = "mmdlg_y";
   private static final String WINDOW_Y = "mmdlg_x";
   private static final String WINDOW_WIDTH = "mmdlg_width";
   private static final String WINDOW_HEIGHT = "mmdlg_height";
   
   public MMDialog() {
      super();
      finishConstructor();
      prefPrefix_ = "";
   }
   public MMDialog(String prefPrefix) {
      super();
      finishConstructor();
      prefPrefix_ = prefPrefix;
   }
   public MMDialog(Frame owner) {
      super(owner);
      finishConstructor();
      prefPrefix_ = "";
   }
   public MMDialog(Frame owner, boolean isModal) {
      super(owner, isModal);
      finishConstructor();
      prefPrefix_ = "";
   }

   private void finishConstructor() {
      mmDialogPrefs_ = Preferences.userNodeForPackage(this.getClass());
      MMStudio mfr = MMStudio.getInstance();
      if (mfr != null) {
         mfr.addMMBackgroundListener(this);
    	   setBackground(mfr.getBackgroundColor());
      }
   }

   /**
    * Checks whether WINDOW_X and WINDOW_Y coordinates are on the screen(s).
    * If not then it sets the prefs to the values specified.
    * Accounts for screen size changes between invocations or if screen
    * is removed (e.g. had 2 monitors and go to 1).
    * @param x new WINDOW_X position if current value isn't valid
    * @param y new WINDOW_Y position if current value isn't valid
    */
   private void ensureSafeWindowPosition(int x, int y) {
      int prefX = mmDialogPrefs_.getInt(prefPrefix_ + WINDOW_X, 0);
      int prefY = mmDialogPrefs_.getInt(prefPrefix_ + WINDOW_Y, 0);
      if (GUIUtils.getGraphicsConfigurationContaining(prefX, prefY) == null) {
         // only reach this code if the pref coordinates are off screen
         mmDialogPrefs_.putInt(prefPrefix_ + WINDOW_X, x);
         mmDialogPrefs_.putInt(prefPrefix_ + WINDOW_Y, y);
      }
   }

    /**
    * Load window position and size from preferences if possible.
    * If not possible then sets them from arguments
    * Attaches a listener to the window that will save the position when the
    * window closing event is received
    * @param x - X position of this dialog if preference value invalid
    * @param y - y position of this dialog if preference value invalid
    * @param width - width of this dialog if preference value invalid
    * @param height - height of this dialog if preference value invalid
    */
   protected void loadAndRestorePosition(int x, int y, int width, int height) {
      loadPosition(x, y, width, height);
      this.addWindowListener(new WindowAdapter() {
         @Override
         public void windowClosing(WindowEvent arg0) {
            savePosition();
         }
      }
      );
   }
   
    /**
    * Load window position from preferences if possible.
    * If not possible then sets it from arguments
    * Attaches a listener to the window that will save the position when the
    * window closing event is received
    * @param x - X position of this dialog if preference value invalid
    * @param y - y position of this dialog if preference value invalid
    */
   protected void loadAndRestorePosition(int x, int y) {
      loadPosition(x, y);
      this.addWindowListener(new WindowAdapter() {
         @Override
         public void windowClosing(WindowEvent arg0) {
            savePosition();
         }
      }
      );
   }
   
   /**
    * Load window position and size from preferences
    * Makes sure that the window can be displayed
    * @param x - X position of this dialog
    * @param y - y position of this dialog
    * @param width - width of this dialog
    * @param height - height of this dialog
    */
   protected void loadPosition(int x, int y, int width, int height) {
      ensureSafeWindowPosition(x, y);
      setBounds(mmDialogPrefs_.getInt(prefPrefix_ + WINDOW_X, x),
                mmDialogPrefs_.getInt(prefPrefix_ + WINDOW_Y, y),
                mmDialogPrefs_.getInt(prefPrefix_ + WINDOW_WIDTH, width),
                mmDialogPrefs_.getInt(prefPrefix_ + WINDOW_HEIGHT, height));
   }
   
   @Override
   public void dispose() {
      savePosition();
      super.dispose();
   }
   
   protected void loadPosition(int x, int y) {
      ensureSafeWindowPosition(x, y);
      setLocation(mmDialogPrefs_.getInt(WINDOW_X, x),
                mmDialogPrefs_.getInt(WINDOW_Y, y));
   }

   /**
    * Writes window position and size to preferences
    */
   protected void savePosition() {
      Rectangle r = getBounds();
      if (r != null) {
         mmDialogPrefs_.putInt(prefPrefix_ + WINDOW_X, r.x);
         mmDialogPrefs_.putInt(prefPrefix_ + WINDOW_Y, r.y);
         mmDialogPrefs_.putInt(prefPrefix_ + WINDOW_WIDTH, r.width);
         mmDialogPrefs_.putInt(prefPrefix_ + WINDOW_HEIGHT, r.height);
      }
   }
   
   public Preferences getPrefsNode() {
      return mmDialogPrefs_;
   }
   
   public void setPrefsNode(Preferences p) {
      mmDialogPrefs_ = p;
   }


}
