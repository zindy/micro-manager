/*
 * AUTHOR:
 * Nico Stuurman, nico.stuurman@ucsf.edu, March 9, 2013
 *
 * Copyright (c) 2013 Regents of the University of California
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

import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DropTargetDragEvent;
import java.awt.dnd.DropTargetDropEvent;
import java.awt.dnd.DropTargetEvent;
import java.awt.dnd.DropTargetListener;
import java.io.File;
import java.io.IOException;
import org.micromanager.MMStudio;

/**
 * DragDropUtil
 * Handler for drop events in Micro-Manager
 * Checks if files or folders are dropped onto Micro-Manager, and 
 * tries to open them.
 * 
 * @author nico
 * 
 */
public class DragDropUtil implements DropTargetListener {

   @Override
   public void dragEnter(DropTargetDragEvent dtde) {
      //throw new UnsupportedOperationException("Not supported yet.");
   }

   @Override
   public void dragOver(DropTargetDragEvent dtde) {
      //throw new UnsupportedOperationException("Not supported yet.");
   }

   @Override
   public void dropActionChanged(DropTargetDragEvent dtde) {
      //throw new UnsupportedOperationException("Not supported yet.");
   }

   @Override
   public void dragExit(DropTargetEvent dte) {
      //throw new UnsupportedOperationException("Not supported yet.");
   }

   /**
    * This function does the actual work
    */
   @Override
   public void drop(final DropTargetDropEvent dtde) {

      try {
         Transferable tr = dtde.getTransferable();
         DataFlavor[] flavors = tr.getTransferDataFlavors();
         for (int i = 0; i < flavors.length; i++) {

            if (flavors[i].isFlavorJavaFileListType()) {

               dtde.acceptDrop(DnDConstants.ACTION_COPY_OR_MOVE);

               java.util.List list = (java.util.List) tr.getTransferData(flavors[i]);
               for (int j = 0; j < list.size(); j++) {
                  File f = (File) list.get(j);
                  String dirtmp = f.getPath();
                  if (f.isFile()) {
                     dirtmp = f.getParent();
                  }
                  final String dir = dirtmp;

                  // to not block the UI of the OS, open in a separate thread          
                  new Thread() {
                     @Override
                     public void run() {
                        try {
                           MMStudio.getInstance().openAcquisitionData(dir, true);
                        } catch (MMScriptException ex) {
                           ReportingUtils.showError(ex);
                        }
                     }
                  }.start();

               }
               dtde.dropComplete(true);
               return;
            }
         }
      } catch (UnsupportedFlavorException ex) {
      } catch (IOException ex) {
      } 

   }
}
