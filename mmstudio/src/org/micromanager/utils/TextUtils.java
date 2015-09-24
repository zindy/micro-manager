/*
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, October 1, 2006
 *
 * Copyright (c) 2006 Regents of the University of California
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

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.text.DecimalFormat;

public class TextUtils {
   
   static public String readTextFile(String path) throws IOException {
      String newLine = System.getProperty("line.separator");
      StringBuffer sb = new StringBuffer();
      BufferedReader input = new BufferedReader(new FileReader(path));
      String line;
      while (null != (line = input.readLine())) {
         if (sb.length() > 0)
            sb.append(newLine);
         sb.append(line);
      }
      
      return sb.toString();
   }
   
   static public void writeTextFile(String path, String content) throws IOException {
      BufferedWriter output= new BufferedWriter(new FileWriter(path));
      output.append(content);
      output.close();
   }
   
   public static final DecimalFormat FMT0 = new DecimalFormat("#0");
   public static final DecimalFormat FMT1 = new DecimalFormat("#0.0");
   public static final DecimalFormat FMT2 = new DecimalFormat("#0.00");
   public static final DecimalFormat FMT3 = new DecimalFormat("#0.000");

}
