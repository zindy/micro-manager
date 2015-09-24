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

import java.awt.Container;
import java.awt.Cursor;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.util.prefs.Preferences;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JTextField;
import org.micromanager.MMStudio;
import org.micromanager.utils.FileDialogs;

import org.micromanager.utils.GUIUtils;
import org.micromanager.utils.ReportingUtils;

/**
 * The last wizard page.
 *
 */
public class FinishPage extends PagePanel {

    private static final long serialVersionUID = 1L;

    private JButton browseButton_;
    private JTextField fileNameField_;
    private boolean overwrite_ = false;
    JCheckBox sendCheck_;

    /**
     * Create the panel
     */
    public FinishPage(Preferences prefs) {
        super();
        title_ = "Save configuration and exit";
        setHelpFileName("conf_finish_page.html");
        prefs_ = prefs;
        setLayout(null);

        final JLabel configurationWillBeLabel = new JLabel();
        configurationWillBeLabel.setText("Configuration file:");
        configurationWillBeLabel.setBounds(14, 11, 123, 21);
        add(configurationWillBeLabel);

        fileNameField_ = new JTextField();
        fileNameField_.setBounds(12, 30, 429, 24);
        add(fileNameField_);

        browseButton_ = new JButton();
        browseButton_.addActionListener(new ActionListener() {

            public void actionPerformed(final ActionEvent e) {
                browseConfigurationFile();
            }
        });
        browseButton_.setText("Browse...");
        browseButton_.setBounds(450, 31, 100, 23);
        add(browseButton_);

        sendCheck_ = new JCheckBox();
        sendCheck_.setBounds(10, 100, 360, 33);
        sendCheck_.setFont(new Font("", Font.PLAIN, 12));
        sendCheck_.addActionListener(new ActionListener() {

            public void actionPerformed(ActionEvent arg0) {
              model_.setSendConfiguration(sendCheck_.isSelected());
            }
        });

        sendCheck_.setText("Send configuration to Micro-manager.org");
        add(sendCheck_);

        final JLabel sendConfigExplain = new JLabel();
        sendConfigExplain.setAutoscrolls(true);
        sendConfigExplain.setText("Providing the configuration data will assist securing further project funding.");
        sendConfigExplain.setBounds(14, 127, 500, 21);
        sendConfigExplain.setFont(sendCheck_.getFont());
        add(sendConfigExplain);
        
        //
    }

    public boolean enterPage(boolean next) {
        sendCheck_.setSelected(model_.getSendConfiguration());
        fileNameField_.setText(model_.getFileName());
        return true;
    }


    public boolean exitPage(boolean toNext) {
        if( toNext)
            saveConfiguration();
        
        return true;
    }

    public void refresh() {
    }

    public void loadSettings() {
        // TODO Auto-generated method stub
    }

    public void saveSettings() {
        // TODO Auto-generated method stub
    }

    private void browseConfigurationFile() {
        String suffixes[] = {".cfg"};
        File f = FileDialogs.save(this.parent_,
                "Select a configuration file name",
                MMStudio.MM_CONFIG_FILE);
        if (f != null) {
            setFilePath(f);
            overwrite_ = true;
        }
    }

    private void setFilePath(File f) {
        String absolutePath = f.getAbsolutePath();
        if (!absolutePath.endsWith(".cfg")) {
            absolutePath += ".cfg";
        }
        fileNameField_.setText(absolutePath);
    }

    private void saveConfiguration() {
         Container ancestor = getTopLevelAncestor();
         Cursor oldc = null;
         if (null != ancestor){
            oldc = ancestor.getCursor();
            Cursor waitc = new Cursor(Cursor.WAIT_CURSOR);
            ancestor.setCursor(waitc);
         }
        try {
            core_.unloadAllDevices();
            GUIUtils.preventDisplayAdapterChangeExceptions();

            File f = new File(fileNameField_.getText());
            if (f.exists() && !overwrite_) {
                int sel = JOptionPane.showConfirmDialog(this,
                        "Overwrite " + f.getName() + "?",
                        "File Save",
                        JOptionPane.YES_NO_OPTION);
                if (sel == JOptionPane.NO_OPTION) {
                    ReportingUtils.logMessage("All changes are going to be lost!");
                    return;
                }
            }
            setFilePath(f);
            model_.removeInvalidConfigurations();
            model_.saveToFile(fileNameField_.getText());
        } catch (MMConfigFileException e) {
            ReportingUtils.showError(e);
        } catch (Exception e) {
            ReportingUtils.showError(e);
        } finally{
            if (null != ancestor){
               if( null != oldc)
                  ancestor.setCursor(oldc);
            }
        }
    }
}
