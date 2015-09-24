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

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JSeparator;
import javax.swing.JTextField;


class LogCaptureControlPanel extends ControlPanel {
   private final ProblemReportController controller_;

   private final JButton startOverButton_;
   private final JButton doneButton_;
   private final JButton cannotReproButton_;

   private final JLabel statusLabel_;
   private final JLabel instructionsLabel_;

   LogCaptureControlPanel(ProblemReportController controller) {
      controller_ = controller;

      JButton cancelButton = new JButton("Cancel");
      cancelButton.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent e) {
            controller_.cancelRequested();
         }
      });

      startOverButton_ = new JButton("Start Over");
      startOverButton_.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent e) {
            controller_.startLogCapture();
         }
      });

      doneButton_ = new JButton("Done");
      doneButton_.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent e) {
            controller_.finishLogCapture();
         }
      });

      cannotReproButton_ = new JButton("Cannot Reproduce");
      cannotReproButton_.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent e) {
            controller_.finishWithoutReproducing();
         }
      });

      statusLabel_ = new JLabel();
      statusLabel_.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
      setStatus("Preparing report...");

      instructionsLabel_ = new JLabel();
      instructionsLabel_.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
      setInstructions("Please wait.");

      final JTextField remarkTextField = new JTextField();
      final JButton remarkButton = new JButton("Insert");
      remarkButton.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent e) {
            String remark = remarkTextField.getText();
            if (!remark.isEmpty()) {
               controller_.insertTimestampedRemark(remark);
            }
         }
      });

      setLayout(new net.miginfocom.swing.MigLayout(
               "fillx, insets 0",
               "[fill]",
               "unrel[]unrel[]unrel[][]"));

      javax.swing.JProgressBar activityIndicator = new javax.swing.JProgressBar();
      activityIndicator.setIndeterminate(true);

      add(statusLabel_, "split 2, grow, gapright related");
      add(activityIndicator, "wrap");

      add(instructionsLabel_, "grow, wrap");

      add(startOverButton_, "split 3, sizegroup btns, gapright push");
      add(cannotReproButton_, "sizegroup btns");
      add(doneButton_, "sizegroup btns, wrap");

      add(cancelButton, "gapright push, wrap");

      add(new JSeparator(), "grow, wrap");
      add(new JLabel("Insert timestamped remark into log:"),
            "align left, wrap");
      add(remarkTextField, "split 2, grow, gapright related");
      add(remarkButton, "");
   }

   void setStatus(String status) {
      statusLabel_.setText(status);
   }

   void setInstructions(String instructions) {
      instructionsLabel_.setText(instructions);
   }

   void setButtonsEnabled(boolean flag) {
      startOverButton_.setEnabled(flag);
      cannotReproButton_.setEnabled(flag);
      doneButton_.setEnabled(flag);
   }
}
