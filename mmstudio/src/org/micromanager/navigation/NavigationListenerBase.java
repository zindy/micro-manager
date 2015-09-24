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

/**
 * 
 */
package org.micromanager.navigation;

import ij.gui.ImageWindow;

import javax.swing.JOptionPane;

import mmcorej.CMMCore;
import mmcorej.MMCoreJ;
import org.micromanager.utils.ReportingUtils;

/**
 * @author OD
 *
 */
public abstract class NavigationListenerBase {
	private CMMCore core_;
	private static boolean isRunning_ = false;
	private boolean mirrorX_;
	private boolean mirrorY_;
	private boolean transposeXY_;
	private boolean correction_;

	public NavigationListenerBase(CMMCore core) {
		core_ = core;
	}

	public boolean isRunning() {
		return isRunning_;
	}
	public void start() {
		isRunning_=true;
	}
	public void stop() {
		isRunning_=false;
	}
	
	protected boolean needMirrorX(){return mirrorX_;}
	protected boolean needMirrorY(){return mirrorY_;}
	protected boolean needTransposeXY(){return transposeXY_;}
	protected boolean needCorrection(){return correction_;}
	
	public abstract void start(ImageWindow win);
	public abstract void attach(ImageWindow win);
	
	public void getOrientation() {
		String camera = core_.getCameraDevice();
		if (camera == null) {
			JOptionPane.showMessageDialog(null,
					"This function does not work without a camera");
			return;
		}
		try {
			String tmp = core_.getProperty(camera, "TransposeCorrection");
			if (tmp.equals("0"))
				correction_ = false;
			else
				correction_ = true;
			tmp = core_.getProperty(camera, MMCoreJ
					.getG_Keyword_Transpose_MirrorX());
			if (tmp.equals("0"))
				mirrorX_ = false;
			else
				mirrorX_ = true;
			tmp = core_.getProperty(camera, MMCoreJ
					.getG_Keyword_Transpose_MirrorY());
			if (tmp.equals("0"))
				mirrorY_ = false;
			else
				mirrorY_ = true;
			tmp = core_.getProperty(camera, MMCoreJ
					.getG_Keyword_Transpose_SwapXY());
			if (tmp.equals("0"))
				transposeXY_ = false;
			else
				transposeXY_ = true;
		} catch (Exception exc) {
			ReportingUtils.logError(exc);
		}

	}

}
