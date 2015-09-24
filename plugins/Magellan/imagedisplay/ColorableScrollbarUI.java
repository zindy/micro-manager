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

import com.sun.java.swing.plaf.windows.WindowsScrollBarUI;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Rectangle;
import javax.swing.JComponent;
import javax.swing.JScrollBar;

/**
 *
 * @author Henry
 */
public class ColorableScrollbarUI extends WindowsScrollBarUI {

   private static final Color DARK_GREEN = new Color(0,70,0);
   
   private int displayedSliceIndex_, minSliceIndex_, maxSliceIndex_;
   
   public void setHighlightedIndices(int currentIndex, int min, int max) {
      displayedSliceIndex_ = currentIndex;
      minSliceIndex_ = min;
      maxSliceIndex_ = max;
   }
   
   @Override
   protected void paintThumb(Graphics g, JComponent c, Rectangle thumbBounds) {
      super.paintThumb(g, c, thumbBounds);
      if (((JScrollBar) c).getValue() == displayedSliceIndex_) { 
         g.setColor(DARK_GREEN);
         g.drawRect(thumbBounds.x, thumbBounds.y, thumbBounds.width, thumbBounds.height);
      }
   }

   @Override
   protected void paintTrack(Graphics g, JComponent c, Rectangle trackBounds) {
      super.paintTrack(g, c, trackBounds);
      int numPositions = ((JScrollBar) c).getMaximum() - ((JScrollBar) c).getMinimum();
      //show range of z scrollbar inlight green
      g.setColor(new Color(180,220,180));
        
      int rangeStart = (int) ((minSliceIndex_- ((JScrollBar) c).getMinimum()) / (double) numPositions * trackBounds.width) + trackBounds.x;
      int rangeWidth = (int) ((maxSliceIndex_ - minSliceIndex_ + 1) / (double) numPositions * trackBounds.width);
      g.fillRect(rangeStart, trackBounds.y, rangeWidth, trackBounds.height);

      
      //show the position in dark green
      g.setColor(DARK_GREEN);
      int start = (int) ((displayedSliceIndex_- ((JScrollBar) c).getMinimum()) / (double) numPositions * trackBounds.width) + trackBounds.x;
      int width = (int) (1 / (double) numPositions * trackBounds.width);
      g.fillRect(start, trackBounds.y, width, trackBounds.height);

   }
}
