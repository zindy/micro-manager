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

package bidc;

import java.util.ArrayList;

/**
 *
 * @author Henry
 */
public abstract class FrameIntegrationMethod {

   public static final int FRAME_AVERAGE = 0, RANK_FILTER = 1, FRAME_SUMMATION = 2, BURST_MODE = 3;;
   protected int width_, height_;
   protected ArrayList<RawBufferWrapper> rawBuffers_;
   protected int doubleWidth_, numFrames_;
   private int offset_;


   public FrameIntegrationMethod(int doubleWidth, int offset, int numFrames) {
      offset_ = offset;
      numFrames_ = numFrames;
      doubleWidth_ = doubleWidth;
      rawBuffers_ = new ArrayList<RawBufferWrapper>();
      width_ = RawBufferWrapper.getWidth();
      height_ = RawBufferWrapper.getHeight();
   }

   
   /**
    * Add a single 
    * @param buffer 
    */
   public void addBuffer(byte[] buffer) {
      rawBuffers_.add(new RawBufferWrapper(buffer, offset_, doubleWidth_));
   }

   public int getConstructedImageWidth() {
      return width_;
   }
   
   public int getConstructedImageHeight() {
      return height_;
   }

   public abstract Object constructImage();
}
