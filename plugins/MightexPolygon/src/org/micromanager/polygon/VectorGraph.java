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
import java.awt.Point;
import java.awt.geom.Rectangle2D;
import java.util.ArrayList;

/**
 *
 * @author Wayne
 */
public class VectorGraph implements java.io.Serializable {
    
    private final ArrayList elements_;

    public VectorGraph()
    {
        elements_ = new ArrayList();
    }
    
    public int getCount()
    {
        return elements_.size();
    }
    VectorGraphElement getVectorGraphElement( int idx )
    {
        return (VectorGraphElement)elements_.get(idx);
    }
    void setVectorGraphElement( int idx, VectorGraphElement e )
    {
        elements_.set( idx, e );
    }
    
    public void add( VectorGraphElement e )
    {
        elements_.add( e );
    }
    public void remove( int idx )
    {
        elements_.remove( idx );
    }
    public void clear()
    {
        elements_.clear();
    }
    
    public void draw( Graphics g, Rectangle2D.Float r )
    {
        for( Object e : elements_){
            ((VectorGraphElement)e).draw( g, r,  false );
        }
    }
    
    public int select( Point pt, Rectangle2D.Float r )
    {
        for( int i = elements_.size() - 1; i >= 0; i-- ){
            VectorGraphElement e = (VectorGraphElement) elements_.get(i);
            if( e.isSelected( pt, r ) )
                return i;
        }
        return -1;
    }
    
    
    @Override
    public VectorGraph clone() throws CloneNotSupportedException
    {
        VectorGraph vg = new VectorGraph();
        for( Object e : elements_ ){
            vg.add( ((VectorGraphElement)e).clone() );
        }
        return vg;
    }
    
}
