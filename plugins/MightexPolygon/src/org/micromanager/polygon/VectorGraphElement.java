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

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;

/**
 *
 * @author Wayne
 */
public interface VectorGraphElement extends java.io.Serializable {
    public static final float KeyPointRadius = 2;
    public static final Color SelectedColor = Color.GREEN;

    // set color
    public void setColor( Color color );
    
    // set pen width
    public void setWidth( float width );
    
    // compare
    public boolean isSameAs( VectorGraphElement vg );
    
    // clone
    public VectorGraphElement clone();
    
    // check if object is within range of view
    public boolean isInRange();
    
    // centre point
    public Point2D.Float getCentre();
    
    // draw object in a rectangle
    public void draw( Graphics g, Rectangle2D.Float r, boolean selected );
    
    // check if a mouse click selects the object
    public boolean isSelected( Point pt, Rectangle2D.Float r );
    
    // key point selection
    public int selectKeyPoint( Point pt, Rectangle2D.Float r );
    public Cursor getKeyPointCursor( int kpidx );
    
    // move object
    public void move( float xOfs, float yOfs );
    
    // move key point
    public void moveKeyPoint( int kpidx, Point2D.Float pt );
    
}
