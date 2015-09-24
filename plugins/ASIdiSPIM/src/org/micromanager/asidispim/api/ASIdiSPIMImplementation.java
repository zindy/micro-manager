/*
 * AUTHOR:
 * Nico Stuurman, Jon Daniels
 *
 * Copyright (c) 2014 Regents of the University of California, & ASI
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

package org.micromanager.asidispim.api;

import org.micromanager.asidispim.ASIdiSPIM;
import org.micromanager.asidispim.ASIdiSPIMFrame;
import org.micromanager.asidispim.AcquisitionPanel;

/**
 * Implementation of the ASidiSPIMInterface
 * To avoid depending on the internals of this class and restrict yourself
 * to the ASIdiSPIMInterface, always cast the instance of this class to ASIdiSPIMInterface
 * e.g.: 
 * 
 * ASIdiSPIMInterface diSPIM = new ASIdiSPIMImplementation();
 * 
 * @author nico
 * @author Jon
 */
public class ASIdiSPIMImplementation implements ASIdiSPIMInterface {

   @Override
   public void runAcquisition() throws ASIdiSPIMException {
      getAcquisitionPanel().runAcquisition();
   }
   
   @Override
   public void setAcquisitionNamePrefix(String acqName) throws ASIdiSPIMException {
      getAcquisitionPanel().setAcquisitionNamePrefix(acqName);
   }
   
   //** Private methods.  Only for internal use **//
   
   private ASIdiSPIMFrame getFrame() throws ASIdiSPIMException {
      ASIdiSPIMFrame frame = ASIdiSPIM.getFrame();
      if (frame == null) {
         throw new ASIdiSPIMException ("Plugin is not open");
      }
      return frame;
   }
   
   private AcquisitionPanel getAcquisitionPanel() throws ASIdiSPIMException {
      AcquisitionPanel acquisitionPanel = getFrame().getAcquisitionPanel();
      if (acquisitionPanel == null) {
         throw new ASIdiSPIMException ("AcquisitionPanel is not open");
      }
      return acquisitionPanel;
   }

}
