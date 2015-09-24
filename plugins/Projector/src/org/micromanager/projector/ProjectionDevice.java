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
import java.util.List;

public interface ProjectionDevice {
   // Properties of device.
   public String getName();
   public String getChannel();
   public double getXRange();
   public double getYRange();
   public double getXMinimum();
   public double getYMinimum();
 
   // ## Alert when something has changed.
   public void addOnStateListener(OnStateListener listener);

   // ## Get/set internal exposure setting
   public long getExposure();
   public void setExposure(long interval_us);

   // ## Control illumination
   public void turnOn();
   public void turnOff();
   public void displaySpot(double x, double y);
   public void activateAllPixels();

   // ## ROIs
   public void loadRois(List<FloatPolygon> rois);
   public void setPolygonRepetitions(int reps);
   public void runPolygons();

   public void waitForDevice();
}
