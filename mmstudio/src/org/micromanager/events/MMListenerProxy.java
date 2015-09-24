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

package org.micromanager.events;

import com.google.common.eventbus.EventBus;

import org.micromanager.api.MMListenerInterface;

import org.micromanager.api.events.ConfigGroupChangedEvent;
import org.micromanager.api.events.ExposureChangedEvent;
import org.micromanager.api.events.PixelSizeChangedEvent;
import org.micromanager.api.events.PropertiesChangedEvent;
import org.micromanager.api.events.PropertyChangedEvent;
import org.micromanager.api.events.SLMExposureChangedEvent;
import org.micromanager.api.events.StagePositionChangedEvent;
import org.micromanager.api.events.SystemConfigurationLoadedEvent;
import org.micromanager.api.events.XYStagePositionChangedEvent;

/**
 * This class wraps around each of the functions specified in 
 * MMListenerInterface, publishing an event for each, so that classes that are
 * only interested in some of the functions are not required to provide stub
 * implementations for all of them.
 */
public class MMListenerProxy implements MMListenerInterface {
   private EventBus bus_;
   public MMListenerProxy(EventBus bus) {
      bus_ = bus;
   }

   @Override
   public void propertiesChangedAlert() {
      bus_.post(new PropertiesChangedEvent());
   }

   @Override
   public void propertyChangedAlert(String device, String property, String value) {
      bus_.post(new PropertyChangedEvent(device, property, value));
   }

   @Override
   public void configGroupChangedAlert(String groupName, String newConfig) {
      bus_.post(new ConfigGroupChangedEvent(groupName, newConfig));
   }

   @Override
   public void systemConfigurationLoaded() {
      bus_.post(new SystemConfigurationLoadedEvent());
   }

   @Override
   public void pixelSizeChangedAlert(double newPixelSizeUm) {
      bus_.post(new PixelSizeChangedEvent(newPixelSizeUm));
   }

   @Override
   public void stagePositionChangedAlert(String deviceName, double pos) {
      bus_.post(new StagePositionChangedEvent(deviceName, pos));
   }

   @Override
   public void xyStagePositionChanged(String deviceName, double xPos, double yPos) {
      bus_.post(new XYStagePositionChangedEvent(deviceName, xPos, yPos));
   }

   @Override
   public void exposureChanged(String cameraName, double newExposureTime) {
      bus_.post(new ExposureChangedEvent(cameraName, newExposureTime));
   }
   
   @Override
   public void slmExposureChanged(String cameraName, double newExposureTime) {
      bus_.post(new SLMExposureChangedEvent(cameraName, newExposureTime));
   }
}

