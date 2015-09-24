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

package acq;

import java.util.concurrent.BlockingQueue;
import misc.Log;
import misc.MD;

/**
 * Dequeue tagged images and append to image cache
 *
 * copied from MM DefaultMagellanTaggedImageQueue
 */
public class TaggedImageSink  {

   private final BlockingQueue<MagellanTaggedImage> imageProducingQueue_;
   private MMImageCache imageCache_ = null;
   private volatile String lastImageLabel_;
   private Thread savingThread_;
   private Acquisition acq_;
   
   public TaggedImageSink(BlockingQueue<MagellanTaggedImage> imageProducingQueue,
           MMImageCache imageCache, Acquisition acq) {
      imageProducingQueue_ = imageProducingQueue;
      imageCache_ = imageCache;
      acq_ = acq;
   }
   
   public String getLastImageLabel() {
      return lastImageLabel_;
   }
   
   public void waitToDie() {
      try {
         savingThread_.join();
      } catch (InterruptedException ex) {
         throw new RuntimeException("saving Thread interrupted");
      }
   }
   
   public void start() {
      savingThread_ = new Thread(acq_.getName() +": Tagged image sink thread") {

         @Override
         public void run() {
            long t1 = System.currentTimeMillis();
            int imageCount = 0;
            while (true) {
               MagellanTaggedImage image;
               try {
                  image = imageProducingQueue_.take();
               } catch (InterruptedException ex) {
                  //shouldn't ever happen because signal images are what terminates this thread
                  break;
               }
               if (image != null) {
                  if (SignalTaggedImage.isAcquisitionFinsihedSignal(image)) {
                     break;
                  } else if (SignalTaggedImage.isTimepointFinishedSignal(image)) {
                     ((FixedAreaAcquisition) acq_).imagesAtTimepointFinishedWriting();
                     continue;
                  } else {
                     ++imageCount;
                     try {
                        imageCache_.putImage(image);
                     } catch (Exception ex) {
                        Log.log("Couldn't add image to storage");
                     }
                     lastImageLabel_ = MD.getLabel(image.tags);
                  }
               } 
            }
            long t2 = System.currentTimeMillis();
            Log.log(imageCount + " images stored in " + (t2 - t1) + " ms.", false);
            acq_.markAsFinished();
            imageCache_.finished();
            if (acq_ instanceof FixedAreaAcquisition) {
               //once everything is done, can signal back upstream to multiple acquisition manager
               ((FixedAreaAcquisition) acq_).imagesAtTimepointFinishedWriting();
            }
         }
      };
      savingThread_.start();
   }

}
