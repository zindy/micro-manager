/*
 * AUTHOR:
 * Arthur Edelstein and Henry Pinkard, 2011
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

import java.util.concurrent.atomic.AtomicReference;
import javax.swing.SwingUtilities;

/**
 *
 * @author arthur and henry
 * DO NOT USE.  This code is only here so that the DataBrowser plugin can still 
 * compile.  Remove after this dependency in the DataBrowser has been removed.
 * 
 * @Deprecated
 */
public class GUIUpdater {

   final AtomicReference<Runnable> latestTask = new AtomicReference<Runnable>();

   /*
    * Post a task for running on the EDT thread. If multiple
    * tasks pile up, only the most recent will run.
    */
   public void post(Runnable task) {
      if (latestTask.getAndSet(task) == null) {
         SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
               latestTask.getAndSet(null).run();
            }
         });
      }
   }
}
