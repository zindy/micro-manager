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

package misc;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;


public class LicenseWriter {
   
   private static final String MAGELLAN_HEADER = "///////////////////////////////////////////////////////////////////////////////\r\n" +
"// AUTHOR:       Henry Pinkard, henry.pinkard@gmail.com\r\n" +
"//\r\n" +
"// COPYRIGHT:    University of California, San Francisco, 2015\r\n" +
"//\n" +
"// LICENSE:      This file is distributed under the BSD license.\r\n" +
"//               License text is included with the source distribution.\r\n" +
"//\r\n" +
"//               This file is distributed in the hope that it will be useful,\r\n" +
"//               but WITHOUT ANY WARRANTY; without even the implied warranty\r\n" +
"//               of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\r\n" +
"//\r\n" +
"//               IN NO EVENT SHALL THE COPYRIGHT OWNER OR\r\n" +
"//               CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,\r\n" +
"//               INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES.\r\n" +
"//\r\n";
   
   public static void main(String[] args) throws FileNotFoundException, IOException {
      String path = "/Users/henrypinkard/Documents/NetbeansSources/MM/trunk/plugins/Magellan/";
      for (File dir : new File(path).listFiles()) {
         if (dir.isDirectory() && !dir.getName().equals("json")) {
            for (File f : dir.listFiles()) {
               System.out.println("File: " + f.getName());
               if (f.getName().endsWith(".java")) {
                  String fileString = fileToString(f);
                  fileString = removeNetbeansDefault(fileString);
                  if ( !fileString.startsWith("/")) {
                     System.out.println("Overwriting");
                     String withLic = MAGELLAN_HEADER + fileString;
                     FileWriter fw = new FileWriter(f, false);
                     fw.write(withLic);
                     fw.flush();
                     fw.close();
                  }     
               }
            }
         }
      }
      
   }
   
   //null if it doesn't match, chop off if it does
   public static String removeNetbeansDefault(String s) {
      //13 is \ r
      String h1 = "/*" + "\r\n" +
              " * To change this license header, choose License Headers in Project Properties." + "\r\n" +
              " * To change this template file, choose Tools | Templates"+ "\r\n" +
             " * and open the template in the editor." + "\r\n" +
              "*/";
      String h2 = "/*" + "\r\n" +
              " * To change this template, choose Tools | Templates" +  "\r\n" +
              " * and open the template in the editor." + "\r\n" +
              " */";
          
      if (  s.startsWith(h1)   ) {
        return s.substring(h1.length());
      } else if (s.startsWith(h2)) {
         return s.substring(h2.length());
      }
      return s;
   }
   
   public static String fileToString(File f) throws FileNotFoundException  {
    String s = "";
      Scanner scanner = new Scanner(f);
      scanner.useDelimiter("\r\n");
    while(scanner.hasNext()) {
       s += scanner.next() + "\r\n";
    }
    scanner.close();
    return s;
}
   
}
