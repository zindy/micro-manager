/*
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, Jul 18, 2005
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

import ij.CommandListener;
import ij.Executer;
import ij.IJ;
import ij.plugin.Duplicator;
import ij.plugin.PlugIn;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;

import mmcorej.CMMCore;

import org.micromanager.MMStudio;
import org.micromanager.imagedisplay.AcquisitionVirtualStack;
import org.micromanager.utils.AutofocusManager;
import org.micromanager.utils.GUIUtils;
import org.micromanager.utils.JavaUtils;
import org.micromanager.utils.ReportingUtils;

/**
 * ImageJ plugin wrapper for Micro-Manager.
 */
public class MMStudioPlugin implements PlugIn, CommandListener {

   static MMStudio studio_;

   @SuppressWarnings("unchecked")
   @Override
   public void run(final String arg) {

      SwingUtilities.invokeLater(new Runnable() {
         @Override
         public void run() {
            try {
               if (studio_ == null || !studio_.getIsProgramRunning()) {
                  // OS-specific stuff
                  if (JavaUtils.isMac()) {
                     System.setProperty("apple.laf.useScreenMenuBar", "true");
                  }
                  try {
                     UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
                  } catch (Exception e) {
                     ReportingUtils.logError(e);
                  }


                  // create and display control panel frame
                  // warn user about old ImageJ version, but do not stop
                  IJ.versionLessThan("1.48g");
                  
                  if (!IJ.versionLessThan("1.46e")) {
                     Executer.addCommandListener(MMStudioPlugin.this);
                  }
                  studio_ = new MMStudio(true);
                  MMStudio.getFrame().setVisible(true);
                  MMStudio.getFrame().setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
               }
               if (arg.equals("OpenAcq")) {
                  studio_.promptForAcquisitionToOpen(true);
               }
            } catch (Exception e) {
               ReportingUtils.logError(e);
            }
         }
      });
   }
   
   private boolean frameSuccessfullyClosed = true;
   private void setFrameClosingResult(boolean res) {
      frameSuccessfullyClosed = res;
   }
   
   @Override
   public String commandExecuting(String command) { 
      if (command.equalsIgnoreCase("Quit") && studio_ != null) {
         try {
            GUIUtils.invokeAndWait(new Runnable() {
               public void run() {
                  boolean result = true;
                  if (!studio_.closeSequence(true)) {
                     result = false;
                  }
                  setFrameClosingResult(result);
               }
            });
         } catch (Exception ex) {
            // do nothing, just make sure to continue quitting
         }
         if (!frameSuccessfullyClosed) {
            return null;
         }
         return command;
      }  else if (command.equals("Crop")) {
         if (IJ.getImage().getStack() instanceof AcquisitionVirtualStack) {

            new Duplicator().run(IJ.getImage()).show();

            // abort further processing of the Crop command
            return null;
         } 
      } else if (command.equals("Add Noise")) {
         // blanket method to make sure that ImageJ filters do not execute on disk-cached images
         // this may backfire!
         if (IJ.getImage().getStack() instanceof AcquisitionVirtualStack) {
            AcquisitionVirtualStack avs = (AcquisitionVirtualStack) IJ.getImage().getStack();
            if (avs.getVirtualAcquisitionDisplay().isDiskCached()) {
               // duplicate the image and then run the ImageJ command on what is now the new image
               new Duplicator().run(IJ.getImage()).show();
            } else {
               // Image is not disk chached.  Warn that data will be lost
               if (!IJ.showMessageWithCancel("Micro-Manager data not saved", "Data are not saved and Undo is impossible. \n" +
                       "Do you really want to execute the command?") ) {
                  return null;
               }
            }
         }
      }

      return command;
   }
}
