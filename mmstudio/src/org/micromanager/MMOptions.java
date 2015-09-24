/*
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, Sept 12, 2006
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

package org.micromanager;

import java.util.prefs.BackingStoreException;
import java.util.prefs.Preferences;

/**
 * Options data for MMStudio.
 */
public class MMOptions {
   private static final String DEBUG_LOG = "DebugLog";
   private static final String PREF_DIR = "MMOptions";
   private static final String CLOSE_ON_EXIT = "CloseOnExit";
   private static final String SKIP_CONFIG = "SkipSplashScreen";
   private static final String BUFFSIZE_MB = "bufsize_mb";
   private static final String DISPLAY_BACKGROUND = "displayBackground";
   private static final String STARTUP_SCRIPT_FILE = "startupScript";
   private static final String AUTORELOAD_DEVICES = "autoreloadDevices"; // No longer used but should not be reused
   private static final String PREF_WINDOW_MAG = "windowMag";
   private static final String MPTIFF_METADATA_FILE = "MakeMetadataFileWithMultipageTiff";
   private static final String MPTIFF_SEPARATE_FILES_FOR_POSITIONS = "SplitXYPostionsInFilesMPTiff";
   private static final String SYNCEXPOSUREMAINANDMDA = "SyncExposureBetweenMainAndMDAWindows";
   private static final String HIDE_MDA_DISPLAY = "HideMDADisplay";
   private static final String FAST_STORAGE = "FastStorage"; // No longer used but should not be reused
   private static final String DELETE_OLD_CORELOGS = "DeleteOldCoreLogs";
   private static final String DELETE_CORELOG_AFTER_DAYS =
      "DeleteCoreLogAfterDays";

   public boolean debugLogEnabled_;
   public boolean doNotAskForConfigFile_;
   public boolean closeOnExit_;
   public int circularBufferSizeMB_;
   public String displayBackground_;
   public String startupScript_;
   public double windowMag_;
   public boolean mpTiffMetadataFile_;
   public boolean mpTiffSeparateFilesForPositions_;
   public boolean syncExposureMainAndMDA_;
   public boolean hideMDADisplay_;
   public boolean deleteOldCoreLogs_;
   public int deleteCoreLogAfterDays_;

   public MMOptions() {
      setDefaultValues();
   }

   private void setDefaultValues() {
      debugLogEnabled_ = false;
      doNotAskForConfigFile_ = false;
      closeOnExit_ = true;
      boolean is64BitJVM =
         System.getProperty("sun.arch.data.model", "32").equals("64");
      circularBufferSizeMB_ = is64BitJVM ? 250 : 25;
      displayBackground_ = "Day";
      startupScript_ = "MMStartup.bsh";
      windowMag_ = 1.0;
      mpTiffMetadataFile_ = false;
      mpTiffSeparateFilesForPositions_ = true;
      syncExposureMainAndMDA_ = false;
      hideMDADisplay_ = false;
      deleteOldCoreLogs_ = false;
      deleteCoreLogAfterDays_ = 7;
   }

   private Preferences getPrefNode() {
      Preferences root = Preferences.userNodeForPackage(this.getClass());
      Preferences prefs = root.node(root.absolutePath() + "/" + PREF_DIR);
      return prefs;
   }

   public void saveSettings() {
      Preferences prefs = getPrefNode();

      prefs.putBoolean(DEBUG_LOG, debugLogEnabled_);
      prefs.putBoolean(SKIP_CONFIG, doNotAskForConfigFile_);
      prefs.putBoolean(CLOSE_ON_EXIT, closeOnExit_);
      prefs.putInt(BUFFSIZE_MB, circularBufferSizeMB_);
      prefs.put(DISPLAY_BACKGROUND, displayBackground_);
      prefs.put(STARTUP_SCRIPT_FILE, startupScript_);
      prefs.putDouble(PREF_WINDOW_MAG, windowMag_);
      prefs.putBoolean(MPTIFF_METADATA_FILE, mpTiffMetadataFile_);
      prefs.putBoolean(MPTIFF_SEPARATE_FILES_FOR_POSITIONS, mpTiffSeparateFilesForPositions_);
      prefs.putBoolean(SYNCEXPOSUREMAINANDMDA, syncExposureMainAndMDA_);
      prefs.putBoolean(HIDE_MDA_DISPLAY, hideMDADisplay_);
      prefs.putBoolean(DELETE_OLD_CORELOGS, deleteOldCoreLogs_);
      prefs.putInt(DELETE_CORELOG_AFTER_DAYS, deleteCoreLogAfterDays_);
   }

   public void loadSettings() {
      Preferences prefs = getPrefNode();

      debugLogEnabled_ = prefs.getBoolean(DEBUG_LOG, debugLogEnabled_);
      doNotAskForConfigFile_ = prefs.getBoolean(SKIP_CONFIG, doNotAskForConfigFile_);
      closeOnExit_ = prefs.getBoolean(CLOSE_ON_EXIT, closeOnExit_);
      circularBufferSizeMB_ = prefs.getInt(BUFFSIZE_MB, circularBufferSizeMB_);
      displayBackground_ = prefs.get(DISPLAY_BACKGROUND, displayBackground_);
      startupScript_ = prefs.get(STARTUP_SCRIPT_FILE, startupScript_);
      windowMag_ = prefs.getDouble(PREF_WINDOW_MAG, windowMag_);
      mpTiffMetadataFile_ = prefs.getBoolean(MPTIFF_METADATA_FILE, mpTiffMetadataFile_);
      mpTiffSeparateFilesForPositions_ = prefs.getBoolean(MPTIFF_SEPARATE_FILES_FOR_POSITIONS, mpTiffSeparateFilesForPositions_);
      syncExposureMainAndMDA_ = prefs.getBoolean(SYNCEXPOSUREMAINANDMDA, syncExposureMainAndMDA_);
      hideMDADisplay_ = prefs.getBoolean(HIDE_MDA_DISPLAY, hideMDADisplay_);
      deleteOldCoreLogs_ =
         prefs.getBoolean(DELETE_OLD_CORELOGS, deleteOldCoreLogs_);
      deleteCoreLogAfterDays_ =
         prefs.getInt(DELETE_CORELOG_AFTER_DAYS, deleteCoreLogAfterDays_);
   }

   public void resetSettings() throws BackingStoreException {
      getPrefNode().clear();
      setDefaultValues();
   }
}
