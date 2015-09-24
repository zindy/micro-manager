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

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.LinkedList;
import java.util.Timer;
import java.util.TimerTask;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.SwingUtilities;


public class EDTProfiler {
   private static final int INTERVAL = 100;
   
   private Timer timer_;
   private final LinkedList<Long> startTimes_;
   private final LinkedList<Long> executionTimes_;
   
   
   public EDTProfiler() {
      timer_ = new Timer(  );
      startTimes_ = new LinkedList<Long>();
      executionTimes_ = new LinkedList<Long>();
      JFrame frame = new JFrame();
      JButton stop = new JButton("stop");
      stop.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent e) {
            end();
         }});
      JButton start = new JButton("start");
      start.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent e) {
            start();
         }});
      frame.setLayout(new GridLayout(2,1));
      frame.add(start);
      frame.add(stop);
      frame.pack();
      frame.setVisible(true);
   }
   
   private void start() {
      timer_.schedule(getTask(),0, INTERVAL);
   }
   
   private void end() {
      timer_.cancel();
      for (int i = 0; i < startTimes_.size(); i ++) {
         String print = startTimes_.get(i) +"\t";
         if ( i < executionTimes_.size() ) {
            print += executionTimes_.get(i) + "";
         }
         System.out.println(print);         
      }
   }
   
   private TimerTask getTask() {
      return new TimerTask() {
         @Override
         public void run() {
            startTimes_.add(System.currentTimeMillis());
            SwingUtilities.invokeLater(new Runnable() {
               @Override
               public void run() {
                  executionTimes_.add(System.currentTimeMillis());
               }});
         }     
      };
   }

}
