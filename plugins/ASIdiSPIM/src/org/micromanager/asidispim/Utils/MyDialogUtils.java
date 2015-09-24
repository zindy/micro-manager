/*
 * AUTHOR:
 * Jon Daniels, Nico Stuurman
 *
 * Copyright (c) 2013 Regents of the University of California, & ASI
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

package org.micromanager.asidispim.Utils;

import javax.swing.JOptionPane;

import org.micromanager.asidispim.ASIdiSPIM;

import org.micromanager.utils.ReportingUtils;

/**
 *
 * @author Jon
 */
public class MyDialogUtils {

   
   public MyDialogUtils() {
   }
   
   /**
    * Shows a confirmation diaglog and returns true if OK/Yes was selected.
    * Centers over the plugin frame
    * @param prompt the string to display
    * @param optionType one of JOptionPane.YES_NO_OPTION or JOptionPane.OK_CANCEL_OPTION
    * @return true if user said "Yes" or "OK", false otherwise
    */
   public static boolean getConfirmDialogResult(String prompt, int optionType) {
      int dialogResult = JOptionPane.showConfirmDialog(ASIdiSPIM.getFrame(),
            prompt,
            "Warning",
            optionType);
      switch (optionType) {
      case JOptionPane.YES_NO_OPTION:
         return (dialogResult == JOptionPane.YES_OPTION);
      case JOptionPane.OK_CANCEL_OPTION:
         return (dialogResult == JOptionPane.OK_OPTION);
      default:
            return false;
      }
   }
   
   /**
    * Convenience method to show an error message (also logged) over the plugin frame.
    * Calls org.micromanager.utils.ReportingUtils() 
    * @param message
    */
   public static void showError(String message) {
      ReportingUtils.showError(message, ASIdiSPIM.getFrame());
   }
   
   /**
    * Convenience method to show an error message (also logged) over the plugin frame.
    * Calls org.micromanager.utils.ReportingUtils() 
    * @param e exception
    * @param message
    */
   public static void showError(Throwable e, String message) {
      ReportingUtils.showError(e, message, ASIdiSPIM.getFrame());
   }
   
   /**
    * Convenience method to show an error message (also logged) over the plugin frame.
    * Calls org.micromanager.utils.ReportingUtils() 
    * @param e exception
    */
   public static void showError(Throwable e) {
      ReportingUtils.showError(e, ASIdiSPIM.getFrame());
   }
}
