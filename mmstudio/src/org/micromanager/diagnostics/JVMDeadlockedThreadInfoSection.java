/*
 * AUTHOR:
 * Mark Tsuchida
 *
 * Copyright (c) 2013-2014 Regents of the University of California
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

import java.lang.management.ThreadInfo;

class JVMDeadlockedThreadInfoSection implements SystemInfo.SystemInfoSection {
   public String getTitle() { return "Deadlocked Java threads"; }

   public String getReport() {
      StringBuilder sb = new StringBuilder();

      java.lang.management.ThreadMXBean threadMXB = java.lang.management.ManagementFactory.getThreadMXBean();
      long[] deadlockedThreadIds = threadMXB.findDeadlockedThreads();

      if (deadlockedThreadIds != null && deadlockedThreadIds.length > 0) {
         sb.append("Deadlocked Java threads: ").
            append(Integer.toString(deadlockedThreadIds.length)).append(" detected\n");

         java.util.Arrays.sort(deadlockedThreadIds);
         ThreadInfo[] deadlockedInfos = threadMXB.getThreadInfo(deadlockedThreadIds, true, true);
         for (ThreadInfo tInfo : deadlockedInfos) {
            sb.append("Deadlocked Java thread: id ").append(Long.toString(tInfo.getThreadId())).
               append(" (\"").append(tInfo.getThreadName()).append("\"):").append('\n');

            java.lang.management.LockInfo blockingLock = tInfo.getLockInfo();
            sb.append("  Blocked waiting to lock ").append(blockingLock.getClassName()).append(' ').
               append(Integer.toString(blockingLock.getIdentityHashCode())).append('\n');

            java.lang.management.MonitorInfo[] monitors = tInfo.getLockedMonitors();
            java.lang.management.LockInfo[] synchronizers = tInfo.getLockedSynchronizers();
            StackTraceElement[] trace = tInfo.getStackTrace();
            for (StackTraceElement frame : trace) {
               sb.append("    at ").append(frame.toString()).append('\n');

               for (java.lang.management.MonitorInfo monitor : monitors) {
                  if (monitor.getLockedStackFrame().equals(frame)) {
                     sb.append("      where monitor was locked: ").
                        append(monitor.getClassName()).append(' ').
                        append(Integer.toString(monitor.getIdentityHashCode())).append('\n');
                  }
               }
            }
            for (java.lang.management.LockInfo sync : synchronizers) {
               sb.append("  Ownable synchronizer is locked: ").
                  append(sync.getClassName()).append(' ').
                  append(Integer.toString(sync.getIdentityHashCode())).append('\n');
            }
         }
      }
      else {
         sb.append("Deadlocked Java threads: none detected");
      }

      return sb.toString();
   }

   // TODO Put this in a proper test suite.
   private void createDeadlockedThreadPair() {
      final Object a = new Object();
      final Object b = new Object();
      Thread th0 = new Thread("DeadLockTestThread0") {
         public void run() {
            try {
               synchronized (a) {
                  Thread.sleep(100);
                  synchronized (b) {
                     Thread.sleep(1);
                  }
               }
            } catch (InterruptedException e) {
            }
         }
      };
      Thread th1 = new Thread("DeadLockTestThread1") {
         public void run() {
            try {
               synchronized (b) {
                  Thread.sleep(100);
                  synchronized (a) {
                     Thread.sleep(1);
                  }
               }
            } catch (InterruptedException e) {
            }
         }
      };
      th0.start();
      th1.start();

      try {
         Thread.sleep(200);
      } catch (InterruptedException exc) {
      }
   }
}
