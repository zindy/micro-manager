/*
 * AUTHOR:
 * Arthur Edelstein
 *
 * Copyright (c) 2010-2014 Regents of the University of California
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

package org.micromanager.projector;

import ij.process.FloatPolygon;
import java.awt.geom.Point2D;
import java.util.HashSet;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import mmcorej.CMMCore;
import org.micromanager.utils.ReportingUtils;

public class Galvo implements ProjectionDevice {

   String galvo_;
   CMMCore mmc_;
   int side_ = 4096;
   ExecutorService galvoExecutor_;
   HashSet<OnStateListener> onStateListeners_ = new HashSet<OnStateListener>();
   long interval_us_;

   public Galvo(CMMCore mmc) {
      mmc_ = mmc;
      galvo_ = mmc_.getGalvoDevice();
      galvoExecutor_ = Executors.newSingleThreadExecutor();
   }

   @Override
   public String getName() {
       return galvo_;
   }
   
   @Override
   public void displaySpot(final double x, final double y) {
      galvoExecutor_.execute(new Runnable() {
         @Override
         public void run() {
            try {
               mmc_.pointGalvoAndFire(galvo_, x, y, Galvo.this.getExposure());
            } catch (Exception ex) {
               ReportingUtils.logError(ex);
            }
         }
      });
   }

   @Override
   public void waitForDevice() {
      Future result = galvoExecutor_.submit(new Runnable() {
         @Override
         public void run() {
            // do nothing;
         }
      });
      try {
         result.get();
      } catch (InterruptedException ex) {
         ReportingUtils.logError(ex);
      } catch (ExecutionException ex) {
         ReportingUtils.logError(ex);
      }
   }

   @Override
   public double getXRange() {
      try {
         Double result = galvoExecutor_.submit(new Callable<Double>() {

            @Override
            public Double call() {
               try {
                  return mmc_.getGalvoXRange(galvo_);
               } catch (Exception ex) {
                  return 0.0;
               }
            }
         }).get();
         if (result == 0) {
            ReportingUtils.logError("Unable to get galvo width");
         }
         return result;
      } catch (InterruptedException ex) {
         ReportingUtils.logError("Unable to get galvo width");
         return 0;
      } catch (ExecutionException ex) {
         ReportingUtils.logError("Unable to get galvo width");
         return 0;
      }
   }
   
   @Override
   public double getYRange() {
      try {
         Double result = galvoExecutor_.submit(new Callable<Double>() {

            @Override
            public Double call() {
               try {
                  return mmc_.getGalvoYRange(galvo_);
               } catch (Exception ex) {
                  return 0.0;
               }
            }
         }).get();
         if (result == 0) {
            ReportingUtils.logError("Unable to get galvo height");
         }
         return result;
      } catch (InterruptedException ex) {
         ReportingUtils.logError("Unable to get galvo height");
         return 0;
      } catch (ExecutionException ex) {
         ReportingUtils.logError("Unable to get galvo height");
         return 0;
      }
   }
   
   @Override
   public double getXMinimum() {
      try {
         Double result = galvoExecutor_.submit(new Callable<Double>() {

            @Override
            public Double call() {
               try {
                  return mmc_.getGalvoXMinimum(galvo_);
               } catch (Exception ex) {
                  return 0.0;
               }
            }
         }).get();
         if (result == 0) {
            ReportingUtils.logError("Unable to get galvo X minimum");
         }
         return result;
      } catch (InterruptedException ex) {
         ReportingUtils.logError("Unable to get galvo X minimum");
         return 0;
      } catch (ExecutionException ex) {
         ReportingUtils.logError("Unable to get galvo X minimum");
         return 0;
      }
   }
   
   @Override
   public double getYMinimum() {
      try {
         Double result = galvoExecutor_.submit(new Callable<Double>() {

            @Override
            public Double call() {
               try {
                  return mmc_.getGalvoYMinimum(galvo_);
               } catch (Exception ex) {
                  return 0.0;
               }
            }
         }).get();
         if (result == 0) {
            ReportingUtils.logError("Unable to get galvo Y minimum");
         }
         return result;
      } catch (InterruptedException ex) {
         ReportingUtils.logError("Unable to get galvo Y minimum");
         return 0;
      } catch (ExecutionException ex) {
         ReportingUtils.logError("Unable to get galvo Y minimum");
         return 0;
      }
   }

   @Override
   public void turnOn() {
      galvoExecutor_.submit(new Runnable() {
         @Override
         public void run() {
            try {
               mmc_.setGalvoIlluminationState(galvo_, true);
            } catch (Exception ex) {
               ReportingUtils.showError(ex);
            }
         }
      });
      for (OnStateListener listener:onStateListeners_) {
        listener.stateChanged(true);
      }
   }

   @Override
   public void turnOff() {
      galvoExecutor_.submit(new Runnable() {
         @Override
         public void run() {
            try {
               mmc_.setGalvoIlluminationState(galvo_, false);
            } catch (Exception ex) {
               ReportingUtils.showError(ex);
            }
         }
      });
      for (OnStateListener listener:onStateListeners_) {
        listener.stateChanged(false);
      }
   }

   @Override
   public void loadRois(final List<FloatPolygon> rois) {
      galvoExecutor_.submit(new Runnable() {
         @Override
         public void run() {
            try {
               mmc_.deleteGalvoPolygons(galvo_);
            } catch (Exception ex) {
               ReportingUtils.logError(ex);
            }
            int roiCount = 0;
            try {
               for (FloatPolygon poly : rois) {
                  Point2D lastGalvoPoint = null;
                  for (int i = 0; i < poly.npoints; ++i) {
                     Point2D.Double galvoPoint = new Point2D.Double(
                             poly.xpoints[i], poly.ypoints[i]);
                     if (i == 0) {
                        lastGalvoPoint = galvoPoint;
                     }
                     mmc_.addGalvoPolygonVertex(galvo_, roiCount, galvoPoint.getX(), 
                             galvoPoint.getY());
                     if (poly.npoints == 1) {
                        ++roiCount;
                     }
                  }
                  if (poly.npoints > 1) {
                     mmc_.addGalvoPolygonVertex(galvo_, roiCount, 
                             lastGalvoPoint.getX(), lastGalvoPoint.getY());
                     ++roiCount;
                  }
               }
            } catch (Exception e) {
               ReportingUtils.showError(e);
            }

            try {
               mmc_.loadGalvoPolygons(galvo_);
            } catch (Exception ex) {
               ReportingUtils.showError(ex);
            }
         }
      });
   }


   @Override
   public void runPolygons() {
      galvoExecutor_.submit(new Runnable() {
         @Override
         public void run() {
            try {
               mmc_.runGalvoPolygons(galvo_);
            } catch (Exception ex) {
               ReportingUtils.showError(ex);
            }
         }
      });

   }

   @Override
   public void addOnStateListener(OnStateListener listener) {
      onStateListeners_.add(listener);
   }

   public void removeOnStateListener(OnStateListener listener) {
      onStateListeners_.remove(listener);
   }

   @Override
   public void setPolygonRepetitions(final int reps) {
      galvoExecutor_.submit(new Runnable() {
         @Override
         public void run() {

            try {
               mmc_.setGalvoPolygonRepetitions(galvo_, reps);
            } catch (Exception ex) {
               ReportingUtils.showError(ex);
            }
         }
      });
   }

    @Override
    public String getChannel() {
        Future<String> channel = galvoExecutor_.submit(new Callable<String>() {
            @Override
            public String call() {
                try {
                    return mmc_.getGalvoChannel(galvo_);
                } catch (Exception ex) {
                    ReportingUtils.logError(ex);
                    return null;
                }
            }
        });
        try {
            return channel.get();
        } catch (InterruptedException e) {
            return null;
        } catch (ExecutionException e) {
           return null;
      }
    }

   @Override
   public void setExposure(long interval_us) {
      try {
         interval_us_ = interval_us;
         mmc_.setGalvoSpotInterval(galvo_, interval_us);
      } catch (Exception ex) {
         ReportingUtils.showError(ex);
      }
   }
   
      // Reads the exposure time in us
   @Override
   public long getExposure() {
      return interval_us_;
   }

    @Override
    public void activateAllPixels() {
        // Do nothing.
    }
}
