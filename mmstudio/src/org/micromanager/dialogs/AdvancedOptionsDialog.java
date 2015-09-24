/*
 * Copyright (c) 2005-2015 Regents of the University of California
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

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;



import org.micromanager.acquisition.AcquisitionEngine;
import org.micromanager.api.ScriptInterface;
import org.micromanager.utils.MMDialog;

/*
 * This class provides a dialog available from the "Advanced" button
 * on the AcqConrolDlg (MDA setup window). It allows users to set custom
 * time intervals: i.e., nonuniform timing for a series of frames.
 */

public class AdvancedOptionsDialog extends MMDialog {

    private static final long serialVersionUID = 1L;
    private final AcquisitionEngine acqEng_;
    private final JTabbedPane tabbedPane_;
    private final JPanel closeButtonPanel_;
    private final CustomTimeIntervalsPanel customTimeIntervalsPanel_;

    public AdvancedOptionsDialog(AcquisitionEngine acqEng, ScriptInterface gui) {
        super();
        this.setModal(true);
        acqEng_ = acqEng;
        loadPosition(0,0,600,500);
        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent arg0) {
                savePosition();
            }});
        setTitle("Advanced acquisition options");

        this.setMinimumSize(new Dimension(600,400));

        tabbedPane_ = new JTabbedPane();
        customTimeIntervalsPanel_ = new CustomTimeIntervalsPanel(acqEng_,tabbedPane_, gui);

        tabbedPane_.add("Custom time intervals", customTimeIntervalsPanel_);
//        tabbedPane_.add("More coming soon", new JLabel());
        this.getContentPane().add(tabbedPane_);

        closeButtonPanel_ = new JPanel();
        JButton closeButton = new JButton("Close");
        closeButton.addActionListener(new ActionListener() {

            @Override
            public void actionPerformed(ActionEvent e) {
                close();
            }
        });
        closeButtonPanel_.add(closeButton);
        this.getContentPane().add(closeButtonPanel_, BorderLayout.PAGE_END);
        
    }

    @Override
    public void setVisible(boolean b) {
        updatePanels();
        super.setVisible(b);
    }
    
    //make sure that all panels display data currently in acqEng upon becoming visible
    private void updatePanels() {
        customTimeIntervalsPanel_.syncCheckBoxFromAcqEng();
        customTimeIntervalsPanel_.syncIntervalsFromAcqEng();
    }
    
    private void close() {
        this.setVisible(false);
    }
}
