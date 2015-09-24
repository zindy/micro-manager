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

import java.awt.Point;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;

/**
 *
 * @author Wayne
 */
public class VectorGraphUtility {
    public static final float SelectRange = 10;
    
    public static double DistanceSquareBetweenPoints( double x1, double y1, double x2, double y2 )
    {
        double d1 = x1 - x2;
        double d2 = y1 - y2;
        double d = d1 * d1 + d2 * d2;
        if( d <= 0 )
            return 0;
        else
            return d;
    }
    public static double DistanceBetweenPoints( double x1, double y1, double x2, double y2 )
    {
        return Math.sqrt( DistanceSquareBetweenPoints( x1, y1, x2, y2 ) );
    }
    
    public static double DistanceSquareToLine( double x, double y, double x1, double y1, double x2, double y2 )
    {
        double d1 = x1 - x2;
        double d2 = y1 - y2;
        double d = d1 * d1 + d2 * d2;
        if( d <= 0 )
            return 0;
        double n = x * y1 - x1 * y + x1 * y2 - x2 * y1 + x2 * y - x * y2;
        return n * n / d;
    }
    public static double DistanceToLine( double x, double y, double x1, double y1, double x2, double y2 )
    {
        return Math.sqrt( DistanceSquareToLine( x, y, x1, y1, x2, y2 ) );
    }
    
    /**
     *
     * @param pt
     * @param LinePt1
     * @param LinePt2
     * @return
     */
    public static double DistanceSquareToLine( Point pt, Point2D.Float LinePt1, Point2D.Float LinePt2 )
    {
        return DistanceSquareToLine( pt.x, pt.y, LinePt1.x, LinePt1.y, LinePt2.x, LinePt2. y);
    }
    public static double DistanceToLine( Point pt, Point2D.Float LinePt1, Point2D.Float LinePt2 )
    {
        return DistanceToLine( pt.x, pt.y, LinePt1.x, LinePt1.y, LinePt2.x, LinePt2. y);
    }
    
    public static boolean IsLineSelected( Point pt, Point2D.Float pt1, Point2D.Float pt2 )
    {
        Rectangle2D.Float rc = new Rectangle2D.Float();
        if( pt1.x < pt2.x ){
            rc.x = pt1.x;
            rc.width = pt2.x - pt1.x;
        }
        else{
            rc.x = pt2.x;
            rc.width = pt1.x - pt2.x;
        }
        if( pt1.y < pt2.y ){
            rc.y = pt1.y;
            rc.height = pt2.y - pt1.y;
        }
        else{
            rc.y = pt2.y;
            rc.height = pt1.y - pt2.y;
        }
        rc.setRect( rc.x - SelectRange, rc.y - SelectRange, rc.width + SelectRange * 2, rc.height + SelectRange * 2 );
        if( !rc.contains( pt ) )
            return false;
        return DistanceSquareToLine( pt, pt1, pt2 ) <= SelectRange * SelectRange;
    }
    
    public static boolean IsKeyPointSelected( Point pt, Point2D.Float kpt )
    {
        return DistanceSquareBetweenPoints( pt.x, pt.y, kpt.x, kpt.y ) <= SelectRange * SelectRange;
    }
    
}
