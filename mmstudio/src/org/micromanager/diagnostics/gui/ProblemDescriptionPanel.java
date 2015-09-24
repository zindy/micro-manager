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

import javax.swing.JScrollPane;
import javax.swing.JTextArea;

class ProblemDescriptionPanel extends javax.swing.JPanel {
   private final ProblemReportController controller_;

   ProblemDescriptionPanel(ProblemReportController controller) {
      super(new net.miginfocom.swing.MigLayout(
              "fillx, filly, insets 0",
              "[grow, fill]",
              "[]related[grow, fill]"));

      controller_ = controller;

      final JTextArea descriptionTextArea = makeDescriptionTextArea();

      final JScrollPane scrollPane = new JScrollPane(descriptionTextArea,
               JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
               JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
      controller_.setDescriptionTextArea(descriptionTextArea);

      add(new javax.swing.JLabel("Problem Description:"), "gapafter push, wrap");
      add(scrollPane, "width 400:400, height 200:200");
   }

   private JTextArea makeDescriptionTextArea() {
      final JTextArea textArea = new JTextArea();
      textArea.setFont(new java.awt.Font(
               java.awt.Font.MONOSPACED,
               java.awt.Font.PLAIN,
               12));
      textArea.setLineWrap(true);
      textArea.setWrapStyleWord(true);

      // Ugly on OS X, but otherwise invisible on Windows
      textArea.setBorder(new javax.swing.border.LineBorder(java.awt.Color.BLACK));

      textArea.getDocument().addDocumentListener(new javax.swing.event.DocumentListener() {
         public void insertUpdate(javax.swing.event.DocumentEvent e) {
            controller_.markDescriptionModified();
         }
         public void removeUpdate(javax.swing.event.DocumentEvent e) {
            controller_.markDescriptionModified();
         }
         public void changedUpdate(javax.swing.event.DocumentEvent e) {
            controller_.markDescriptionModified();
         }
      });

      return textArea;
   }
}
