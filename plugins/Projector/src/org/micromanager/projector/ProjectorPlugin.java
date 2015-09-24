/*
 * AUTHOR:
 * Arthur Edelstein
 *
 * Copyright (c) 2010-2014 Regents of the University of California
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

package org.micromanager.projector;

import ij.IJ;
import ij.plugin.frame.RoiManager;
import java.awt.Checkbox;
import java.awt.Panel;
import java.awt.event.ItemEvent;
import mmcorej.CMMCore;
import org.micromanager.api.MMPlugin;
import org.micromanager.api.ScriptInterface;
import org.micromanager.utils.GUIUtils;
import org.micromanager.utils.ReportingUtils;


// The Projector plugin provides a user interface for calibration and control
// of SLM- and Galvo-based phototargeting devices. Phototargeting can be
// ad-hoc or planned as part of a multi-dimensional acquisition.
public class ProjectorPlugin implements MMPlugin {
   public static final String menuName = "Projector";
   public static final String tooltipDescription =
      "Control galvo or SLM devices that project a spot or pattern " +
      "on the sample";
   
   private ScriptInterface app_;
   private CMMCore core_;

  // Show the ImageJ Roi Manager and return a reference to it.   
   public static RoiManager showRoiManager() {
      IJ.run("ROI Manager...");
      final RoiManager roiManager = RoiManager.getInstance();
      GUIUtils.recallPosition(roiManager);
      // "Get the "Show All" checkbox and make sure it is checked.
      Checkbox checkbox = (Checkbox) ((Panel) roiManager.getComponent(1)).getComponent(9);
      checkbox.setState(true);
      // Simulated click of the "Show All" checkbox to force ImageJ
      // to show all of the ROIs.
      roiManager.itemStateChanged(new ItemEvent(checkbox, 0, null, ItemEvent.SELECTED));
      return roiManager;
   }
   
   @Override
   public void dispose() {
      ProjectorControlForm.showSingleton(core_, app_).dispose();
      
   }

   @Override
   public void setApp(ScriptInterface app) {
      app_ = app;
      core_ = app_.getMMCore();
   }

   // Instantiate the ProjectorControlForm window if necessary, and show it
   // if it's not visible.
   @Override
   public void show() {
      if (core_.getSLMDevice().length()==0 && core_.getGalvoDevice().length()==0) {
         ReportingUtils.showMessage("Please load an SLM (Spatial Light Modulator) " +
               "or a Galvo-based phototargeting device " +
               "before using the Projector plugin.");
         return;
      }
      ProjectorControlForm.showSingleton(core_, app_);
   }

   public void configurationChanged() {
      throw new UnsupportedOperationException("Not supported yet.");
   }

   @Override
   public String getDescription() {
      return tooltipDescription;
   }

   @Override
   public String getInfo() {
      throw new UnsupportedOperationException("Not supported yet.");
   }

   @Override
   public String getVersion() {
      throw new UnsupportedOperationException("Not supported yet.");
   }

   @Override
   public String getCopyright() {
      throw new UnsupportedOperationException("Not supported yet.");
   }

}
