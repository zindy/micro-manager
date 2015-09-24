/*
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

import ij.process.ImageProcessor;
import org.micromanager.utils.MMException;
import org.micromanager.utils.PropertyItem;

/*
 * This interface is used for a java-based autofocus plugin. Properly
 * compiled jars can be added to the mmautofocus directory of Micro-Manager
 * and they will be loaded into the autofocus menu. It also wraps
 * autofocus devices implemented in C++ and exposed by the core, so both
 * java and C++ based autofocus plugins can be used by this common interface.
 */
public interface Autofocus {

   public void applySettings();
   public void saveSettings();

   /*
    * Provides the autofocus plugin with a reference to the Micro-Manager GUI.
    */
   public void setApp(ScriptInterface app);

   /*
    * Run a full, one-shot autofocus protocol. Blocks until focusing is
    * finished.
    */
   public double fullFocus() throws MMException;

   /*
    * Run a single, incremental focusing step.
    */
   public double incrementalFocus() throws MMException;
   
   /*
    * Returns the number of images acquired
    */
   public int getNumberOfImages();

   /*
    * Returns a detailed status of the autofocus plugin/device.
    */
   public String getVerboseStatus();

   /*
    * Returns an array of the properties for this autofocus plugin.
    */
   public PropertyItem[] getProperties();

   /*
    * Returns an array of the names of properties for this autofocus plugin.
    */
   public String[] getPropertyNames();

   /*
    * Returns the name and value of properties for the autofocus plugin.
    */
   public PropertyItem getProperty(String key) throws MMException;

   /*
    * Sets the value of a particular property.
    */
   public void setProperty(PropertyItem p) throws MMException;

   /*
    * Gets the value of a named property.
    */
   public String getPropertyValue(String name) throws MMException;

   /*
    * Sets the value of a named property.
    */
   public void setPropertyValue(String name, String value) throws MMException;

   /*
    * Returns the current focus "score" (goodness of focus).
    */
   public double getCurrentFocusScore();

   /*
    * Returns the name of the autofocus device.
    */
   public String getDeviceName();

   /*
    * Turns on continuous autofocus. Typically used by hardware autofocus
    * devices such as the Nikon Perfect Focus (PFS).
    */
   public void enableContinuousFocus(boolean enable) throws MMException;

   /*
    * Returns true if continuous autofocus has been enabled. Typically used
    * by hardware autofocus devices such as the Nikon Perfect Focus (PFS).
    */
   public boolean isContinuousFocusEnabled() throws MMException;

   /*
    * Returns true if continuous autofocus is currently locked (successfully
    * following the specimen). Typically used by hardware autofocus devices
    * such as the Nikon Perfect Focus (PFS).
    */
   public boolean isContinuousFocusLocked() throws MMException;
   
   /**
    * Computes a focus score for the given image
    * @param impro
    * @return calculated score
    */
   public double computeScore(final ImageProcessor impro);

   /**
    * OBSOLETE - do not use this method.
    */
   public void focus(double coarseStep, int numCoarse, double fineStep, int numFine) throws MMException;
   
}
