/*
 * Mightex Polygon400 plugin.
 *
 * AUTHOR:
 * Wayne Liao, mightexsystem.com, 05/15/2015
 *
 * Copyright (c) 2015 Mightex Systems
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

package org.micromanager.polygon;

import ij.process.ImageProcessor;
import java.awt.image.BufferedImage;
import mmcorej.CMMCore;
import mmcorej.TaggedImage;
import org.micromanager.api.ScriptInterface;
import org.micromanager.utils.ImageUtils;

/**
 *
 * @author Wayne
 */
public class Camera {
    private final ScriptInterface app_;
    private final CMMCore core_;
    
    Camera( ScriptInterface app )
    {
        app_ = app;
        core_ = app.getMMCore();
    }
    
    int getImageWidth()
    {
        return (int)core_.getImageWidth();
    }
    
    int getImageHeight()
    {
        return (int)core_.getImageHeight();
    }
    
    TaggedImage getTaggedImage() throws Exception
    {
        if( !app_.isLiveModeOn() )
            core_.snapImage();
        return core_.getTaggedImage();
    }
    
    ImageProcessor getImageProcessor() throws Exception
    {
        TaggedImage img = getTaggedImage();
        return ImageUtils.makeProcessor(img);
    }
    
    ImageProcessor getMonochromeProcessor() throws Exception
    {
        TaggedImage img = getTaggedImage();
        return ImageUtils.makeMonochromeProcessor(img);
    }

    BufferedImage getBufferedImage() throws Exception
    {
        TaggedImage img = getTaggedImage();
        ImageProcessor proc = ImageUtils.makeProcessor(img);
        return proc.getBufferedImage();
    }
}
