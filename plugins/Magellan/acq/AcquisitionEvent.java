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

package acq;

import coordinates.XYStagePosition;
import java.util.List;
import propsandcovariants.CovariantPairing;
import propsandcovariants.CovariantValue;

/**
 * Information about the acquisition of a single image
 */
public class AcquisitionEvent  {

   enum SpecialFlag {TimepointFinished, AcqusitionFinished, SwappingQueues, EngineTaskFinished, AutofocusAdjustment};
   
   final public Acquisition acquisition_;
   final public int timeIndex_, sliceIndex_, channelIndex_, positionIndex_;
   final public double zPosition_;
   final public XYStagePosition xyPosition_;
   private SpecialFlag specialFlag_;
   final public List<CovariantPairing> covariants_;
   public String autofocusZName_;
   public double autofocusPosition_;
   
   
   public AcquisitionEvent(Acquisition acq, int frameIndex, int channelIndex, int sliceIndex, int positionIndex, 
            double zPos, XYStagePosition xyPos, List<CovariantPairing> covariants) {
      timeIndex_ = frameIndex;
      sliceIndex_ = sliceIndex;
      channelIndex_ = channelIndex;
      positionIndex_ = positionIndex;    
      zPosition_ = zPos;
      acquisition_ = acq;
      xyPosition_ = xyPos;
      covariants_ = covariants;
   }
   
   public static AcquisitionEvent createAutofocusEvent(String zName, double pos ) {   
      AcquisitionEvent evt = new AcquisitionEvent(null, 0, 0, 0, 0, 0, null, null);
      evt.autofocusZName_ = zName;
      evt.autofocusPosition_ = pos;
      evt.specialFlag_ = SpecialFlag.AutofocusAdjustment;
      return evt;
   }
   
    public boolean isAutofocusAdjustmentEvent() {
      return specialFlag_ == SpecialFlag.AutofocusAdjustment;
   }
   
   public static AcquisitionEvent createEngineTaskFinishedEvent() {
      AcquisitionEvent evt = new AcquisitionEvent(null, 0, 0,  0, 0, 0, null, null);
      evt.specialFlag_ = SpecialFlag.EngineTaskFinished;
      return evt;
   }
   
   public boolean isEngineTaskFinishedEvent() {
      return specialFlag_ == SpecialFlag.EngineTaskFinished;
   }
   
   public static AcquisitionEvent createTimepointFinishedEvent(Acquisition acq) {
      AcquisitionEvent evt = new AcquisitionEvent(acq, 0, 0,  0, 0, 0, null, null);
      evt.specialFlag_ = SpecialFlag.TimepointFinished;
      return evt;
   }
   
   public boolean isTimepointFinishedEvent() {
      return specialFlag_ == SpecialFlag.TimepointFinished;
   }
   
   public static AcquisitionEvent createReQuerieEventQueueEvent() {
      AcquisitionEvent evt = new AcquisitionEvent(null, 0, 0, 0, 0, 0, null, null);
      evt.specialFlag_ = SpecialFlag.SwappingQueues;
      return evt;
   }
   
   public boolean isReQueryEvent() {
      return specialFlag_ == SpecialFlag.SwappingQueues;
   }
   
   public static AcquisitionEvent createAcquisitionFinishedEvent(Acquisition acq) {
      AcquisitionEvent evt = new AcquisitionEvent(acq, 0, 0, 0, 0, 0, null, null);
      evt.specialFlag_ = SpecialFlag.AcqusitionFinished;
      return evt;
   }
   
   public boolean isAcquisitionFinishedEvent() {
      return specialFlag_ == SpecialFlag.AcqusitionFinished;
   }
   
   @Override
   public String toString() {
      if (specialFlag_ == SpecialFlag.AcqusitionFinished) {
          return "Acq finished event";
      } else if (specialFlag_ == SpecialFlag.AutofocusAdjustment) {       
          return "Autofocus event";
      } else if (specialFlag_ == SpecialFlag.EngineTaskFinished) {       
          return "Engine task finished event";
      } else if (specialFlag_ == SpecialFlag.SwappingQueues) {
          return "Swapping queues event";
      } else if (specialFlag_ == SpecialFlag.TimepointFinished) {
          return "Timepoint finished event";
      }
       
       return "P: " + positionIndex_ + "\t\tT: " + timeIndex_ + "\t\tZ: " + sliceIndex_ + "\t\tC: " + channelIndex_; 
   }
}
