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

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;

import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;

import mmcorej.CMMCore;
import mmcorej.DeviceType;
import mmcorej.StrVector;

/**
 * This class allows the user to select a StageDevice or XYStageDevice from
 * a popup menu.
 */
class MergeStageDevicePopupMenu extends JPopupMenu {
   public MergeStageDevicePopupMenu(final PositionListDlg parent, CMMCore core) {
      Vector<String> options = new Vector<String>();
      StrVector xyStages = core.getLoadedDevicesOfType(DeviceType.XYStageDevice);
      for (int i = 0; i < xyStages.size(); ++i) {
         options.add(xyStages.get(i));
      }
      StrVector stages = core.getLoadedDevicesOfType(DeviceType.StageDevice);
      for (int i = 0; i < stages.size(); ++i) {
         options.add(stages.get(i));
      }

      for (int i = 0; i < options.size(); ++i) {
         final String deviceName = options.get(i);
         JMenuItem item = new JMenuItem(String.format("Merge with %s current position", deviceName));
         item.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent event) {
               parent.mergePositionsWithDevice(deviceName);
            }
         });
         add(item);
      }
   }
}
