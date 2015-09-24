/*
 * AUTHOR:
 * Mark Tsuchida (based in part on previous version by Karl Hoover)
 *
 * Copyright (c) 2010-2014 Regents of the University of California
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

// TODO Perhaps this should live in a package that defines the
// micro-manager.org server, together with other things that interact with the
// server.
package org.micromanager.diagnostics;

/**
 * Upload a textual problem report.
 *
 * This class handles the simple task of uploading, given the report text. It
 * does not and should not depend on the contents of the report or its text.
 */
public class ReportSender {
   private static java.net.URL problemReportUploadURL_;
   static {
      try {
         problemReportUploadURL_ = new java.net.URL("http://valelab.ucsf.edu/~MM/upload_corelog.php");
      }
      catch (java.net.MalformedURLException impossible) {
         System.exit(1);
      }
   }

   public static java.net.URL getProblemReportUploadURL() {
      return problemReportUploadURL_;
   }

   // TODO The uuencoding ought to belong to a generic upload-to-mm-server
   // framework, not in each place that performs a file upload.
   public void sendReport(String report, String fileName, java.net.URL uploadURL)
      throws java.io.IOException, java.io.FileNotFoundException, Exception {

      // TODO Eliminate temp file (and all the possible errors associated with its creation).
      final java.io.File tempDir = new java.io.File(System.getProperty("java.io.tmpdir"));
      final java.io.File tempFile = new java.io.File(tempDir, fileName);
      tempFile.deleteOnExit();

      try {
         saveReportToUUEncodedGZIPFile(report, tempFile);
         new org.micromanager.utils.HttpUtils().upload(uploadURL, tempFile);
      }
      finally {
         tempFile.delete();
      }
   }

   private void saveReportToUUEncodedGZIPFile(String report, java.io.File file)
      throws java.io.IOException, java.io.FileNotFoundException {

      final java.io.InputStream gzipInputFromPipe = getGZIPInputFromString(report);
      final java.io.OutputStream uuencodedOutputToFile;
      uuencodedOutputToFile = new java.io.FileOutputStream(file);

      try {
         // Surely "encodeBuffer" is a misnomer. It encodes streams.
         new org.micromanager.utils.MMUUEncoder().encodeBuffer(gzipInputFromPipe, uuencodedOutputToFile);
      }
      finally {
         try {
            uuencodedOutputToFile.close();
            gzipInputFromPipe.close();
         }
         catch (java.io.IOException ignore) {
         }
      }
   }

   private java.io.InputStream getGZIPInputFromString(final String str) throws java.io.IOException {
      final java.io.PipedOutputStream gzipOutputToPipe = new java.io.PipedOutputStream();
      final java.io.InputStream gzipInputFromPipe = new java.io.PipedInputStream(gzipOutputToPipe);

      final java.io.OutputStream outputToGZIP = new java.util.zip.GZIPOutputStream(gzipOutputToPipe);
      final java.io.Writer writerToGZIP = new java.io.OutputStreamWriter(outputToGZIP, "UTF-8");

      new Thread() {
         @Override
         public void run() {
            try {
               writerToGZIP.write(str);
               writerToGZIP.close();
            }
            catch (java.io.IOException ignore) {
               // Let's assume that piped (in-memory) streams will not encounter I/O errors
            }
         }
      }.start();

      return gzipInputFromPipe;
   }
}
