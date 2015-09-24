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
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import javax.imageio.ImageIO;

/**
 *
 * @author Wayne
 */
public class Pattern implements LoadSave {
    public BufferedImage bi;
    public VectorGraph vg;

    public Pattern()
    {
        ClearBackground();
        vg = new VectorGraph();
    }
    
    public Pattern( BufferedImage b, VectorGraph v )
    {
        bi = b;
        vg = v;
    }
    
    public Pattern clone() throws CloneNotSupportedException
    {
        BufferedImage bi2 = Utility.DuplicateBufferedImage( bi );
        VectorGraph vg2 = vg.clone();
        return new Pattern( bi2, vg2 );
    }
    
    public void draw( Graphics g, Rectangle2D.Float r )
    {
        if(bi!=null)
            g.drawImage(bi, (int)r.x, (int)r.y, (int)r.width, (int)r.height, null);
        vg.draw(g, r);
    }
    
    public void ClearBackground()
    {
        bi = new BufferedImage(1, 1, BufferedImage.TYPE_BYTE_GRAY);
    }
    
    public void ClearVectorGraph()
    {
        vg.clear();
    }
    public void Clear()
    {
        ClearBackground();
        ClearVectorGraph();
    }
    
    @Override
    public boolean Save(String fn)
    {
        ObjectOutputStream os = null;
        try {
            os = new ObjectOutputStream(new FileOutputStream(fn));
            os.writeObject(vg);
            ImageIO.write(bi, "jpg", os);
            os.close();
            return true;
        }
        catch( IOException ex ){
            if( os != null ){
                try {
                    os.close();
                }
                catch( IOException ex2 ){
                }
            }
            return false;
        }
    }
    
    @Override
    public boolean Load(String fn)
    {
        ObjectInputStream is = null;
        try {
            is = new ObjectInputStream(new FileInputStream(fn));
            vg = (VectorGraph) is.readObject();
            bi = (BufferedImage) ImageIO.read(is);
            is.close();
            return true;
        }
        catch( Exception ex ) {
            if( is != null ){
                try {
                    is.close();
                }
                catch( Exception ex2 ){
                }
            }
            return false;
        }
    }
            
}
