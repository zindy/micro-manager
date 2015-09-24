/*
 * AUTHOR:
 * Arthur Edelstein, arthuredelstein@gmail.com January 2011
 *
 * Copyright (c) 2011 Regents of the University of California
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

package org.micromanager.utils;

import java.awt.Dialog;
import java.awt.FileDialog;
import java.awt.Frame;
import java.awt.Window;
import java.io.File;
import java.util.prefs.Preferences;
import javax.swing.JFileChooser;

public class FileDialogs {

   public static class FileType {
      final String name;
      final String[] suffixes;
      final String description;
      final boolean suggestFileOnSave;
      final String defaultFileName;

      public FileType(String name, String description, String defaultFileName,
              boolean suggestFileOnSave, String... suffixes) {
         this.name = name;
         this.description = description;
         this.suffixes = suffixes;
         this.defaultFileName = defaultFileName;
         this.suggestFileOnSave = suggestFileOnSave;
      }
   }

   private static class GeneralFileFilter
           extends javax.swing.filechooser.FileFilter
           implements java.io.FilenameFilter
   {
      private final String fileDescription_;
      private final String[] fileSuffixes_;
      public GeneralFileFilter(String fileDescription, final String [] fileSuffixes) {
         fileDescription_ = fileDescription;
         fileSuffixes_ = fileSuffixes;
      }
      @Override
      public boolean accept(File pathname) {
         String name = pathname.getName();
         int n = name.lastIndexOf(".");
         String suffix = name.substring(1+n).toLowerCase();
         if (fileSuffixes_ == null || fileSuffixes_.length == 0) {
            return true;
         }
         if (!JavaUtils.isMac() && pathname.isDirectory()) {
            return true;
         }
         for (int i=0; i<fileSuffixes_.length; ++i) {
            if (fileSuffixes_[i] != null && fileSuffixes_[i].toLowerCase().contentEquals(suffix)) {
               return true;
            }
         }
         return false;
      }
      @Override
      public String getDescription() {
         return fileDescription_;
      }

      @Override
      public boolean accept(File dir, String name) {
         return accept(new File(dir, name));
      }
   }

   public static File show(Window parent,
                    String title,
                    File startFile,
                    boolean selectDirectories, boolean load,
                    final String fileDescription,
                    final String[] fileSuffixes,
                    boolean suggestFileName) {
      File selectedFile = null;
      GeneralFileFilter filter = new GeneralFileFilter(fileDescription, fileSuffixes);

      if (JavaUtils.isMac()) {
         if (selectDirectories) {
         // For Mac we only select directories, unfortunately!
            System.setProperty("apple.awt.fileDialogForDirectories", "true");
         }
         int mode = load? FileDialog.LOAD : FileDialog.SAVE;
         FileDialog fd;
         if (parent instanceof Dialog) {
            fd = new FileDialog((Dialog) parent, title, mode);
         } else if (parent instanceof Frame) {
            fd = new FileDialog((Frame) parent, title, mode);
         } else {
            fd = new FileDialog((Dialog) null, title, mode);
         }
         if (startFile != null) {
            if (startFile.isDirectory()) {
               fd.setDirectory(startFile.getAbsolutePath());
            }
            else {
               fd.setDirectory(startFile.getParent());
            }
            if (!load && suggestFileName) {
               fd.setFile(startFile.getName());
            }
         }
         if (fileSuffixes != null) {
            fd.setFilenameFilter(filter);
         }
         fd.setVisible(true);
         if (selectDirectories) {
            System.setProperty("apple.awt.fileDialogForDirectories", "false");
         }
         if (fd.getFile() != null) {
           selectedFile = new File(fd.getDirectory() + "/" + fd.getFile());
            if (mode == FileDialog.SAVE) {
               if (! filter.accept(selectedFile)) {
                  selectedFile = new File(selectedFile.getAbsolutePath()
                                          + "." + fileSuffixes[0]);
               }
            }
         }
         fd.dispose();
         
      } else {
         JFileChooser fc = new JFileChooser();
         if (startFile != null) {
            if ((!load && suggestFileName) || startFile.isDirectory()) {
               fc.setSelectedFile(startFile);
            } else {
               fc.setSelectedFile(startFile.getParentFile());
            }
         }
         fc.setDialogTitle(title);
         if (selectDirectories) {
            fc.setFileSelectionMode(JFileChooser.FILES_AND_DIRECTORIES);
         }
         if (fileSuffixes != null) {
            fc.setFileFilter(filter);
         }
         int returnVal;
         if (load) {
            returnVal = fc.showOpenDialog(parent);
         } else {
            returnVal = fc.showSaveDialog(parent);
         }
         if (returnVal == JFileChooser.APPROVE_OPTION) {
            selectedFile = fc.getSelectedFile();
         }
      }
      return selectedFile;
   }

   public static File show(Window parent, String title, FileType type,
                    boolean selectDirectories, boolean load) {
      Preferences node = Preferences.userNodeForPackage(FileDialogs.class);
      String startFile = node.get(type.name, type.defaultFileName);
      File startDir = null;
      if (startFile != null) {
         startDir = new File(startFile);
      }
      File result = show(parent, title, startDir, selectDirectories, load,
                         type.description, type.suffixes, type.suggestFileOnSave);
      if (result != null) {
         node.put(type.name, result.getAbsolutePath());
      }
      return result;
   }

   public static void storePath(FileType type, File path) {
      Preferences.userNodeForPackage(FileDialogs.class)
              .put(type.name, path.getAbsolutePath());
   }

   public static File openFile(Window parent, String title, FileType type) {
      return show(parent, title, type, false, true);
   }

   public static File openDir(Window parent, String title, FileType type) {
      return show(parent, title, type, true, true);
   }

   public static File save(Window parent, String title, FileType type) {
      return show(parent, title, type, false, false);
   }
}
