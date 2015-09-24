/*
 * AUTHOR:
 * Nico Stuurman, nico@cmp.ucsf.edu, December, 2006
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

package misc;

import java.awt.BorderLayout;

import javax.swing.BorderFactory;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JProgressBar;


public class ProgressBar extends JPanel {
   private static final long serialVersionUID = 1L;
   private JProgressBar progressBar;
   private JFrame frame;

   public ProgressBar (String windowName, int start, int end) {
      super(new BorderLayout());
      
      frame = new JFrame(windowName);
      frame.setDefaultCloseOperation (JFrame.DISPOSE_ON_CLOSE);
      frame.setBounds(0,0,150 + 6 * windowName.length() ,100);

      progressBar = new JProgressBar(start,end);
      progressBar.setValue(0);
      JPanel panel = new JPanel(new BorderLayout());
      panel.add(progressBar, BorderLayout.CENTER);
      add(panel, BorderLayout.CENTER);
      panel.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20));

      JComponent newContentPane = panel;
      newContentPane.setOpaque(true);
      frame.setContentPane(newContentPane);

      frame.setLocationRelativeTo(null);
      frame.setVisible(true);
   }

   public void setProgress(int progress) {
      progressBar.setValue(progress);
      progressBar.repaint();
   }

   @Override
   public void setVisible(boolean visible) {
      frame.setVisible(visible);
   }

    public void setRange(int min, int max) {
        progressBar.setMinimum(min);
        progressBar.setMaximum(max);
    }

   /*
   public static void main(String[] args) {
      ProgressBar testBar = new ProgressBar ("Opening File...", 0, 100);
      for (int i=0; i<=100; i++) {
         try {
            Thread.sleep(10);
         } catch (InterruptedException ignore) { ReportingUtils.logError(ignore); }
         testBar.setProgress(i);
      }
      testBar.setVisible(false);
   }
   */
}
