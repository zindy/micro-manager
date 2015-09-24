/*
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

import ij.IJ;
import ij.WindowManager;
import ij.gui.ImageCanvas;
import ij.gui.ImageWindow;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JOptionPane;

import mmcorej.CMMCore;
import mmcorej.MMCoreJ;

import org.micromanager.MMStudio;
import org.micromanager.internalinterfaces.LiveModeListener;
import org.micromanager.utils.ReportingUtils;

/**
 * @author OD
 *
 */
public final class XYZKeyListener implements KeyListener, LiveModeListener {
	private final CMMCore core_;
   private final MMStudio studio_;
	private ImageCanvas canvas_;
	private static boolean isRunning_ = false;
	private boolean mirrorX_;
	private boolean mirrorY_;
	private boolean transposeXY_;
	private boolean correction_;
	private static final double zmoveIncrement_ = 0.20;
	public static int ctrlZStep = 1;
	public static int normalZStep = 3;
	public static int shiftZStep = 10;
	public static int ctrlXYStep = 1;
	public static int normalXYStep = 10;
	private int step;
	private double stepX;
	private double stepY;

	public XYZKeyListener(CMMCore core, MMStudio gui) {
		core_ = core;
      studio_ = gui;
	}

   @Override
	public void keyPressed(KeyEvent e) {

		//core_.logMessage(Integer.toString(e.getKeyCode()));

		switch (e.getKeyCode()) {
		case KeyEvent.VK_LEFT:
		case KeyEvent.VK_RIGHT:
		case KeyEvent.VK_UP:
		case KeyEvent.VK_DOWN:
			//XY step
			stepX = stepY = normalXYStep;
			if (e.isControlDown()) {
				stepX = stepY = ctrlXYStep;
			} else if (e.isShiftDown()) {
				stepX = core_.getImageWidth();
				stepY = core_.getImageHeight();
			} else if (e.isAltDown()) {
				stepX = core_.getImageWidth() / 2;
				stepY = core_.getImageHeight() / 2;
			}
			switch (e.getKeyCode()) {
			case KeyEvent.VK_LEFT:
				IncrementXY(-stepX, 0);
				break;
			case KeyEvent.VK_RIGHT:
				IncrementXY(stepX, 0);
				break;
			case KeyEvent.VK_UP:
				IncrementXY(0, stepY);
				break;
			case KeyEvent.VK_DOWN:
				IncrementXY(0, -stepY);
			}
			break;
		case KeyEvent.VK_1:
		case KeyEvent.VK_U:
		case KeyEvent.VK_PAGE_UP:
		case KeyEvent.VK_2:
		case KeyEvent.VK_J:
		case KeyEvent.VK_PAGE_DOWN:
			step = normalZStep;
			if (e.isControlDown())
				step = ctrlZStep;
			else if (e.isShiftDown())
				step = shiftZStep;
			switch (e.getKeyCode()) {
			case KeyEvent.VK_1:
			case KeyEvent.VK_U:
			case KeyEvent.VK_PAGE_UP:
				IncrementZ(-step);
				break;
			case KeyEvent.VK_2:
			case KeyEvent.VK_J:
			case KeyEvent.VK_PAGE_DOWN:
				IncrementZ(step);
			}
		}
	}

   @Override
	public void keyReleased(KeyEvent arg0) {
	}

   @Override
	public void keyTyped(KeyEvent arg0) {
	}

	public void start() {
		// Get a handle to the AcqWindow
		if (WindowManager.getCurrentWindow() != null) {
			start(WindowManager.getCurrentWindow());
		}
	}

	public void start(ImageWindow win) {
		if (isRunning_)
			stop();

		isRunning_ = true;
		if (win != null) {
			attach(win);
		}
		getOrientation();
	}

	public void stop() {
		if (canvas_ != null) {
			canvas_.removeKeyListener(this);
         canvas_.addKeyListener(IJ.getInstance());
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
      canvas_.removeKeyListener(IJ.getInstance());
		canvas_.addKeyListener(this);
	}

	public void IncrementXY(double stepX, double stepY) {
		// Get needed info from core
		getOrientation();
		String xyStage = core_.getXYStageDevice();
		if (xyStage == null)
			return;
		try {
			if (core_.deviceBusy(xyStage))
				return;
		} catch (Exception ex) {
			ReportingUtils.showError(ex);
			return;
		}

		double pixSizeUm = core_.getPixelSizeUm();
		if (!(pixSizeUm > 0.0)) {
			JOptionPane
					.showMessageDialog(null,
							"Please provide pixel size calibration data before using this function");
			return;
		}

		// calculate needed relative movement
		double tmpXUm = stepX * pixSizeUm;
		double tmpYUm = stepY * pixSizeUm;

		double mXUm = tmpXUm;
		double mYUm = tmpYUm;
		// if camera does not correct image orientation, we'll correct for it here:
		if (!correction_) {
			// Order: swapxy, then mirror axis
			if (transposeXY_) {
				mXUm = tmpYUm;
				mYUm = tmpXUm;
			}
			if (mirrorX_) {
				mXUm = -mXUm;
			}
			if (mirrorY_) {
				mYUm = -mYUm;
			}
		}

		// Move the stage
		try {
			core_.setRelativeXYPosition(xyStage, mXUm, mYUm);
		} catch (Exception ex) {
			ReportingUtils.showError(ex);
			return;
		}

      // Cheap way to update XY position in GUI
      studio_.updateXYPosRelative(mXUm, mYUm);
	}

	public void IncrementZ(int step) {
		// Get needed info from core
		String zStage = core_.getFocusDevice();
		if (zStage == null || zStage.length() == 0)
			return;

		double moveIncrement = zmoveIncrement_;
		double pixSizeUm = core_.getPixelSizeUm();
		if (pixSizeUm > 0.0) {
			moveIncrement = pixSizeUm;
		}

		// Move the stage
		try {
			core_.setRelativePosition(zStage, moveIncrement * step);
		} catch (Exception ex) {
			ReportingUtils.showError(ex);
		}
	}

	public void getOrientation() {
		String camera = core_.getCameraDevice();
		if (camera == null) {
			JOptionPane.showMessageDialog(null,
					"This function does not work without a camera");
			return;
		}
		try {
			String tmp = core_.getProperty(camera, "TransposeCorrection");
         correction_ = !tmp.equals("0");
			tmp = core_.getProperty(camera, MMCoreJ
					.getG_Keyword_Transpose_MirrorX());
         mirrorX_ = !tmp.equals("0");
			tmp = core_.getProperty(camera, MMCoreJ
					.getG_Keyword_Transpose_MirrorY());
         mirrorY_ = !tmp.equals("0");
			tmp = core_.getProperty(camera, MMCoreJ
					.getG_Keyword_Transpose_SwapXY());
         transposeXY_ = !tmp.equals("0");
		} catch (Exception exc) {
			ReportingUtils.showError(exc);
		}
	}

   @Override
   public void liveModeEnabled(boolean enabled) {
      if (enabled) {
         start();
      } else {
         stop();
      }
   }
}
