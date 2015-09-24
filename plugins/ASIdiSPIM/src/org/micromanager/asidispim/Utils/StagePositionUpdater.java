/*
 * AUTHOR:
 * Nico Stuurman, Jon Daniels
 *
 * Copyright (c) 2014 Regents of the University of California, & ASI
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

package org.micromanager.asidispim.Utils;

import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.atomic.AtomicBoolean;

import org.micromanager.asidispim.Data.Devices;
import org.micromanager.asidispim.Data.Positions;
import org.micromanager.asidispim.Data.Properties;

/**
 *
 * @author nico
 * @author Jon
 */
public class StagePositionUpdater {
   private final List<ListeningJPanel> panels_;
   private Timer timer_;
   private final Positions positions_;
   private final Properties props_;
   private final AtomicBoolean updatingNow_ = new AtomicBoolean(false);  // true iff in middle of update right now, use to skip updates if last one is running instead of letting them pile up
   private final AtomicBoolean pauseUpdates_ = new AtomicBoolean(false);  // true iff updates temporarily disabled (e.g. running acquisition)
   private boolean timerRunning; // whether we are set to update positions currently
   private int pauseCounter_ = 0;
   
   /**
    * Utility class for stage position timer.
    * 
    * The timer will be constructed when the start function is called.
    * Panels to be informed of updated stage positions should be added
    * using the addPanel function.
    * 
    * @param positions
    * @param props
    */
   public StagePositionUpdater(Positions positions, Properties props) {
      positions_ = positions;
      props_ = props;
      panels_ = new ArrayList<ListeningJPanel>();
      updatingNow_.set(false);
      pauseUpdates_.set(false);
      timerRunning = false;
      timer_ = null;
   }
   
   private int getPositionUpdateInterval() {
      // get interval from plugin property stored as preference
      // property/pref value has units of seconds, interval_ has units of milliseconds
      int interval =  (int) (1000*props_.getPropValueFloat(Devices.Keys.PLUGIN, 
            Properties.Keys.PLUGIN_POSITION_REFRESH_INTERVAL));
      if (interval == 0) {
         interval = 1000;
      }
      return interval;
   }
   
   /**
    * Add panel to list of ListeningJPanels that get notified whenever
    * we have refreshed positions.
    * @param panel
    */
   public void addPanel(ListeningJPanel panel) {
      panels_.add(panel);
   }
   
   /**
    * Start the updater at whatever interval is.  Uses its own thread
    * via java.util.Timer.scheduleAtFixedRate().  Call stop() to stop.
    */
   public void start() {
      // end any existing updater before starting (anew)
      if (timer_ != null) {
         timer_.cancel();
      }
      timer_ = new Timer(true);
      timer_.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
               // update positions if we aren't already doing it or paused
               // this prevents building up task queue if something slows down
               if (!updatingNow_.get() && !pauseUpdates_.get()) {
                  updatingNow_.set(true);
                  oneTimeUpdate();
                  updatingNow_.set(false);
               }
            }
          }, 0, getPositionUpdateInterval());
      timerRunning = true;
   }
   
   public void restartIfRunning() {
      if (timerRunning) {
         start();
      }
   }
   
   /**
    * Stops the timer.  To restart call start().
    */
   public void stop() {
      if (timer_ != null) {
         timer_.cancel();
      }
      for (ListeningJPanel panel : panels_) {
         panel.stoppedStagePositions();
      }
      timerRunning = false;
   }
   
   /**
    * checks whether updates have been paused (likely because acquisition is running) 
    * @return 
    */
   public boolean isPaused() {
      return pauseUpdates_.get();
   }

   
   /**
    * Call this with true to temporarily turn off updates.
    * Be sure to call it again with false. Keeps an internal counter
    * so that the calling code does not need to know whether other code
    * also called the pauseUpdate function.
    * @param pause true disables updates temporarily, and increases the pause 
    * counter.  false decreases the pause counter.  When the counter returns to 
    * 0, unpauses the updater
    */
   public void pauseUpdates(boolean pause) {
      if (pause) {
         pauseCounter_++;
      } else {
         pauseCounter_--;
         // protect against the counter going negative:
         if (pauseCounter_ < 0) {
            pauseCounter_ = 0;
         }
      }
      if (pauseCounter_ < 2) {
         pauseUpdates_.set(pause);
         for (ListeningJPanel panel : panels_) {
            panel.stoppedStagePositions();
         }
      }
   }
   
   /**
    * Updates the stage positions.  Called whenever the timer "dings", or can be called separately.
    */
   public void oneTimeUpdate() {
      // update stage positions in devices
      positions_.refreshStagePositions();
      // notify listeners that positions are updated
      for (ListeningJPanel panel : panels_) {
         panel.updateStagePositions();
      }
   }
   
}
