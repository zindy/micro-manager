/*
 * AUTHOR:
 * Andrej Bencur, abencur@photometrics.com, August 28, 2015
 *
 * Copyright (c) 2015 QImaging, Surrey, BC
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

package PMQI;

import java.util.logging.Level;
import java.util.logging.Logger;
import org.micromanager.MMStudio;
import org.micromanager.api.ScriptInterface;
import org.micromanager.utils.ReportingUtils;

/**
 *
 * @author Andrej
 */
public class AutoWB implements org.micromanager.api.MMPlugin {

    public static String menuName = "PM/QI Auto White Balance";
    public static String tooltipDescription = "Runs automatic white balance algorithm";
    private MMStudio gui_;
    private WhiteBalance_UI wbForm_;

    @Override
    public void dispose() {

    }

    @Override
    public void setApp(ScriptInterface app) {
        gui_ = (MMStudio) app;

        try {
            wbForm_ = new WhiteBalance_UI(gui_);
            gui_.addMMListener(wbForm_);
            gui_.addMMBackgroundListener(wbForm_);
        } catch (Exception e) {
            Logger.getLogger(WhiteBalance_UI.class.getName()).log(Level.SEVERE, null, e);
            ReportingUtils.showError(e);
        }
        wbForm_.setVisible(true);

    }

    @Override
    public void show() {
        String msg = "PMQI White Balance";
    }

    @Override
    public String getDescription() {
        return "Automatic White Balance scaling calculator for PVCAM compatible Photometrics and QImaging cameras";
    }

    @Override
    public String getInfo() {
        return "PMQI Automatic White Balance";
    }

    @Override
    public String getVersion() {
        return "1.0";
    }

    @Override
    public String getCopyright() {
        return "(C) 2015 Photometrics/QImaging";
    }

}
