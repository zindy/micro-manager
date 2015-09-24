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

/*
 * 
 */
package org.micromanager.utils;

import ij.gui.ImageCanvas;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Class to manage calls to ImageJ Canvas.PaintPending
 * Micro-Manager has several classes that need to know whether a paint to 
 * a specific Canvas is pending, but they can easily deadlock.
 * This class keeps track of whom set the PaintPending and will only tell
 * whomever set PaintPending that a Paint is indeed pending
 * @author nico
 */
public class CanvasPaintPending {
   public static Map<ImageCanvas, List<Object>> requesters_ = 
           new HashMap<ImageCanvas, List<Object>>();
   
   /**
    * Checks if the calling class has a paint pending 
    * If there is no paint pending, it will remove all previous requests
    * 
    * @param canvas - ImageCanvas for which to check if a paint is pending
    * @param caller - Calling class
    * 
    * @return - true if a paint is pending and has been requested by the calling
    * class, false otherwise.
    */
   public static synchronized boolean isMyPaintPending (ImageCanvas canvas, 
           Object caller) {
      if (canvas.getPaintPending()) {
         List<Object> objectList = requesters_.get(canvas);
         if (objectList!= null) {
            if (objectList.contains(caller)) {
               return true;
            }
         }
      }
      // no Paint Pending.  Erase all past calls to setPaintPending
      else {
         requesters_.remove(canvas);
      }
      return false;
   }
   
   
   public static synchronized void setPaintPending (ImageCanvas canvas, 
           Object caller) {
      canvas.setPaintPending(true);
      List<Object> objectList = requesters_.get(canvas);
      if (objectList == null) {
         objectList = new ArrayList<Object>();
         requesters_.put(canvas, objectList);
      }
      if (!objectList.contains(caller) ) {
         objectList.add(caller);
      }   
   }
   
   public static synchronized void removePaintPending(ImageCanvas canvas, 
           Object caller) {
      List<Object> objectList = requesters_.get(canvas);
      if (objectList != null && objectList.contains(caller)) {
         objectList.remove(caller);
         if (objectList.isEmpty()) {
            canvas.setPaintPending(false);
            requesters_.remove(canvas);
         }
      }
   }

   public static synchronized void removeAllPaintPending(ImageCanvas canvas) {
      if (requesters_.containsKey(canvas)) {
         requesters_.remove(canvas);
      }
   }   
}
