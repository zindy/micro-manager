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

package org.micromanager.positionlist;

import java.util.ArrayList;
import java.util.prefs.Preferences;

import mmcorej.CMMCore;
import mmcorej.DeviceType;
import mmcorej.StrVector;

import org.micromanager.utils.ReportingUtils;

/**
 * List with Axis data.  Currently, we use only a single global instance 
 * of this class
 */
class AxisList {
   private ArrayList<AxisData> axisList_ = new ArrayList<AxisData>();
   private CMMCore core_;
   private Preferences prefs_;
   
   public AxisList(CMMCore core, Preferences prefs) {
      core_ = core;
      prefs_ = prefs;
      // Initialize the axisList.
      try {
         // add 1D stages
         StrVector stages = core_.getLoadedDevicesOfType(DeviceType.StageDevice);
         for (int i=0; i<stages.size(); i++) {
            axisList_.add(new AxisData(prefs_.getBoolean(stages.get(i),true), 
                    stages.get(i), AxisData.AxisType.oneD));
         }
         // read 2-axis stages
         StrVector stages2D = core_.getLoadedDevicesOfType(DeviceType.XYStageDevice);
         for (int i=0; i<stages2D.size(); i++) {
            axisList_.add(new AxisData(prefs_.getBoolean(stages2D.get(i),true), 
                    stages2D.get(i), AxisData.AxisType.twoD));
         }
      } catch (Exception e) {
         ReportingUtils.showError(e);
      }
   }
   public AxisData get(int i) {
      if (i >=0 && i < axisList_.size()) {
         return axisList_.get(i);
      }
      return null;
   }
   public int getNumberOfPositions() {
      return axisList_.size();
   }
   public boolean use(String axisName) {
      for (int i=0; i< axisList_.size(); i++) {
         if (axisName.equals(get(i).getAxisName())) {
            return get(i).getUse();
         }
      }
      // not in the list??  It might be time to refresh the list.  
      return true;
   }
      
}
