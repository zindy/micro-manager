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

package org.micromanager.utils;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.prefs.Preferences;

/**
 *
 * @author nico
 */
public class HotKeys {
   Preferences root_;
   private static Preferences prefs_;
   private static final int STOP = -1;
   private static final String KEY = "Key";
   private static final String TYPE = "Type";
   private static final String GUICOMMAND = "GuiCommand";
   private static final String FILENAME = "FileName";

   // Note that this data structure is not synchroinized.  Since we are not
   // currently reading and writing at the same time, and access it only from
   // a single thread (I think), this should be safe.
   // Howvere, if this changes in the future, please synchronize this structure
   public static final LinkedHashMap<Integer, HotKeyAction> keys_ =
           new LinkedHashMap<Integer, HotKeyAction>();

   public  static boolean active_ = true;

   public HotKeys () {
      root_ = Preferences.userNodeForPackage(this.getClass());
      prefs_ = root_.node(root_.absolutePath() + "/HotKeys");
   }

   public void loadSettings() {
      // restore previously listed hotkeys from prefs
      if (prefs_ == null)
         return;

      int j = 0;
      int key;
      int type;
      int guiCommand;
      File file;
      do {
         key = prefs_.getInt(KEY + j, STOP);
         if (key != STOP) {
            type = prefs_.getInt(TYPE + j, HotKeyAction.GUICOMMAND);
            if (type == HotKeyAction.GUICOMMAND) {
               guiCommand = prefs_.getInt(GUICOMMAND + j, HotKeyAction.SNAP);
               HotKeyAction action = new HotKeyAction(guiCommand);
               keys_.put(key, action);
            }  else {
               file = new File(prefs_.get(FILENAME + j, ""));
               HotKeyAction action = new HotKeyAction(file);
               keys_.put(key, action);
            }
         }
         j++;
      }
      while (key != STOP);
   }
   
   public void saveSettings() {
      if (prefs_ == null)
         return;

      Iterator it = keys_.entrySet().iterator();
      int i = 0;
      while (it.hasNext()) {
         Map.Entry pairs = (Map.Entry)it.next();
         prefs_.putInt(KEY + i, ((Integer) pairs.getKey()).intValue());
         HotKeyAction action = (HotKeyAction) pairs.getValue();
         prefs_.putInt(TYPE + i, action.type_);
         if (action.type_ == HotKeyAction.GUICOMMAND)
            prefs_.putInt(GUICOMMAND + i, action.guiCommand_);
         else
            prefs_.put(FILENAME + i, action.beanShellScript_.getAbsolutePath());
         i++;
      }

      // Add key as signal for the reader to stop reading
      prefs_.putInt(KEY + i, STOP);

   }

   public static void load(File f) throws FileNotFoundException {
      if (f== null || !f.canRead())
         return;

      DataInputStream in = new DataInputStream
              (new BufferedInputStream(new FileInputStream(f)));
      keys_.clear();
      try {
         while (in.available() > 0) {
            int key = in.readInt();
            int type = in.readInt();
            String filePath = "";
            if (type == HotKeyAction.GUICOMMAND) {
               int guiCommand = in.readInt();
               HotKeyAction action = new HotKeyAction(guiCommand);
               keys_.put(key, action);
            }
            else {
               int strLength = in.readInt();
               for (int i=0; i < strLength; i ++)
                  filePath += in.readChar();
               HotKeyAction action = new HotKeyAction(new File(filePath));
               keys_.put(key, action);
            }
         }
         in.close();
      } catch (IOException ex) {
         ReportingUtils.showError("Error while reading in Shortcuts");
      }
   }
   /*
    * Save Hotkeys to a file
    * File needs to exist and be writeable
    */
   public static void save(File f) throws FileNotFoundException {
      if (f== null || !f.canWrite())
         return;

      DataOutputStream out = new DataOutputStream
              (new BufferedOutputStream(new FileOutputStream(f)));

      Iterator it = keys_.entrySet().iterator();
      int i = 0;
      while (it.hasNext()) {
         try {
            Map.Entry pairs = (Map.Entry) it.next();
            out.writeInt(((Integer) pairs.getKey()).intValue());
            HotKeyAction action = (HotKeyAction) pairs.getValue();
            out.writeInt(action.type_);
            if (action.type_ == HotKeyAction.GUICOMMAND) {
               out.writeInt(action.guiCommand_);
            } else {
               out.writeInt(action.beanShellScript_.getAbsolutePath().length());
               out.writeChars(action.beanShellScript_.getAbsolutePath());
            }
            i++;
         } catch (IOException ex) {
            ReportingUtils.showError("Error while saving Shortcuts");
         }
      }
      try {
         out.close();
      } catch (IOException ex) {
         ReportingUtils.showError("Error while closing Shortcuts file");
      }

   }
}
