/*
 * AUTHOR:
 * Mark Tsuchida
 *
 * Copyright (c) 2014 Regents of the University of California
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

package org.micromanager.diagnostics;

import org.micromanager.utils.ReportingUtils;


public class ThreadExceptionLogger implements Thread.UncaughtExceptionHandler {
   private static boolean setUp_ = false;
   public static void setUp() {
      if (setUp_) {
         return;
      }

      Thread.setDefaultUncaughtExceptionHandler(
            new ThreadExceptionLogger(Thread.getDefaultUncaughtExceptionHandler()));

      // Be nice: don't disable an existing handler
      if (System.getProperty("sun.awt.exception.handler") == null) {
         System.setProperty("sun.awt.exception.handler",
               ThreadExceptionLogger.class.getName());
      }

      setUp_ = true;
   }

   private final Thread.UncaughtExceptionHandler chainedHandler_;

   public ThreadExceptionLogger(Thread.UncaughtExceptionHandler chained) {
      chainedHandler_ = chained;
   }

   // For instantiation by Swing via sun.awt.exception.handler
   public ThreadExceptionLogger() {
      chainedHandler_ = null;
   }

   @Override
   public void uncaughtException(Thread t, Throwable e) {
      ReportingUtils.logMessage("Thread " + t.getId() + " (" + t.getName() +
            ") terminated with uncaught exception");
      logException(e);
      if (chainedHandler_ != null) {
         chainedHandler_.uncaughtException(t, e);
      }
   }

   // This method is called reflectively via the sun.awt.exception.handler mechanism.
   public void handle(Throwable e) {
      ReportingUtils.logMessage("Uncaught exception in AWT/Swing event dispatch thread:");
      logException(e);
   }

   // TODO Factor out (make common with ReportingUtils.logError()).
   private void logException(Throwable e) {
      ReportingUtils.logMessage(e.toString());

      for (StackTraceElement frame : e.getStackTrace()) {
         ReportingUtils.logMessage("  at " + frame.toString());
      }

      Throwable cause = e.getCause();
      if (cause != null) {
         ReportingUtils.logMessage("Caused by exception:");
         logException(cause);
      }
   }
}
