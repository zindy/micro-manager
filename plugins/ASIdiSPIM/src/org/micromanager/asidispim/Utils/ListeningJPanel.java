/*
 * AUTHOR:
 * Nico Stuurman, Jon Daniels
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

import java.awt.LayoutManager;

import javax.swing.JPanel;

/**
 * Extension of JPanel that adds a few callbacks making it possible for the
 * enclosing frame to easily inform tabs of events
 * @author nico
 */
@SuppressWarnings("serial")
public class ListeningJPanel extends JPanel {
   
   protected String panelName_;
   
   public ListeningJPanel(String panelName, LayoutManager l) {
      super (l);
      panelName_ = panelName;
   }
   
   /**
    * Will be called when this Panel is selected in the parent TabbedPanel 
    */
   public void gotSelected() {};
   
   /**
    * Will be called when this Panel was active in the parent TabbedPanel
    *  and another one is becoming active instead 
    */
   public void gotDeSelected() {};
   
   /**
    * Should force the panel to write its current settings to its preferences
    */
   public void saveSettings() {};
   
   /**
    * Called when the plugin window is closing
    */
   public void windowClosing() {};
   
   /**
    * Called when new stage positions are available in the device class
    */
   public void updateStagePositions() {};
   
   /**
    * Called when stage position updates have stopped
    */
   public void stoppedStagePositions() {};
   
   /**
    * returns the name of the panel 
    * @return - panelName
    */
   public String getPanelName() { return panelName_; }
   
   /**
    * Called when the display should be refreshed, sort of like
    * user-defined version of repaint().  Used to refresh duration
    * labels in Acquisition tab when channels change.
    */
   public void refreshDisplay() {};
   
   /**
    * Called when the Panel loses focus but then regains it and certain
    * tasks need to be accomplished. For example after autofocus.
    */
   public void refreshSelected() {};
   
}
