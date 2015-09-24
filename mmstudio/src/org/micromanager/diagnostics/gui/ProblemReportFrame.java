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

class ProblemReportFrame extends javax.swing.JFrame {
   private final ProblemReportController controller_;

   private final ProblemDescriptionPanel descriptionPanel_;
   private ControlPanel controlPanel_;

   private final javax.swing.JPanel controlPanelPanel_;

   ProblemReportFrame(ProblemReportController controller) {
      super("Problem Report");
      setDefaultCloseOperation(javax.swing.WindowConstants.DO_NOTHING_ON_CLOSE);
      setLocationRelativeTo(null);

      controller_ = controller;

      addWindowListener(new java.awt.event.WindowAdapter() {
         @Override
         public void windowClosing(java.awt.event.WindowEvent e) {
            controller_.cancelRequested();
         }
      });

      descriptionPanel_ = new ProblemDescriptionPanel(controller);
      controlPanel_ = new InitialControlPanel(controller);

      controlPanelPanel_ = new javax.swing.JPanel();
      controlPanelPanel_.setLayout(new net.miginfocom.swing.MigLayout(
               "fill, insets 0",
               "[grow, fill]",
               "[grow, fill]"));
      controlPanelPanel_.add(controlPanel_);

      setLayout(new net.miginfocom.swing.MigLayout(
               "fill, insets dialog",
               "[grow, fill]",
               "[grow, fill]unrelated[]"));
      add(descriptionPanel_, "wrap");
      add(controlPanelPanel_);
      pack();
      setMinimumSize(getPreferredSize());
   }

   void close() {
      dispose();
   }

   void setControlPanel(final ControlPanel newPanel) {
      int saveWidth = getWidth();

      // Preserve the size of the description panel
      int descriptionPanelHeight = descriptionPanel_.getHeight();
      int defaultDescriptionPanelHeight = descriptionPanel_.getPreferredSize().height;
      int saveExtraHeight = descriptionPanelHeight - defaultDescriptionPanelHeight;

      controlPanelPanel_.remove(controlPanel_);
      controlPanel_ = newPanel;
      controlPanelPanel_.add(controlPanel_);

      validate();
      repaint();

      setMinimumSize(getPreferredSize());

      int newFrameHeight = getPreferredSize().height + saveExtraHeight;
      setSize(saveWidth, newFrameHeight);
   }

   ControlPanel getControlPanel() {
      return controlPanel_;
   }
}
