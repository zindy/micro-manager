/*
 * AUTHOR:
 * Arthur Edelstein
 *
 * Copyright (c) 2012 Regents of the University of California
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

import mmcorej.TaggedImage;

/**
 * This interface provides a way to register callbacks to monitor the
 * state of an ImageCache. Used, for example, by the VirtualAcquisitionDisplay
 * to update the display of the image data set whenever a new image is
 * received during acquisition.
 */
public interface ImageCacheListener {

   /*
    * Implement this method to be informed when a TaggedImage or metadata
    * has been added to the image cache. May be called as many times as images
    * are received by the cache.
    */
   public void imageReceived(TaggedImage taggedImage);

   /*
    * Implement this method to be informed when no more TaggedImages are
    * expected to be added the image cache (i.e., that acquisition has
    * finished). After this method is call, no more images or image metadata
    * will be added to the image storage object. Called once.
    */
   public void imagingFinished(String path);
}
