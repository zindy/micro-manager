/*
 * AUTHOR:
 * Henry Pinkard, henry.pinkard@gmail.com
 *
 * Copyright (c) 2015 Regents of the University of California
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

package imagedisplay;

import java.awt.BasicStroke;
import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.geom.GeneralPath;

/**
 * This class displays a little play/pause icon with a single-character label,
 * and is used for handling animation of an AxisScroller.
 */
public class ScrollbarAnimateIcon extends Canvas {
   private static final int WIDTH = 24, HEIGHT = 14;
   private BasicStroke stroke = new BasicStroke(2f);
   private String label_;
   private boolean isAnimated_;

   public ScrollbarAnimateIcon(String axis) {
      setSize(WIDTH, HEIGHT);
      // Only use the first letter of the axis.
      label_ = axis.substring(0, 1);
      isAnimated_ = false;
   }

   public void setIsAnimated(boolean isAnimated) {
      isAnimated_ = isAnimated;
      repaint();
   }

   /** Overrides Component getPreferredSize(). */
   @Override
   public Dimension getPreferredSize() {
      return new Dimension(WIDTH, HEIGHT);
   }
   
   @Override
   public void paint(Graphics g) {
      g.setColor(Color.white);
      g.fillRect(0, 0, WIDTH, HEIGHT);
      Graphics2D g2d = (Graphics2D)g;
      g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
      drawPlayPauseButton(g2d);
      drawLetter(g);
   }
   
   private void drawCenteredLetter(Graphics g) {
      g.setFont(new Font("SansSerif", Font.PLAIN, 14));
      g.setColor(Color.black);
      g.drawString(label_, 8, 12);
   }
   
   private void drawLetter(Graphics g) {
      g.setFont(new Font("SansSerif", Font.PLAIN, 14));
      g.setColor(Color.black);
      g.drawString(label_, 4, 12);
   }

   private void drawPlayPauseButton(Graphics2D g) {
      if (isAnimated_) {
         // Draw a pause button
         g.setColor(Color.red);
         g.setStroke(stroke);
         g.drawLine(15, 3, 15, 11);
         g.drawLine(20, 3, 20, 11);
      } else {
         // Draw a play button
         g.setColor(new Color(0,150,0));
         GeneralPath path = new GeneralPath();
         path.moveTo(15f, 2f);
         path.lineTo(22f, 7f);
         path.lineTo(15f, 12f);
         path.lineTo(15f, 2f);
         g.fill(path);
      }
   }
   }


