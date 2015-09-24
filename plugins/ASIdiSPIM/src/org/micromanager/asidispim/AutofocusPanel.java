/*
 * AUTHOR:
 * Nico Stuurman, Jon Daniels
 *
 * Copyright (c) 2015 Regents of the University of California, & ASI
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

package org.micromanager.asidispim;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSpinner;

import mmcorej.StrVector;
import net.miginfocom.swing.MigLayout;

import org.micromanager.api.ScriptInterface;
import org.micromanager.asidispim.Data.Devices;
import org.micromanager.asidispim.Data.MyStrings;
import org.micromanager.asidispim.Data.Prefs;
import org.micromanager.asidispim.Data.Properties;
import org.micromanager.asidispim.Utils.AutofocusUtils;
import org.micromanager.asidispim.Utils.ListeningJPanel;
import org.micromanager.asidispim.Utils.PanelUtils;
import org.micromanager.asidispim.fit.Fitter;

/**
 *
 * @author nico
 */
@SuppressWarnings("serial")
public class AutofocusPanel extends ListeningJPanel{
   final private Properties props_;
   final private Prefs prefs_;
   final private Devices devices_;
   
   private final JPanel optionsPanel_;
   private final JPanel acqOptionsPanel_;
   
   public AutofocusPanel(final ScriptInterface gui, final Devices devices, 
           final Properties props, final Prefs prefs, 
           final AutofocusUtils autofocus) {
      
      super(MyStrings.PanelNames.AUTOFOCUS.toString(),
              new MigLayout(
              "",
              "[center]8[center]",
              "[]16[]16[]"));
      prefs_ = prefs;
      props_ = props;
      devices_ = devices;
      
      PanelUtils pu = new PanelUtils(prefs_, props_, devices_);

      // start options panel
      optionsPanel_ = new JPanel(new MigLayout(
            "",
            "[right]16[center]",
            "[]8[]"));
      optionsPanel_.setBorder(PanelUtils.makeTitledBorder("Autofocus Options"));
      
      // show images checkbox
      final JCheckBox showImagesCheckBox = pu.makeCheckBox("Show images",
              Properties.Keys.PLUGIN_AUTOFOCUS_SHOWIMAGES, panelName_, false);
      optionsPanel_.add(showImagesCheckBox);
      
      // show plot checkbox
      final JCheckBox showPlotCheckBox = pu.makeCheckBox("Show plot",
              Properties.Keys.PLUGIN_AUTOFOCUS_SHOWPLOT, panelName_, false);
      optionsPanel_.add(showPlotCheckBox, "wrap");
 
      // spinner with number of images:
      optionsPanel_.add(new JLabel("Number of images:"));
      final JSpinner nrImagesSpinner = pu.makeSpinnerInteger(1, 1000,
            Devices.Keys.PLUGIN,
            Properties.Keys.PLUGIN_AUTOFOCUS_NRIMAGES, 10);
      optionsPanel_.add(nrImagesSpinner, "wrap");
      
      // spinner with stepsize:
      optionsPanel_.add(new JLabel("Step size [\u00B5m]:"));
      final JSpinner stepSizeSpinner = pu.makeSpinnerFloat(0.001, 100., 1.,
            Devices.Keys.PLUGIN,
            Properties.Keys.PLUGIN_AUTOFOCUS_STEPSIZE, 10);
      optionsPanel_.add(stepSizeSpinner, "wrap");
      
      // scan either piezo or sheet; select which one
      optionsPanel_.add(new JLabel("Mode:"));
      String[] scanOptions = {"Fix piezo, sweep slice", "Fix slice, sweep piezo"};
      final JComboBox scanModeCB = pu.makeDropDownBox(scanOptions,
            Devices.Keys.PLUGIN, Properties.Keys.AUTOFOCUS_ACQUSITION_MODE,
            scanOptions[0]);
      optionsPanel_.add(scanModeCB, "wrap");
      
      optionsPanel_.add(new JLabel("Scoring algorithm:"));
      final JComboBox scoringAlgorithmCB = new JComboBox();
      for (String scoringAlgorithm : Fitter.getAlgorithms()) {
         scoringAlgorithmCB.addItem(scoringAlgorithm);
      }
      scoringAlgorithmCB.setSelectedItem(Fitter.getAlgorithmFromPrefCode(
            prefs_.getInt(panelName_,
            Properties.Keys.AUTOFOCUS_SCORING_ALGORITHM,
            Fitter.Algorithm.VOLATH.getPrefCode())).toString());
      scoringAlgorithmCB.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent e) {
            prefs_.putInt(panelName_, Properties.Keys.AUTOFOCUS_SCORING_ALGORITHM,
                  Fitter.getPrefCodeFromString(scoringAlgorithmCB.getSelectedItem().toString()));
         }
      });
      optionsPanel_.add(scoringAlgorithmCB, "wrap");
      
      optionsPanel_.add(new JLabel("Fit using:"));
      final JComboBox fitFunctionSelection = new JComboBox();
      for (String fitFunction : Fitter.getFunctionTypes()) {
         fitFunctionSelection.addItem(fitFunction);
      }
      fitFunctionSelection.setSelectedItem(prefs_.getString(panelName_, 
              Prefs.Keys.AUTOFOCUSFITFUNCTION, 
              Fitter.getFunctionTypeAsString(Fitter.FunctionType.Gaussian)));
      fitFunctionSelection.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent e) {
            prefs_.putString(panelName_, Prefs.Keys.AUTOFOCUSFITFUNCTION, 
                    (String) fitFunctionSelection.getSelectedItem());
         }
      });
      optionsPanel_.add(fitFunctionSelection, "wrap");
            
      optionsPanel_.add(new JLabel("<html>Minimum R<sup>2</sup></html>:"));
      final JSpinner minimumR2Spinner = pu.makeSpinnerFloat(0.0, 1.0, 0.01,
              Devices.Keys.PLUGIN, Properties.Keys.PLUGIN_AUTOFOCUS_MINIMUMR2, 0.75);
      optionsPanel_.add(minimumR2Spinner);
      
      // end options subpanel
      
      // start acquisition options panel
      acqOptionsPanel_ = new JPanel(new MigLayout(
            "",
            "[right]16[center]",
            "[]8[]"));
      acqOptionsPanel_.setBorder(PanelUtils.makeTitledBorder(
              "Autofocus Options during Acquisition"));
      
      // whether or not to run autofocus at the start of the acquisition
      final JCheckBox beforeStartCheckBox = pu.makeCheckBox("Autofocus before starting acquisition",
              Properties.Keys.PLUGIN_AUTOFOCUS_ACQBEFORESTART, panelName_, false);     
      acqOptionsPanel_.add(beforeStartCheckBox, "center, span 3, wrap");
      
      // autofocus every nth image
      acqOptionsPanel_.add(new JLabel("Autofocus every "));
      final JSpinner eachNTimePointsSpinner = pu.makeSpinnerInteger(1, 1000,
            Devices.Keys.PLUGIN,
            Properties.Keys.PLUGIN_AUTOFOCUS_EACHNIMAGES, 10);
      acqOptionsPanel_.add(eachNTimePointsSpinner);
      acqOptionsPanel_.add(new JLabel( "time points"), "wrap");
      
      // autofocus using this channel
      // TODO: need to update when the channel group changes
      String channelGroup_  = props_.getPropValueString(Devices.Keys.PLUGIN,
            Properties.Keys.PLUGIN_MULTICHANNEL_GROUP);
      StrVector channels = gui.getMMCore().getAvailableConfigs(channelGroup_);
      final JComboBox channelSelect = pu.makeDropDownBox(channels.toArray(), 
              Devices.Keys.PLUGIN, Properties.Keys.PLUGIN_AUTOFOCUS_CHANNEL, "");
      // make sure to explicitly set it to something so pref gets written
      channelSelect.setSelectedIndex(channelSelect.getSelectedIndex());
      acqOptionsPanel_.add(new JLabel("Autofocus Channel: "));
      acqOptionsPanel_.add(channelSelect, "wrap");

      
      // construct the main panel
      add(optionsPanel_);
      add (acqOptionsPanel_);
       
      
      
   }
   
}
