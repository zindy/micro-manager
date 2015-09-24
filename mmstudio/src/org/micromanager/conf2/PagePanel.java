/*
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, October 29, 2006
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

package org.micromanager.conf2;

import java.awt.Color;
import java.awt.Dialog;
import java.util.prefs.Preferences;

import javax.swing.JOptionPane;
import javax.swing.JPanel;

import mmcorej.CMMCore;
import org.micromanager.MMStudio;
import org.micromanager.utils.ReportingUtils;

/**
 * Wizard panel used as the abstract base class for all pages.
 */
public abstract class PagePanel extends JPanel{
   private static final long serialVersionUID = -4598248516499305300L;
   protected MicroscopeModel model_;
   protected CMMCore core_;
   protected Preferences prefs_;
   protected String title_;
   protected String helpText_;
   protected String helpFileName_;
   protected Dialog parent_;
   
   protected static final String CFG_PATH = "cfg_path";
   
   public PagePanel() {
      super();
      title_ = new String();
      MMStudio mmf = MMStudio.getInstance();
      if (mmf != null)
    	  setBackground(MMStudio.getInstance().getBackgroundColor());
   }

   public void setModel(MicroscopeModel model, CMMCore core) {
      model_ = model;
      core_ = core;
   }
   
   public void setTitle(String txt) {
      title_ = txt;
   }
   
   public void setHelpText(String txt) {
      helpText_ = txt;
   }
   
   public void setHelpFileName(String name) {
      helpFileName_ = name;
   }
   
   public String getHelpText() {
      return helpText_;
   }
   
   public String getTitle() {
      return title_;
   }
   
   public void setPrefrences(Preferences prefs) {
      prefs_ = prefs;
   }
      
   public String getHelpFileName() {
      return helpFileName_;
   }

   public void setParentDialog(Dialog p) {
      this.parent_ = p;
   }

   public abstract boolean enterPage(boolean next);
   
   public abstract boolean exitPage(boolean next);
   
   public abstract void loadSettings();
   
   public abstract void saveSettings();

   
   protected void handleError(String txt) {
      JOptionPane.showMessageDialog(this, txt);           
   }
   protected void handleException(Exception e) {
      ReportingUtils.showError(e);
   }

   public abstract void refresh();

}
