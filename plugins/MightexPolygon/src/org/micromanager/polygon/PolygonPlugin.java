/*
 * Mightex Polygon400 plugin.
 *
 * AUTHOR:
 * Wayne Liao, mightexsystem.com, 05/15/2015
 *
 * Copyright (c) 2015 Mightex Systems
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

package org.micromanager.polygon;

import mmcorej.CMMCore;
import org.micromanager.api.ScriptInterface;
import org.micromanager.utils.ReportingUtils;

/**
 *
 * @author Wayne
 */
public class PolygonPlugin implements org.micromanager.api.MMPlugin {
    public static String menuName = "Mightex Polygon Plugin";
    public static String tooltipDescription = "Plugin for Mightex Polygon400 device to control pattern projection";

    private ScriptInterface app_;
    private CMMCore core_;

    private PolygonForm form_;

    @Override
    public void dispose() {
    }

    @Override
    public void setApp(ScriptInterface app) {
        app_ = app;
        core_ = app.getMMCore();
   }

   @Override
   public void show() {
        if(core_.getSLMDevice().length()==0){
            Utility.LogMsg( "SLM device count = " + core_.getSLMDevice().length() );
            ReportingUtils.showMessage("Please load a Mightex Polygon400 device before using the MightexPolygon plugin.");
            return;
        }
        if(form_==null) {
            form_ = new PolygonForm(app_);
        }
        form_.setVisible(true);
   }

   @Override
   public String getDescription() {
      return tooltipDescription;
   }

   @Override
   public String getInfo() {
      return "Info: Mightex Polygon Plugin";
   }

   @Override
   public String getVersion() {
      return "1.0";
   }

   @Override
   public String getCopyright() {
      return "(C) 2014 Mightex Systems. This software is released under the BSD license";
   }
  
}
