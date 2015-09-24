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

import org.junit.Test;
import static org.junit.Assert.*;

public class ImageUtilsTest {
   @Test
   public void unsignedByteValueIsCorrect() {
      assertEquals(0x00, ImageUtils.unsignedValue((byte) 0x00));
      assertEquals(0x01, ImageUtils.unsignedValue((byte) 0x01));
      assertEquals(0xff, ImageUtils.unsignedValue((byte) 0xff));
      assertEquals(0xfe, ImageUtils.unsignedValue((byte) 0xfe));
      assertEquals(0x7f, ImageUtils.unsignedValue((byte) 0x7f));
      assertEquals(0x80, ImageUtils.unsignedValue((byte) 0x80));
   }

   @Test
   public void unsignedShortValueIsCorrect() {
      assertEquals(0x0000, ImageUtils.unsignedValue((short) 0x0000));
      assertEquals(0x0001, ImageUtils.unsignedValue((short) 0x0001));
      assertEquals(0xffff, ImageUtils.unsignedValue((short) 0xffff));
      assertEquals(0xfffe, ImageUtils.unsignedValue((short) 0xfffe));
      assertEquals(0x7fff, ImageUtils.unsignedValue((short) 0x7fff));
      assertEquals(0x8000, ImageUtils.unsignedValue((short) 0x8000));
   }
}
