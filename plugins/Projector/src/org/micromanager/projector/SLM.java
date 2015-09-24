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

import ij.gui.PolygonRoi;
import ij.gui.Roi;
import ij.process.ByteProcessor;
import ij.process.FloatPolygon;
import ij.process.ImageProcessor;
import java.awt.Color;
import java.awt.Polygon;
import java.util.HashSet;
import java.util.List;
import mmcorej.CMMCore;
import org.micromanager.utils.ReportingUtils;

public class SLM implements ProjectionDevice {

   String slm_;
   CMMCore mmc_;
   final int slmWidth_;
   final int slmHeight_;
   private final double spotDiameter_;
   private boolean imageOn_ = false;
   HashSet<OnStateListener> onStateListeners_ = new HashSet<OnStateListener>();

   // The constructor.
   public SLM(CMMCore mmc, double spotDiameter) {
      mmc_ = mmc;
      slm_ = mmc_.getSLMDevice();
      spotDiameter_ = spotDiameter;
      slmWidth_ = (int) mmc.getSLMWidth(slm_);
      slmHeight_ = (int) mmc.getSLMHeight(slm_);
   }

   // Adds a state listener that lets a third party know if we are on or off.
   @Override
   public void addOnStateListener(OnStateListener listener) {
      onStateListeners_.add(listener);
   }

   // Removes a state listener.
   public void removeOnStateListener(OnStateListener listener) {
      onStateListeners_.remove(listener);
   }

   // Returns the name of the SLM.
   @Override
   public String getName() {
      return slm_;
   }

   // Returns the SLM's width in pixels.
   @Override
   public double getXRange() {
      return this.slmWidth_;
   }

   // Returns the SLM's height in pixels.
   @Override
   public double getYRange() {
      return this.slmHeight_;
   }
   
   @Override
   public double getXMinimum() {
      return 0;
   }
   
   @Override
   public double getYMinimum() {
      return 0;
   }

   // TODO: Looks like a stub. Do we need to implement this method?
   @Override
   public String getChannel() {
      return "Default";
   }

   @Override
   public void waitForDevice() {
      try {
         mmc_.waitForDevice(slm_);
      } catch (Exception ex) {
         ReportingUtils.logError(ex);
      }
   }

   // Sets how long the SLM will be illuminated when we display an
   // image.
   @Override
   public void setExposure(long interval_us) {
      try {
         mmc_.setSLMExposure(slm_, interval_us / 1000.);
      } catch (Exception ex) {
         ReportingUtils.showError(ex);
      }
   }
   
   // Reads the exposure time in microseconds.
   @Override
   public long getExposure() {
      try {
         return (long) (mmc_.getSLMExposure(slm_) * 1000.);
      } catch (Exception ex) {
         ReportingUtils.showError(ex);
      }
      return 0;
   }

   // Makes sure all pixels are illuminated at maximum intensity (white).
   @Override
   public void activateAllPixels() {
      try {
         mmc_.setSLMPixelsTo(slm_, (short) 255);
         if (imageOn_ == true) {
            mmc_.displaySLMImage(slm_);
         }
      } catch (Exception ex) {
         ReportingUtils.showError(ex);
      }
   }

   // Fills a circular spot in an ImageJ ImageProcessor with diatemer dia.
   private static void fillSpot(ImageProcessor proc, int x, int y, double dia) {
      proc.fillOval((int) (x - dia / 2), (int) (y - dia / 2), (int) dia, (int) dia);
   }

   // Displays the location of a spot at x, y, with diameter this.spotDiameter_
   private void displaySpot(int x, int y) {
      ImageProcessor proc = new ByteProcessor(slmWidth_, slmHeight_);
      proc.setColor(Color.black);
      proc.fill();
      proc.setColor(Color.white);
      fillSpot(proc, x, y, spotDiameter_);
      try {
         mmc_.setSLMImage(slm_, (byte[]) proc.getPixels());
         mmc_.displaySLMImage(slm_);
      } catch (Throwable e) {
         ReportingUtils.showError("SLM not connecting properly.");
      }
   }

   // Display a spot at location x,y for the given duration.
   @Override
   public void displaySpot(double x, double y) {
      displaySpot((int) x, (int) y);
   }

   // Set all pixels to off.
   @Override
   public void turnOff() {
      try {
         mmc_.setSLMPixelsTo(slm_, (byte) 0);
         imageOn_ = false;
         for (OnStateListener listener : onStateListeners_) {
            listener.stateChanged(false);
         }
      } catch (Exception ex) {
         ReportingUtils.showError(ex);
      }
   }

   // Turn the SLM device on (illuminate whatever image has already been
   // uploaded).
   @Override
   public void turnOn() {
      try {
         if (imageOn_ == false) {
            mmc_.displaySLMImage(slm_);
            imageOn_ = true;
         }
         for (OnStateListener listener : onStateListeners_) {
            listener.stateChanged(true);
         }
      } catch (Exception ex) {
         ReportingUtils.showError(ex);
      }
   }

   // Convert an array of polygonal ROIs to a single pixel image. If 
   // polygonIntensities is null, then all polygons are set to white
   public byte[] roisToPixels(int width, int height, List<Polygon>roiPolygons, List<Integer> polygonIntensities) {
      ByteProcessor processor = new ByteProcessor(width, height);
      processor.setColor(Color.black);
      processor.fill();
      processor.setColor(Color.white);
      for (int i = 0; i < roiPolygons.size(); ++i) {
         Polygon roiPolygon = roiPolygons.get(i);
         if (polygonIntensities != null) {
            int intensity = polygonIntensities.get(i);
            processor.setColor(new Color(intensity, intensity, intensity));
         }
         // TODO: Fix overlapping ROIs so we choose the maximum intensity,
         // rather than simply overwriting earlier ROIs.
         if (roiPolygon.npoints == 1) {
            fillSpot(processor, roiPolygon.xpoints[0], roiPolygon.ypoints[0], spotDiameter_);
         } else {
            Roi roi = new PolygonRoi(roiPolygon, Roi.POLYGON);
            processor.fill(roi);
         }
      }
      return (byte[]) processor.getPixels();
   }

   // Convert an array of polygonal ROIs to a single pixel image.
   // All polygons are assumed to have maximum intensity (white)
   public byte[] roisToPixels(int width, int height, List<Polygon>roiPolygons) {
      return roisToPixels(width, height, roiPolygons, null);
   }

   // Convert roiPolygons to an image, and upload that image to the SLM.
   @Override
   public void loadRois(List<FloatPolygon> roiFloatPolygons) {
      try {
         List<Polygon> roiPolygons = Utils.FloatToNormalPolygon(roiFloatPolygons);
         mmc_.setSLMImage(slm_, roisToPixels(slmWidth_, slmHeight_, roiPolygons));
      } catch (Exception ex) {
         ReportingUtils.showError(ex);
      }
   }

   // This only applies to galvo devices. Don't use.
   @Override
   public void setPolygonRepetitions(int reps) {
      // Ignore!
   }

   // Assumes we have an image of polygons, and now we want to show them.
   @Override
   public void runPolygons() {
      try {
         mmc_.displaySLMImage(slm_);
      } catch (Exception ex) {
         ReportingUtils.showError(ex);
      }
   }
}
