/*
 * AUTHOR:
 * Arthur Edelstein
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

package org.micromanager.acquisition;

import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import mmcorej.TaggedImage;
import org.micromanager.api.DataProcessor;
import org.micromanager.utils.ReportingUtils;

/**
 * Sets up a queue of DataProcessors
 * The static method "run" will chain the given list of DataProcessors to 
 * inputqueue, and return an output queue.  The net result is that each 
 * DataProcessor will modify the image and pass it along to the next 
 * DataProcessor
 *
 * @author arthur
 */
public class ProcessorStack<E> {

   private final List<DataProcessor<E>> processors_;
   private final BlockingQueue<E> input_;
   private final BlockingQueue<E> output_;
   public ProcessorStack(BlockingQueue<E> input,
           List<DataProcessor<E>> processors) {
      processors_ = processors;
      input_ = input;

      BlockingQueue<E> left = input_;
      BlockingQueue<E> right = left;
      if (processors_ != null) {
         for (DataProcessor<E> processor : processors_) {
            if (processor.getIsEnabled()) {
               right = new LinkedBlockingQueue<E>(1);
               processor.setInput(left);
               processor.setOutput(right);
               left = right;
            }
         }
      }
      output_ = right;
   }

   public BlockingQueue<E> begin() {
      start();
      return output_;
   }

   public void start() {
      for (DataProcessor<E> processor : processors_) {
         if (!processor.isAlive()) {
            if (processor.isStarted()) {
               ReportingUtils.showError("Processor: " + processor.getName()
                       + " is no longer running. Remove and re-insert to get it to go again");
            } else {
               processor.start();
            }
         }
      }
   }

   private static Object processorInputOutputLock_ = new Object();

   /**
    * Sets up the DataProcessor<TaggedImage> sequence
    * @param inputTaggedImageQueue
    * @param imageProcessors
    * @return 
    */
   public static BlockingQueue<TaggedImage> run(
           BlockingQueue<TaggedImage> inputTaggedImageQueue, 
           List<DataProcessor<TaggedImage>> imageProcessors) {
      synchronized(processorInputOutputLock_) {
         ProcessorStack<TaggedImage> processorStack =
              new ProcessorStack<TaggedImage>(inputTaggedImageQueue, imageProcessors);
         return processorStack.begin();
      }
   }
   
}
