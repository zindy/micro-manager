/*
 * AUTHOR:
 * Nico Stuurman, nico@cmp.ucsf.edu, Fabruary 12, 2011
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

import ij.ImagePlus;
import ij.gui.Overlay;
import ij.gui.Roi;
import ij.gui.TextRoi;

import java.awt.Font;
import java.text.NumberFormat;

/**
 * This class provides facilities for creating a scale bar)
 * It could also be used from outside Micro-Manager (i.e, as an ImageJ facility)
 *
 * @author Nico Stuurman
 * 
 */
public class MMScaleBar {

   public enum Position {
      TOPLEFT, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT
   };

   private Position pos_ = Position.TOPLEFT;
   private final int OFFSET = 10;
   private ImagePlus ip_;
   private Font font_ = new Font("Helvetics", Font.PLAIN, 12);
   private double pixelSizeUm_;
   private final double MULTIPLIER = Math.sqrt(10.0);
   private int exponent_ = 0;
   private int barWidth_, barHeight_;
   private String value_;
   private String units_ = "\u00B5" + "m";

   public MMScaleBar(ImagePlus ip) {
      ip_ = ip;
      double pixelSize = ip.getCalibration().pixelWidth;
      units_ = ip.getCalibration().getXUnit();

      if (pixelSize > 0) {
         int width = ip_.getWidth();
         int height = ip_.getHeight();
         barHeight_ = (height / 75);
         int minBarWidth = width / 20;
         int maxBarWidth = width / 4;
         double pixsPerUnit = 1.0 / pixelSize;

         double pixs = pixsPerUnit;
         while (pixs < minBarWidth) {
            pixs *= MULTIPLIER;
            exponent_++;
         }
         while (pixs > maxBarWidth) {
            pixs /= MULTIPLIER;
            exponent_--;
         }
         double value = Math.pow(MULTIPLIER, exponent_);
/*
         if (exponent_ >= 6) {
            units_ = "mm";
            value = value / 1000000;
         }
         if (exponent_ <= -6) {
            units_ = "nm";
            value = value * 1000000;
         }
 */

         NumberFormat format = NumberFormat.getInstance();
         format.setMaximumFractionDigits(1);
         if (value > 10)
            format.setMaximumFractionDigits(0);

         value_ = format.format(value);

         barWidth_ = (int) pixs;
      }
   }

   public void setPosition(Position pos) {
      pos_ = pos;
   }

   public void setFont (Font font) {
      font_ = font;
   }
   
   public static void removeScaleBarFromOverlay(Overlay ol) {
      int textIndex = -1, barIndex = -1;
      for (int i = 0; i < ol.size(); i++) {
         if (ol.get(i) instanceof MMText ) {
            textIndex = i;
         } else if (ol.get(i) instanceof MMBar) {
            barIndex = i;
         }
      }
      if (textIndex != -1 && barIndex != -1) {
         //Bar was added 2nd so should be later in list
         ol.remove(barIndex);
         ol.remove(textIndex);
      }
   }

   public void addToOverlay(Overlay ol) {
      font_ = font_.deriveFont((int) (0.015 * ip_.getWidth()));
      TextRoi text = new MMText(10, 10, value_ + units_, font_);
      int textWidth = ip_.getProcessor().getStringWidth(value_ + units_);
      // TODO: text positioning depends on textHeight, but I don't know how to get this...
      if (pos_ == Position.TOPLEFT) {
         text.setLocation((int) (OFFSET + (0.5 * barWidth_ ) - (0.5 * textWidth)),
                 OFFSET - barHeight_ - 2);
      } else if (pos_ == Position.TOPRIGHT) {
         text.setLocation((int) (ip_.getWidth() - OFFSET - (0.5 * barWidth_) - (0.5 * textWidth)),
                 OFFSET - barHeight_ - 2);
      } else if (pos_ == Position.BOTTOMLEFT) {
         text.setLocation((int) (OFFSET + (0.5 * barWidth_ ) - (0.5 * textWidth)),
                 ip_.getHeight() - OFFSET - (3 * barHeight_) - 2);
      } else if (pos_ == Position.BOTTOMRIGHT) {
         text.setLocation( (int) (ip_.getWidth() - OFFSET - (0.5 * barWidth_) - (0.5 * textWidth)),
                 ip_.getHeight() - OFFSET - (3 * barHeight_) - 2);
      }

      ol.add(text);

      Roi bar = new MMBar(OFFSET, OFFSET + barHeight_, barWidth_, barHeight_);
      if (pos_ == Position.TOPRIGHT) {
         bar.setLocation(ip_.getWidth() - OFFSET - barWidth_, OFFSET + barHeight_);
      } else if (pos_ == Position.BOTTOMLEFT) {
         bar.setLocation(OFFSET, ip_.getHeight() - OFFSET - barHeight_);
      } else if (pos_ == Position.BOTTOMRIGHT) {
         bar.setLocation(ip_.getWidth() - OFFSET - barWidth_, ip_.getHeight() - OFFSET - barHeight_);
      }
      ol.add(bar);
   }
   
   //Make these their own class so they can be found and removed from overlay
   private class MMBar extends Roi {
      public MMBar(int a, int b, int c, int d) {
         super(a,b,c,d);
      }
   }
   
   private class MMText extends TextRoi {
      public MMText(int a, int b, String text, Font font) {
         super(a,b,text,font);
      }
   }

}

