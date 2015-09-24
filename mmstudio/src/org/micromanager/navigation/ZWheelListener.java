/*
 * AUTHOR:
 * Nico Stuurman, nico@cmp.ucsf.edu May 30, 2008
 *
 * Copyright (c) 2008 Regents of the University of California
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

package org.micromanager.navigation;

import ij.WindowManager;
import ij.gui.ImageCanvas;
import ij.gui.ImageWindow;

import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;


import org.micromanager.MMStudio;
import mmcorej.CMMCore;
import org.micromanager.internalinterfaces.LiveModeListener;
import org.micromanager.utils.ReportingUtils;

/**
*/
public class ZWheelListener implements MouseWheelListener, LiveModeListener {
   private CMMCore core_;
   private MMStudio studio_;
   private ImageCanvas canvas_;
   private static boolean isRunning_ = false;
   private static final double moveIncrement_ = 0.20;

   public ZWheelListener(CMMCore core, MMStudio gui) {
      core_ = core;
      studio_ = gui;
   }

   public void start () {
      // Get a handle to the AcqWindow
      if (WindowManager.getCurrentWindow() != null) {
         start(WindowManager.getCurrentWindow());
      }
   }
   
   public void start(ImageWindow win) {
      if (isRunning_) {
         stop(); 
      }

	  isRunning_ = true;
	  if (win != null) {
		  attach(win);
	  }
   }

   public void stop() {
      if (canvas_ != null) {
         canvas_.removeMouseWheelListener(this);
      }
      isRunning_ = false;
   }

   public boolean isRunning() {
      return isRunning_;
   }

   public void attach(ImageWindow win) {
      if (!isRunning_)
         return;
      canvas_ = win.getCanvas();
      canvas_.addMouseWheelListener(this);
   }

   @Override
   public void mouseWheelMoved(MouseWheelEvent e) {
	  synchronized(this) {
		  // Get needed info from core
		  String zStage = core_.getFocusDevice();
		  if (zStage == null || zStage.equals(""))
			  return;
 
		  double moveIncrement = moveIncrement_;
		  double pixSizeUm = core_.getPixelSizeUm();
		  if (pixSizeUm > 0.0) {
			  moveIncrement = 2 * pixSizeUm;
		  }
		  // Get coordinates of event
		  int move = e.getWheelRotation();
		  
		  // Move the stage
		  try {
			  core_.setRelativePosition(zStage, move * moveIncrement);
		  } catch (Exception ex) {
			  ReportingUtils.showError(ex);
			  return;
		  }
        studio_.updateZPosRelative(move * moveIncrement);
	  }
   } 
   
   public void liveModeEnabled(boolean enabled) {
      if (enabled) {
         start();
      } else {
         stop();
      }
   }
}
