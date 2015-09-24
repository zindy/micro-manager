/*
 * AUTHOR:
 * Nico Stuurman, nico@cmp.ucsf.edu, August 23, 2010
 *
 * Copyright (c) 2010 Regents of the University of California
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

package org.micromanager.api;

/*
 * Provides a way to submit callbacks for various hardware events in
 * Micro-Manager.
 */
public interface MMListenerInterface{
   /*
    * Implement this callback when any property has changed.
    */
   public void propertiesChangedAlert();

   /*
    * Implement this callback to be alerted when each property changes.
    */
   public void propertyChangedAlert(String device, String property, String value);

   /*
    * Implement this callback to be informed of a new value in a
    * configuration group.
    */
   public void configGroupChangedAlert(String groupName, String newConfig);

   /**
    * This function will be called when a configuration file is loaded
    * It can be used to stay informed of changes in the hardware configuration
    * Note that devices can be loaded an unloaded individually, in which case
    * this function will not be called
    */
   public void systemConfigurationLoaded();
   
   /*
    * Implement this callback to be informed when the current pixel size
    * changes.
    */
   public void pixelSizeChangedAlert(double newPixelSizeUm);

   /*
    * Implement this callback to be informed when a one-axis
    * drive (e.g. focus drive) has moved.
    */
   public void stagePositionChangedAlert(String deviceName, double pos);

   /*
    * Implement this callback to be informed when an XY stage has moved.
    */
   public void xyStagePositionChanged(String deviceName, double xPos, double yPos);
   
   /**
    * Implement to be notified that the exposure time given camera has changed
    * @param cameraName - label of camera whose exposure changed
    * @param newExposureTime - new exposure time
    */
   public void exposureChanged(String cameraName, double newExposureTime);

   /**
    * Implement to be notified that the exposure time given camera has changed
    * @param slmName - label of SLM/Galvo whose exposure changed
    * @param newExposureTime - new exposure time
    */
   public void slmExposureChanged(String slmName, double newExposureTime);
}

