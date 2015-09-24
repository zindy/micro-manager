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

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 *
 * @author Johannes Schindelin
 */
public class MMUUEncoder {
        protected int maxBytesPerLine = 45;

        public void encodeBuffer(InputStream in, OutputStream out) throws IOException {
                out.write("begin 664 raw.bin\n".getBytes());
                byte[] buffer = new byte[maxBytesPerLine * 4 / 3];
                int counter = 0;
                for (;;) {
                        if (counter == maxBytesPerLine * 4 / 3) {
                                writeWithLength(out, buffer, counter);
                                counter = 0;
                        }

                        int value1 = in.read();
                        if (value1 < 0)
                                break;
                        int value2 = in.read(), value3 = in.read();
                        int packed = ((value1 & 0xff) << 16) |
                                ((value2 & 0xff) << 8) |
                                (value3 & 0xff);
                        buffer[counter++] = encode((packed >> 18) & 0x3f);
                        buffer[counter++] = encode((packed >> 12) & 0x3f);
                        if (value2 < 0)
                                break;
                        buffer[counter++] = encode((packed >> 6) & 0x3f);
                        if (value3 < 0)
                                break;
                        buffer[counter++] = encode(packed & 0x3f);
                }
                if (counter > 0)
                        writeWithLength(out, buffer, counter);
                out.write("`\nend\n".getBytes());
        }

        public static void writeWithLength(OutputStream out, byte[] buffer, int counter) throws IOException {
                out.write(encode(counter * 3 / 4));
                out.write(buffer, 0, counter);
                out.write('\n');
        }

        public static byte encode(int value) {
                return (byte)(value == 0 ? 96 : 32 + value);
        }

}
