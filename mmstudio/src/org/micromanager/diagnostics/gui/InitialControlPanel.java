/*
 * AUTHOR:
 * Mark Tsuchida
 *
 * Copyright (c) 2014 Regents of the University of California
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

package org.micromanager.diagnostics.gui;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import javax.swing.JButton;
import javax.swing.JCheckBox;


class InitialControlPanel extends ControlPanel {
   private final ProblemReportController controller_;

   InitialControlPanel(ProblemReportController controller) {
      controller_ = controller;

      JButton cancelButton = new JButton("Cancel");
      cancelButton.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent e) {
            controller_.cancelRequested();
         }
      });

      JButton startButton = new JButton("Generate Report...");
      startButton.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent e) {
            controller_.startLogCapture();
         }
      });

      final JCheckBox crashRobustCheckBox =
         new JCheckBox("Use crash-robust logging");
      crashRobustCheckBox.setSelected(controller_.getUseCrashRobustLogging());
      crashRobustCheckBox.setToolTipText("Prevents lost log entries. " +
            "Uncheck if it masks the problem to be reported.");
      crashRobustCheckBox.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent e) {
            controller_.setUseCrashRobustLogging(crashRobustCheckBox.isSelected());
         }
      });

      add(cancelButton, "align left");
      add(startButton, "align right, wrap");
      add(crashRobustCheckBox, "align left, span 2");
   }
}
