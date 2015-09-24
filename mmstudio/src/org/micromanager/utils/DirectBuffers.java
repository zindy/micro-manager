/*
 * Copyright (c) 2005-2015 Regents of the University of California
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


package org.micromanager.utils;

import java.io.UnsupportedEncodingException;
import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;
import java.nio.ShortBuffer;

/**
 *
 * @author Arthur
 */
public class DirectBuffers {
   
   public static ByteOrder NATIVE_ORDER = ByteOrder.nativeOrder();
   
   public static ByteBuffer bufferFromBytes(byte[] bytes) {
      return ByteBuffer.allocateDirect(bytes.length).put(bytes);
   }
   
   public static ShortBuffer bufferFromShorts(short[] shorts) {
      return ByteBuffer.allocateDirect(2*shorts.length).order(NATIVE_ORDER).asShortBuffer().put(shorts);
   }
   
   public static IntBuffer bufferFromInts(int[] ints) {
      return ByteBuffer.allocateDirect(4*ints.length).order(NATIVE_ORDER).asIntBuffer().put(ints);
   }
      
   public static byte[] bytesFromBuffer(ByteBuffer buffer) {
      synchronized(buffer) {
         byte[] bytes = new byte[buffer.capacity()];
         buffer.rewind();
         buffer.get(bytes);
         return bytes;
      }
   }
   
   public static short[] shortsFromBuffer(ShortBuffer buffer) {
      synchronized(buffer) {
         short[] shorts = new short[buffer.capacity()];
         buffer.rewind();
         buffer.get(shorts);
         return shorts;
      }
   }

   public static int[] intsFromBuffer(IntBuffer buffer) {
      synchronized(buffer) {
         int[] ints = new int[buffer.capacity()];
         buffer.rewind();
         buffer.get(ints);
         return ints;
      }
   }

   public static Object arrayFromBuffer(Buffer buffer) {
      synchronized(buffer) {
         if (buffer instanceof ByteBuffer) {
            return bytesFromBuffer((ByteBuffer) buffer);
         } else if (buffer instanceof ShortBuffer) {
            return shortsFromBuffer((ShortBuffer) buffer);
         } else if (buffer instanceof IntBuffer) {
            return intsFromBuffer((IntBuffer) buffer);
         }
      }
      return null;
   }
   
   public static Buffer bufferFromArray(Object primitiveArray) {
      if (primitiveArray instanceof byte[]) {
         return bufferFromBytes((byte []) primitiveArray);
      } else if (primitiveArray instanceof short[]) {
         return bufferFromShorts((short []) primitiveArray);
      } else if (primitiveArray instanceof int[]) {
         return bufferFromInts((int []) primitiveArray);
      }
      return null;
   }
   
   public static ByteBuffer bufferFromString(String string) {
      try {
         return bufferFromBytes(string.getBytes("UTF-8"));
      } catch (UnsupportedEncodingException ex) {
         ReportingUtils.logError(ex);
         return null;
      }
   }

   public static String stringFromBuffer(ByteBuffer byteBuffer) {
      try {
         return new String(bytesFromBuffer(byteBuffer), "UTF-8");
      } catch (UnsupportedEncodingException ex) {
         ReportingUtils.logError(ex);
         return null;
      }
   }
   
}
