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
import java.awt.Point;
import java.awt.image.BufferedImage;
import java.awt.image.ColorModel;
import java.awt.image.WritableRaster;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import javax.imageio.ImageIO;
import org.micromanager.utils.ReportingUtils;

/**
 *
 * @author Wayne
 */
public class Utility {
    public static void LogMsg( String s )
    {
        PrintWriter out = null;
        try{
            out = new PrintWriter(new BufferedWriter(new FileWriter("log.txt", true)));
            out.println(s);
        }catch (IOException e) {
        }
        finally{
            if(out!=null){
                out.close();
            }
        }
    }

    public static void LogException( Exception ex )
    {
        PrintWriter out = null;
        try{
            out = new PrintWriter(new BufferedWriter(new FileWriter("log.txt", true)));
            ex.printStackTrace( out );
        }catch (IOException e) {
        }
        finally{
            if(out!=null){
                out.close();
            }
        }
    }
    public static void Save(ImageProcessor proc, String fn)
    {
        try {
            ImageIO.write(proc.getBufferedImage(), "bmp", new File(fn));
        } catch (IOException e) {
            ReportingUtils.logError(e);
        }
    }
    public static void Save(BufferedImage bi, String fn)
    {
        try {
            ImageIO.write(bi, "bmp", new File(fn));
        } catch (IOException e) {
            ReportingUtils.logError(e);
        }
    }
    public static void DrawCross(BufferedImage bi, Point pt)
    {
        final int max_r = 50, min_r = 10, color = 0xffffff;
        for( int x = pt.x - max_r; x < pt.x - min_r; x++ ){
            if( x >= 0 && x < bi.getWidth()) 
                bi.setRGB( x, pt.y, color );
        }
        for( int x = pt.x + min_r; x < pt.x + max_r; x++ ){
            if( x >= 0 && x < bi.getWidth()) 
                bi.setRGB( x, pt.y, color );
        }
        for( int y = pt.y - max_r; y < pt.y - min_r; y++ ){
            if( y >=0 && y < bi.getHeight() )
                bi.setRGB(pt.x, y, color);
        }
        for( int y = pt.y + min_r; y < pt.y + max_r; y++ ){
            if( y >=0 && y < bi.getHeight() )
                bi.setRGB(pt.x, y, color);
        }
    }
    public static long CalculateVariance(short[] data)
    {
        long sum = 0, sum2 = 0;
        for( int i = 0; i < data.length; i++){
            sum += data[i];
            sum2 += data[i] * data[i];
        }
        return ( sum2 - sum * sum / data.length ) / data.length;
    }
    public static float CalculateVariance(float[] data)
    {
        float sum = 0, sum2 = 0;
        for( int i = 0; i < data.length; i++){
            sum += data[i];
            sum2 += data[i] * data[i];
        }
        return ( sum2 - sum * sum / data.length ) / data.length;
    }

    public static void DeleteFolder(File folder) {
        File[] files = folder.listFiles();
        if( files != null ) { 
            for( File f: files ) {
                if( f.isDirectory() ) {
                    DeleteFolder( f );
                } else {
                    f.delete();
                }
            }
        }
        folder.delete();
    }
    
    public static BufferedImage DuplicateBufferedImage(BufferedImage bi) 
    {
        ColorModel cm = bi.getColorModel();
        boolean isAlphaPremultiplied = cm.isAlphaPremultiplied();
        WritableRaster raster = bi.copyData(null);
        return new BufferedImage(cm, raster, isAlphaPremultiplied, null);
    }    
}
