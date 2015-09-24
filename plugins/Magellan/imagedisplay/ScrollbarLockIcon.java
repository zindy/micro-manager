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

import com.google.common.eventbus.EventBus;
import java.awt.*;
import java.awt.event.MouseEvent;
import javax.swing.JComponent;
import javax.swing.event.MouseInputAdapter;

/**
 *
 * @author Henry
 */
public class ScrollbarLockIcon extends JComponent   {

   /**
    * This enum tracks the possible states of the lock icon. The difference
    * between "locked" and "superlocked" is that in the locked state, we will
    * still flash the display to newly-acquired images for a brief period.
    */
   public enum LockedState {
      UNLOCKED, LOCKED, SUPERLOCKED
   }

   /**
    * This event informs listeners of when the lock button is toggled.
    */
   public static class LockEvent {
      private String axis_;
      private LockedState lockedState_;
      public LockEvent(String axis, LockedState lockedState) {
         axis_ = axis;
         lockedState_ = lockedState;
      }
      public String getAxis() {
         return axis_;
      }
      public LockedState getLockedState() {
         return lockedState_;
      }
   }

   private static final int WIDTH = 17, HEIGHT = 14;
   private LockedState lockedState_;
   private String axis_;
   private EventBus bus_;
   private final Color BACKGROUND_COLOR = Color.white;
   private final Color LOCK_COLOR = Color.black;
   private final Color SUPERLOCK_COLOR = Color.red;
   private Color foreground_ = LOCK_COLOR;
   
   public ScrollbarLockIcon(final String axis, final EventBus bus) {
      lockedState_ = LockedState.UNLOCKED;
      axis_ = axis;
      bus_ = bus;
      setSize(WIDTH, HEIGHT);
      this.setToolTipText("Lock the scrollbar to its current postion");
      this.addMouseListener(new MouseInputAdapter() {
         @Override
         public void mouseClicked(MouseEvent e) {
            advanceLockedState();
         }
      });
   }

   private void advanceLockedState() {
      switch (lockedState_) {
         case UNLOCKED:
            setLockedState(LockedState.LOCKED);
            break;
         case LOCKED:
            setLockedState(LockedState.SUPERLOCKED);
            break;
         default:
            setLockedState(LockedState.UNLOCKED);
            break;
      }
   }

   public void setLockedState(LockedState state) {
      lockedState_ = state;
      foreground_ = (lockedState_ == LockedState.SUPERLOCKED) ? SUPERLOCK_COLOR : LOCK_COLOR;
      bus_.post(new LockEvent(axis_, lockedState_));
      repaint();
   }

   public LockedState getLockedState() {
      return lockedState_;
   }

   /**
    * Return true if we are in LOCKED or SUPERLOCKED state.
    */
   public boolean getIsLocked() {
      return (lockedState_ == LockedState.LOCKED || 
            lockedState_ == LockedState.SUPERLOCKED);
   }

   /**
    * Overrides Component getPreferredSize().
    */
   @Override
   public Dimension getPreferredSize() {
      return new Dimension(WIDTH, HEIGHT);
   }
   
   @Override
   public Dimension getMinimumSize() {
       return new Dimension(WIDTH, HEIGHT);
   }
   
   @Override
    public Dimension getMaximumSize() {
       return new Dimension(WIDTH, HEIGHT);
   }

   @Override
   public void paint(Graphics g) {
      if (g == null) {
         return;
      }
      g.setColor(Color.white);
      g.fillRect(0, 0, WIDTH, HEIGHT);
      Graphics2D g2d = (Graphics2D) g;
      g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
      if (lockedState_ == LockedState.UNLOCKED) {
         drawUnlocked(g2d);
      } 
      else {
         drawLocked(g2d);
      }
   }
   
   private void drawUnlocked(Graphics2D g) {
      g.setColor(foreground_);
      //body
      g.fillRect(1, 7, 10, 6);

      //lock part
      g.fillRect(8, 4, 2, 3);
      g.fillRect(14, 4, 2, 3);

      g.fillArc(8, 1, 8, 8, 0, 180);
      g.setColor(BACKGROUND_COLOR);
      g.fillArc(10, 3, 4, 4, 0, 180);
   }

   private void drawLocked(Graphics2D g) {
      g.setColor(foreground_);   
      //body
      g.fillRect(1, 7, 10, 6);
      
      //lock part
      g.fillRect(2, 4, 2, 3);
      g.fillRect(8, 4, 2, 3);
      
      g.fillArc(2, 1, 8, 8, 0, 180);
      g.setColor(BACKGROUND_COLOR);
      g.fillArc(4, 3, 4, 4, 0, 180);
   }
}

