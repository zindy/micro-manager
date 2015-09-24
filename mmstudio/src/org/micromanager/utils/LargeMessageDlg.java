/*
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, October 1, 2006
 *
 * Copyright (c) 2006 Regents of the University of California
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

package org.micromanager.utils;

import java.awt.Dimension;
import java.awt.Font;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.SpringLayout;

/**
 * Dialog box for displaying large error text messages.
 */
public class LargeMessageDlg extends JDialog {
   private static final long serialVersionUID = -2477635586817637967L;
   private SpringLayout springLayout;
   private JScrollPane scrollPane_;

   /**
    * Create the dialog
    */
   public LargeMessageDlg(String title, String message) {
      super();
      addWindowListener(new WindowAdapter() {
         @Override
         public void windowOpened(WindowEvent e) {
            scrollPane_.getVerticalScrollBar().setValue(0);
         }
      });
      setResizable(false);
      springLayout = new SpringLayout();
      getContentPane().setLayout(springLayout);
      setTitle(title);
      setModal(true);
      setBounds(100, 100, 507, 327);

      Dimension winSize = getSize();
      Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
      setLocation(screenSize.width/2 - (winSize.width/2), screenSize.height/2 - (winSize.height/2));

      final JButton okButton = new JButton();
      okButton.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent arg0) {
            dispose();
         }
      });
      okButton.setText("OK");
      getContentPane().add(okButton);
      springLayout.putConstraint(SpringLayout.SOUTH, okButton, 288, SpringLayout.NORTH, getContentPane());
      springLayout.putConstraint(SpringLayout.NORTH, okButton, -30, SpringLayout.SOUTH, getContentPane());
      springLayout.putConstraint(SpringLayout.EAST, okButton, 291, SpringLayout.WEST, getContentPane());
      springLayout.putConstraint(SpringLayout.WEST, okButton, 208, SpringLayout.WEST, getContentPane());

      scrollPane_ = new JScrollPane();
      getContentPane().add(scrollPane_);
      springLayout.putConstraint(SpringLayout.SOUTH, scrollPane_, -35, SpringLayout.SOUTH, getContentPane());
      springLayout.putConstraint(SpringLayout.NORTH, scrollPane_, 5, SpringLayout.NORTH, getContentPane());
      springLayout.putConstraint(SpringLayout.EAST, scrollPane_, -5, SpringLayout.EAST, getContentPane());
      springLayout.putConstraint(SpringLayout.WEST, scrollPane_, 5, SpringLayout.WEST, getContentPane());

      final JTextArea textPane = new JTextArea();
      textPane.setFont(new Font("Arial", Font.PLAIN, 12));
      textPane.setWrapStyleWord(true);
      textPane.setLineWrap(true);
      textPane.setEditable(false);
      textPane.setText(message);
      scrollPane_.setViewportView(textPane);
      //
   }

}
