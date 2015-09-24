/*
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, Dec 1, 2005
 *
 * Copyright (c) 2006 Regents of the University of California
 * Copyright (c) 2008 100X Imaging Inc, www.100ximaging.com
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

package org.micromanager.dialogs;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Insets;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JTextArea;
import javax.swing.WindowConstants;
import javax.swing.border.BevelBorder;
import javax.swing.border.LineBorder;

import com.swtdesigner.SwingResourceManager;

/**
 * Help | About dialog for MMStudio.
 * Displays version, license, info, etc.
 */
public class MMAboutDlg extends JDialog {
   private static final long serialVersionUID = 1L;
   private JTextArea welcomeTextArea_;
   private JTextArea homeHttphcs100ximagingcomBugTextArea;
   private JTextArea versionInfo_;
   
   public static String COPYRIGHT_TEXT = 
      
      "Copyright University of California San Francisco, 2010. All rights reserved.\n\n" +
      "Additional copyright on portions of this software by the following institutions, projects or individuals:" +
      " Wayne Rasband, NIH, Joachim Walter, BeanShell, JSON, logix4u, libserial, boost.org, Todd Klark, Ramon de Klein, " +
      "MIT, University of Dundee, Board of Regents of the University of Wisconsin-Madison, Glencoe Software, and  SLF4J";
   
   public MMAboutDlg() {
      super();
      Dimension winSize = new Dimension(384, 392);
      setSize(winSize);
      setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
      setName("aboutDlg");
      // Java 1.5 specific:
      //setAlwaysOnTop(true);
      setResizable(false);
      setModal(true);
      getContentPane().setLayout(null);
      setTitle("About Micro-Manager 1.4");
      
      Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
      setLocation(screenSize.width/2 - (winSize.width/2), screenSize.height/2 - (winSize.height/2));

      final JLabel micromanageLabel = new JLabel();
      micromanageLabel.setFont(new Font("", Font.BOLD, 16));
      micromanageLabel.setText("Micro-Manager 1.4");
      micromanageLabel.setBounds(44, 11, 176, 23);
      getContentPane().add(micromanageLabel);

      final JLabel openSourceAutomatedLabel = new JLabel();
      openSourceAutomatedLabel.setFont(new Font("Arial", Font.PLAIN, 10));
      openSourceAutomatedLabel.setText("The Open Source Microscopy Software");
      openSourceAutomatedLabel.setBounds(44, 30, 329, 18);
      getContentPane().add(openSourceAutomatedLabel);

      final JTextArea citeUs = new JTextArea();
      citeUs.setFont(new Font("Arial", Font.PLAIN, 10));
      citeUs.setEditable(false);
      citeUs.setLineWrap(true);
      citeUs.setWrapStyleWord(true);
      citeUs.setBorder(new LineBorder(Color.black, 1, false));
      citeUs.setBackground(new Color(192, 192, 192));
      citeUs.setText("If you've found this software useful, please cite Micro-Manager in your publications.");
      citeUs.setBounds(5, 277, 368, 40);
      getContentPane().add(citeUs);


      final JButton okButton = new JButton();
      okButton.setFont(new Font("Arial", Font.PLAIN, 10));
      okButton.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent e) {
            dispose();
         }
      });
      okButton.setText("OK");
      okButton.setBounds(145, 322, 91, 28);
      getContentPane().add(okButton);

      versionInfo_ = new JTextArea();
      versionInfo_.setEditable(false);
      versionInfo_.setBorder(new BevelBorder(BevelBorder.LOWERED));
      versionInfo_.setFont(new Font("Arial", Font.PLAIN, 10));
      versionInfo_.setBounds(5, 49, 368, 66);
      getContentPane().add(versionInfo_);

      homeHttphcs100ximagingcomBugTextArea = new JTextArea();
      homeHttphcs100ximagingcomBugTextArea.setEditable(false);
      homeHttphcs100ximagingcomBugTextArea.setBorder(new LineBorder(Color.black, 1, false));
      homeHttphcs100ximagingcomBugTextArea.setBackground(new Color(192, 192, 192));
      homeHttphcs100ximagingcomBugTextArea.setFont(new Font("Courier New", Font.PLAIN, 12));
      homeHttphcs100ximagingcomBugTextArea.setText(" home:               http://www.micro-manager.org\r\n bug reports:        bugs@micro-manager.org\r\n feature requests:   features@micro-manager.org\r\n");
      homeHttphcs100ximagingcomBugTextArea.setBounds(5, 219, 368, 47);
      getContentPane().add(homeHttphcs100ximagingcomBugTextArea);

      final JLabel label = new JLabel();
      label.setIcon(SwingResourceManager.getIcon(MMAboutDlg.class, "/org/micromanager/icons/microscope.gif"));
      label.setBounds(6, 14, 32, 32);
      getContentPane().add(label);

      welcomeTextArea_ = new JTextArea();
      welcomeTextArea_.setBorder(new LineBorder(Color.black, 1, false));
      welcomeTextArea_.setWrapStyleWord(true);
      welcomeTextArea_.setText("Copyright University of California San Francisco, 2010. All rights reserved.\r\nCopyright 100X Imaging Inc, 2010. All rights reserved\r\n\r\nAdditional copyright on portions of this software by the following institutions, projects or individuals: Wayne Rasband, NIH, Joachim Walter, boost.org, BeanShell, JSON, logix4u, libserial, Todd Klark and Ramon de Klein");
      welcomeTextArea_.setMargin(new Insets(10, 10, 10, 10));
      welcomeTextArea_.setLineWrap(true);
      welcomeTextArea_.setFont(new Font("Arial", Font.PLAIN, 10));
      welcomeTextArea_.setFocusable(false);
      welcomeTextArea_.setEditable(false);
      welcomeTextArea_.setBackground(new Color(192, 192, 192));
      welcomeTextArea_.setBounds(5, 126, 368, 87);
      getContentPane().add(welcomeTextArea_);
   }
   
   public void setVersionInfo(String verText) {
      versionInfo_.setText(verText);
   }

}
