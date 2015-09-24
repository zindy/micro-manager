/*
 * Copyright (c) 2008 Regents of the University of California
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

import java.awt.KeyEventDispatcher;
import java.awt.event.KeyEvent;

/**
 * Application-wide key dispatcher
 * It is necessary to use this approach since otherwise ImageJ will steal the
 * shortcuts before we get them
 * Downside is that all keyevents in the application will go through here
 * @author nico
 */
public class MMKeyDispatcher implements KeyEventDispatcher{
   Class textCanvasClass = null;
   final Class [] forbiddenClasses_;

   public MMKeyDispatcher() {
      try {
         textCanvasClass = ClassLoader.getSystemClassLoader().loadClass("ij.text.TextCanvas");
      } catch (ClassNotFoundException ex) {
         textCanvasClass = null;
         ReportingUtils.logError(ex);
      }

      /*
       * If there are other areas in the application in which keyevents should
       * not be processed, add those here
       */
      Class [] forbiddenClasses = {
         java.awt.TextComponent.class,
         javax.swing.text.JTextComponent.class,
         org.fife.ui.rsyntaxtextarea.RSyntaxTextArea.class,
         textCanvasClass
      };
      forbiddenClasses_ = forbiddenClasses;
   }

   /*
    * Exclude key events coming from specific sources (like text components)
    * Only way I could come up with was introspection
    */
   private boolean checkSource(KeyEvent ke) {
      Object source = ke.getSource();
      for (Class clazz:forbiddenClasses_) {
         if (clazz != null && clazz.isInstance(source))
            return false;
      }
      return true;
   }

   /*
    * 
    */
   @Override
   public boolean dispatchKeyEvent(KeyEvent ke) {
      if (!HotKeys.active_)
         return false;
      if (ke.getID() != KeyEvent.KEY_PRESSED)
         return false;

      // Since all key events in the application go through here
      // we need to efficiently determine whether or not to deal with this
      // key event will be dealt with.  CheckSource seems relatively expensive
      // so only call this when the key matches

      if (HotKeys.keys_.containsKey(ke.getKeyCode())) {
         if (checkSource(ke))
            return HotKeys.keys_.get(ke.getKeyCode()).ExecuteAction();
      }
      return false;
   }
}
