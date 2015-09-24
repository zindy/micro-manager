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

package org.micromanager.acquisition;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;
import mmcorej.TaggedImage;
import org.micromanager.api.ImageCache;
import org.micromanager.utils.ReportingUtils;

/**
 * Dequeue tagged images and append to image cache
 *
 * @author arthur
 */
public class DefaultTaggedImageSink  {

   private final BlockingQueue<TaggedImage> imageProducingQueue_;
   private ImageCache imageCache_ = null;

   public DefaultTaggedImageSink(BlockingQueue<TaggedImage> imageProducingQueue,
                  ImageCache imageCache) {
      imageProducingQueue_ = imageProducingQueue;
         imageCache_ = imageCache;
   }

   public void start() {
      start(null);
   }

   // sinkFullCallback is a way to stop production of images when/if the sink
   // can no longer accept images.
   public void start(final Runnable sinkFullCallback) {
      Thread savingThread = new Thread("tagged image sink thread") {

         @Override
         public void run() {
            long t1 = System.currentTimeMillis();
            int imageCount = 0;
            try {
               while (true) {
                  TaggedImage image = imageProducingQueue_.poll(1, TimeUnit.SECONDS);
                  if (image != null) {
                     if (TaggedImageQueue.isPoison(image)) {
                        break;
                     }
                     ++imageCount;
                     try {
                        imageCache_.putImage(image);
                     }
                     catch (OutOfMemoryError e) {
                        handleOutOfMemory(e, sinkFullCallback);
                        break;
                     }
                  }
               }
            } catch (Exception ex2) {
               ReportingUtils.logError(ex2);
            }
            long t2 = System.currentTimeMillis();
            ReportingUtils.logMessage(imageCount + " images stored in " + (t2 - t1) + " ms.");
            imageCache_.finished();
         }
      };
      savingThread.start();
   }

   // Never called from EDT
   private void handleOutOfMemory(final OutOfMemoryError e,
         Runnable sinkFullCallback)
   {
      ReportingUtils.logError(e);
      if (sinkFullCallback != null) {
         sinkFullCallback.run();
      }

      SwingUtilities.invokeLater(new Runnable() {
         @Override
         public void run() {
            JOptionPane.showMessageDialog(null,
                  "Out of memory to store images: " + e.getMessage(),
                  "Out of image storage memory", JOptionPane.ERROR_MESSAGE);
         }
      });
   }

   public ImageCache getImageCache() {
      return imageCache_;
   }
}
