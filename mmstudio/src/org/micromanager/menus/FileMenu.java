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

package org.micromanager.menus;


import com.google.common.collect.EvictingQueue;
import java.io.File;
import java.io.IOException;
import java.util.prefs.BackingStoreException;
import java.util.prefs.Preferences;
import javax.swing.JMenu;
import javax.swing.JMenuBar;

import org.micromanager.MMStudio;
import org.micromanager.utils.GUIUtils;
import org.micromanager.utils.MMScriptException;
import org.micromanager.utils.ReportingUtils;



/**
 * Handles setting up the File menu and its actions.
 */
public class FileMenu {
   private final MMStudio studio_;
   private final JMenu openRecentRamMenu_;
   private final EvictingQueue<File> recentFiles_;
   private final Preferences prefs_;
   private final int RECENTFILESSIZE = 8;
   private final String PREFKEYBASE = "file";

   public FileMenu(MMStudio studio) {
      studio_ = studio;
      openRecentRamMenu_ = new JMenu("Open Recent (RAM)...");
      recentFiles_ =  EvictingQueue.create(RECENTFILESSIZE);
      prefs_ = Preferences.userNodeForPackage(getClass());
      restoreFilesFromPref(prefs_, PREFKEYBASE, recentFiles_);
   }

   public void initializeFileMenu(JMenuBar menuBar) {
      JMenu fileMenu = GUIUtils.createMenuInMenuBar(menuBar, "File");

      GUIUtils.addMenuItem(fileMenu, "Open (Virtual)...", null,
         new Runnable() {
            @Override
            public void run() {
               new Thread() {
                  @Override
                  public void run() {
                     studio_.promptForAcquisitionToOpen(false);
                  }
               }.start();
            }
         }
      );

      GUIUtils.addMenuItem(fileMenu, "Open (RAM)...", null,
         new Runnable() {
            @Override
            public void run() {
               new Thread() {
                  @Override
                  public void run() {
                     File f = studio_.promptForAcquisitionToOpen(true);
                     addFileToRecentlyOpenedMenu(f);
                  }
               }.start();
            }
         }
      );
      
      makeRecentFileRamMenu(openRecentRamMenu_, recentFiles_);
      
      fileMenu.add(openRecentRamMenu_);

      fileMenu.addSeparator();

      GUIUtils.addMenuItem(fileMenu, "Exit", null,
         new Runnable() {
            @Override
            public void run() {
               studio_.closeSequence(false);
            }
         }
      );
   }
   
   public void addFileToRecentlyOpenedMenu(File f) {
      if (!recentFiles_.contains(f)) {
         recentFiles_.add(f);
      }
      writeFilesToPref(prefs_, PREFKEYBASE, recentFiles_);
      makeRecentFileRamMenu(openRecentRamMenu_, recentFiles_);
   }
   
   
   private void restoreFilesFromPref(Preferences prefs, String keyBase, 
           EvictingQueue<File> fileQueue) {
      final String notFound = "";
      boolean hasKey = true;
      int i = 0;
      while (hasKey) {
         String key = keyBase + i;
         String filePath = prefs.get(key, notFound);
         if (filePath.equals(notFound)) {
            hasKey = false;
         } else {
            fileQueue.add(new File (filePath));
            i++;
         }
      }
   }
   
   private static void writeFilesToPref(Preferences prefs, String keyBase, 
           EvictingQueue<File> fileQueue) {
      try {
         prefs.clear();
      } catch (BackingStoreException ex) {
         // TODO: Complain?
      }
      int i = 0;
      for (File f : fileQueue.toArray(new File[fileQueue.size()])) {
         String path = f.getParent();
         if (f.isDirectory()) {
            path = f.getAbsolutePath();
         }
         prefs.put(keyBase + i, path);
         i++;
      }
   }
   
   private void makeRecentFileRamMenu(JMenu recentFileMenu, 
           EvictingQueue<File> fileQueue) {
      recentFileMenu.removeAll();
      // travers the EvictingQueue in reverse order so that the most recently
      // opened file shows up first
      File[] fileQueueArray = fileQueue.toArray(new File[fileQueue.size()]);
      for (int i = fileQueueArray.length - 1; i >= 0; i--) {
         final File f = fileQueueArray[i];
         String p = f.getParent();
         if (f.isDirectory()) {
            p = f.getAbsolutePath();
         }
         final String path = p;
         f.getName();
         try {
            GUIUtils.addMenuItem(recentFileMenu, f.getCanonicalPath(), null,
                    new Runnable() {
                       @Override
                       public void run() {
                          new Thread() {
                             @Override
                             public void run() {
                                try {
                                   studio_.openAcquisitionData(path, true, true);
                                } catch (MMScriptException ex) {
                                   ReportingUtils.showError("Failed to open file: " +
                                           f.getName());
                                }
                             }
                          }.start();
                       }
                    }
            ); } catch (IOException ex) {
               ReportingUtils.logError(ex, "Failed to add file: " + f.getName() +
                       " to the recent file menu");
            }
      }
   }
   
   
   
}
