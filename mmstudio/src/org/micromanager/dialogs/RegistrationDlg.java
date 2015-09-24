/*
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, March 20, 2006
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

package org.micromanager.dialogs;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Insets;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.prefs.Preferences;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import org.micromanager.utils.ReportingUtils;

public class RegistrationDlg extends JDialog {
   private static final long serialVersionUID = 1L;
   public static final String REGISTRATION = "registered";
   public static final String REGISTRATION_NEVER = "reg_never";
   public static final String REGISTRATION_ATTEMPTS = "registration_attempts";
   public static final String REGISTRATION_NAME = "reg_name";
   public static final String REGISTRATION_INST = "reg_institution";

   private JTextArea welcomeTextArea_;
   private JTextField email_;
   private JTextField inst_;
   private JTextField name_;
   private Preferences prefs_;

   /**
    * Dialog to collect registration data from the user.
    */
   public RegistrationDlg(Preferences prefs) {
      super();
      prefs_ = prefs;
      
      incrementRegistrationAttempts();
      
      setModal(true);
      setUndecorated(true);
      setTitle("Micro-Manager Registration");
      getContentPane().setLayout(null);
      setResizable(false);
      setBounds(100, 100, 398, 299);

      Dimension winSize = getSize();
      Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
      setLocation(screenSize.width/2 - (winSize.width/2), screenSize.height/2 - (winSize.height/2));

      final JLabel nameLabel = new JLabel();
      nameLabel.setText("Name");
      nameLabel.setBounds(10, 198, 65, 14);
      getContentPane().add(nameLabel);

      final JLabel institutionLabel = new JLabel();
      institutionLabel.setText("Institution");
      institutionLabel.setBounds(10, 220, 65, 14);
      getContentPane().add(institutionLabel);

      final JLabel emailLabel = new JLabel();
      emailLabel.setText("Email");
      emailLabel.setBounds(10, 240, 65, 14);
      getContentPane().add(emailLabel);

      name_ = new JTextField();
      name_.setBounds(77, 195, 256, 20);
      getContentPane().add(name_);

      inst_ = new JTextField();
      inst_.setBounds(77, 218, 256, 20);
      getContentPane().add(inst_);

      email_ = new JTextField();
      email_.setBounds(77, 240, 256, 20);
      getContentPane().add(email_);

      final JButton okButton = new JButton();
      okButton.setFont(new Font("", Font.BOLD, 12));
      okButton.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent arg0) {
            if (name_.getText().length() == 0 || email_.getText().length() == 0) {
               JOptionPane.showMessageDialog(RegistrationDlg.this, "Name and email fields can't be empty.");
            } else {
               try {
                  URL url;
                  InputStream is;
                  BufferedReader br;
                  
                  // save registration information to registry
                  prefs_.put(REGISTRATION_NAME, name_.getText());
                  prefs_.put(REGISTRATION_INST, inst_.getText());
                  
                  // replace special characters to properly format the command string
                  String name = name_.getText().replaceAll("[ \t]", "%20");
                  name = name.replaceAll("[&]", "%20and%20");
                  String inst = inst_.getText().replaceAll("[ \t]", "%20");
                  inst = inst.replaceAll("[&]", "%20and%20");
                  String email = email_.getText().replaceAll("[ \t]", "%20");
                  email = email.replaceAll("[&]", "%20and%20");
                  
                  String regText = "http://valelab.ucsf.edu/micro-manager-registration.php?Name=" + name +
                  "&Institute=" + inst + "&email=" + email;
                  
                  url = new URL(regText);
                  is = url.openStream();
                  br = new BufferedReader(new InputStreamReader(is));
                  String response = br.readLine();
                  if (response.compareTo("SUCCESS") != 0) {
                     JOptionPane.showMessageDialog(RegistrationDlg.this, "Registration did not succeed. You will be prompted again next time.");
                     dispose();
                     return;
                  }
                  
                  } catch (java.net.UnknownHostException e) {
                     ReportingUtils.showError(e, "Registration did not succeed. You are probably not connected to the Internet.\n" +
                                                   "You will be prompted again next time you start.");
                  } catch (MalformedURLException e) {
                     ReportingUtils.showError(e);
                  } catch (IOException e) {
                     ReportingUtils.showError(e);
                  } catch (SecurityException e){
                     ReportingUtils.showError(e,
                           "\nThe program failed to save registration status.\n" +
                           "Most likely you are not logged in with administrator privileges.\n" +
                     "Please try registering again using the administrator's account.");
   
                  } catch (Exception e) {
                     ReportingUtils.logError(e);
                  } finally {
                     dispose();
                  
                  }   
                  // save to registry
                  prefs_.putBoolean(REGISTRATION, true);
            }

         }
      });
      okButton.setText("OK");
      okButton.setBounds(50, 266, 100, 23);
      getContentPane().add(okButton);

      welcomeTextArea_ = new JTextArea();
      welcomeTextArea_.setMargin(new Insets(10, 10, 10, 10));
      welcomeTextArea_.setLineWrap(true);
      welcomeTextArea_.setBackground(new Color(192, 192, 192));
      welcomeTextArea_.setFocusable(false);
      welcomeTextArea_.setEditable(false);
      welcomeTextArea_.setFont(new Font("Arial", Font.PLAIN, 12));
      welcomeTextArea_.setWrapStyleWord(true);
      welcomeTextArea_.setText("Welcome to Micro-Manager.\n\n" +
            "Please take a minute to let us know that you are using this software. " +
            "The information you enter will only be used to estimate how many people use Micro-Manager." +
            " Accurate tracking of the number of users is very important for our funding efforts and essential for the long-term survival of Micro-Manager.\n\n" +
            "Your information will never be made public or given away to third parties, however, we might very occasionally inform you of Micro-Manager developments.");
      welcomeTextArea_.setBounds(10, 10, 378, 179);
      getContentPane().add(welcomeTextArea_);

      final JButton skipButton = new JButton();
      skipButton.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent arg0) {
            JOptionPane.showMessageDialog(RegistrationDlg.this, "You choose to postpone registration.\n" +
                  "This prompt will appear again next time you start the application.");
            prefs_.putBoolean(REGISTRATION_NEVER,false);
            dispose();
         }
      });
      skipButton.setText("Later");
      skipButton.setBounds(160, 266, 100, 23);
      getContentPane().add(skipButton);
      //
      
      if (getRegistrationAttempts() > 1) {        // Don't show "never" button the first time
         final JButton neverButton = new JButton();
         neverButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent arg0) {
               JOptionPane.showMessageDialog(RegistrationDlg.this, "You have chosen never to register. \n" +
                     "If you change your mind in the future, please\nchoose the \"Register\" option in the Help menu.");
               prefs_.putBoolean(REGISTRATION_NEVER,true);
               dispose();
            }
         });
         
         neverButton.setText("Never");
         neverButton.setBounds(270, 266, 100, 23);
         getContentPane().add(neverButton);
      }
      
   }
   
   private int incrementRegistrationAttempts() {
      int attempts = getRegistrationAttempts();
      attempts++;
      prefs_.putInt(REGISTRATION_ATTEMPTS, attempts);
      return attempts;
   }
   
   public final int getRegistrationAttempts() {
      return prefs_.getInt(REGISTRATION_ATTEMPTS,0);
   }

}
