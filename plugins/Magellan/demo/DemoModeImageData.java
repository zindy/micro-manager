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

package demo;

import ij.IJ;
import ij.ImagePlus;
import ij.WindowManager;
import java.awt.Frame;
import main.Magellan;
import misc.JavaUtils;

/**
 *
 * @author Henry
 */
public class DemoModeImageData {
      
   private static ImagePlus img_;
   
   public DemoModeImageData() {
//      Interpreter.batchMode = true; //batch mode makes everything ridiculously slow for some reason
      
      String name = "Navigator demo LN" + 
              (Magellan.getCore().getBytesPerPixel() > 1 ? "16Bit" : "")+ ".tif";
      if (JavaUtils.isMac()) {
         //Laptop         
         IJ.runMacro("run(\"TIFF Virtual Stack...\", \"open=[/Applications/Micro-Manager1.4/Navigator demo LN.tif]\");");
      } else {
         //BIDC computer
         IJ.runMacro("run(\"TIFF Virtual Stack...\", \"open=[C:/Program Files/Micro-Manager-1.4/" + name+ "]\");");
      }
      img_ = WindowManager.getImage(name);
      img_.getWindow().setState(Frame.ICONIFIED);
   }
   
    //this demo data has z spaceing of 3, from 0-399
   //byte data in a short container
   public static short[] getShortPixelData(int channel, int x, int y, int z, int width, int height) {
      int fullWidth = img_.getWidth();
      int fullHeight = img_.getHeight();      
      while (x < 0) x+= fullWidth;
      while (y < 0) y+= fullHeight;
      while (z < 0) z+= 399;
      x = x % fullWidth;
      y = y % fullHeight;
      int sliceIndex = (z % 399) / 3;
      short[] fullSlice = (short[]) img_.getStack().getPixels(6 * sliceIndex + channel + 1);
      short[] pixels = new short[width * height];
      for (int line = 0; line < height; line++) {
         try {
            if (y + line >= fullHeight) {
               y -= fullHeight; //reset to top if go over
            }
            System.arraycopy(fullSlice, (y + line) * fullWidth + x, pixels, line * width, Math.min(width, fullWidth - x));
            //Copy rest of line if spill over in x
            System.arraycopy(fullSlice, (y + line) * fullWidth, pixels, line * width + Math.min(width, fullWidth - x), width - Math.min(width, fullWidth - x));
         } catch (ArrayIndexOutOfBoundsException e) {
            e.printStackTrace();
         }
      }
      return pixels;
   }
   
   //this demo data has z spaceing of 3, from 0-399
   public static byte[] getBytePixelData(int channel, int x, int y, int z, int width, int height) {
      int fullWidth = img_.getWidth();
      int fullHeight = img_.getHeight();      
      while (x < 0) x+= fullWidth;
      while (y < 0) y+= fullHeight;
      while (z < 0) z+= 399;
      x = x % fullWidth;
      y = y % fullHeight;
      int sliceIndex = (z % 399) / 3;
      byte[] fullSlice = (byte[]) img_.getStack().getPixels(6 * sliceIndex + channel + 1);
      byte[] pixels = new byte[width * height];
      for (int line = 0; line < height; line++) {
         try {
            if (y + line >= fullHeight) {
               y -= fullHeight; //reset to top if go over
            }
            System.arraycopy(fullSlice, (y + line) * fullWidth + x, pixels, line * width, Math.min(width, fullWidth - x));
            //Copy rest of line if spill over in x
            System.arraycopy(fullSlice, (y + line) * fullWidth, pixels, line * width + Math.min(width, fullWidth - x), width - Math.min(width, fullWidth - x));
         } catch (ArrayIndexOutOfBoundsException e) {
            e.printStackTrace();
         }
      }
      return pixels;
   }
  
   
}
