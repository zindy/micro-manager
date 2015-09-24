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

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Wayne
 */
public class SessionThread extends Thread {
    private final SLM slm_;
    private final Mapping mapping_;
    private final ArrayList seq_;
    private final Rectangle aoi_;
    
    private boolean masterMode_;
    private int patternPeriod_;
    private int loopCount_;
    private boolean positiveEdgeTrigger_;
    
    private boolean stop_;
    
    SessionThread(SLM slm, Mapping mapping, ArrayList seq, Rectangle aoi)
    {
        slm_ = slm;
        mapping_ = mapping;
        seq_ = seq;
        aoi_ = aoi;
    }
    
    public void setMasterMode( boolean b )
    {
        masterMode_ = b;
    }
    
    public void setMasterModeParameters( int period, int loops )
    {
        patternPeriod_ = period;
        loopCount_ = loops;
    }
    
    public void setSlaveModeParameters( boolean positiveEdgeTrigger )
    {
        positiveEdgeTrigger_ = positiveEdgeTrigger;
    }
    
    @Override
    public void run()
    {
        LinkedList<byte[]> patterns = new LinkedList<byte[]>();

        Rectangle2D.Float aoi2D = new Rectangle2D.Float( aoi_.x, aoi_.y, aoi_.width, aoi_.height );
        for (Object obj : seq_) {
            Pattern ptn = (Pattern) obj;
            BufferedImage bi = new BufferedImage( slm_.getWidth(), slm_.getHeight(), BufferedImage.TYPE_BYTE_GRAY );
            Graphics2D g = bi.createGraphics();
            g.setTransform( mapping_.getCameraToPolygonAffineTransform() );
            g.drawImage( ptn.bi, aoi_.x, aoi_.y, aoi_.width, aoi_.height, null );
            ptn.vg.draw( g, aoi2D );
            DataBufferByte dbb = (DataBufferByte)bi.getRaster().getDataBuffer();
            patterns.add( dbb.getData() );
            if( stop_ )
                return;
        }
        slm_.setTriggerType( masterMode_ ? 0 : ( positiveEdgeTrigger_ ? 2 : 3 ) );
        slm_.loadSequence( patterns );
        if( stop_ )
            return;
        slm_.startSequence();
        if( stop_ )
            return;
        if( masterMode_ ){
            for( int i = 0; i < seq_.size() * loopCount_; i++ ){
                try {
                    slm_.commandTrigger();
                    Thread.sleep( patternPeriod_ );
                } catch (Exception ex) {
                    Logger.getLogger(SessionThread.class.getName()).log(Level.SEVERE, null, ex);
                }
                if( stop_ )
                    break;
            }
            slm_.stopSequence();
        }
    }
    
    public void pleaseStop()
    {
        stop_ = true;
    }
}
