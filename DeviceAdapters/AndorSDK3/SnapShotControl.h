/*
 * Copyright (c) 2007, Regents of the University of California
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

#ifndef _SNAPSHOTCONTROL_H_
#define _SNAPSHOTCONTROL_H_

namespace andor {
class IDevice;
class IEnum;
class ICommand;
class IBufferControl;
}
class CEventsManager;

class SnapShotControl
{
public:
   SnapShotControl(andor::IDevice* cameraDevice, CEventsManager* _evMngr);
   ~SnapShotControl();

   static const unsigned int WAIT_DATA_TIMEOUT_BUFFER_MILLISECONDS = 500;

   void setupTriggerModeSilently();
   void poiseForSnapShot();
   void leavePoisedMode();
   bool takeSnapShot();
   void getData(unsigned char*& image_buffers);
   void resetCameraAcquiring();

   bool isPoised(){return is_poised_;};
   bool isInternal() {return set_internal_;}
   bool isExternal() {return in_external_;}
   bool isSoftware() {return in_software_;}
   bool isMono12Packed() {return mono12PackedMode_; };

private:
   int  getReadoutTime();
   int  getTransferTime();
   int  retrieveCurrentExposureTime();
   bool isGlobalShutter();
   void resetTriggerMode();

private:
   static const unsigned int EXT_TRIG_TIMEOUT_MILLISECONDS = 10000;
   static const int INVALID_EXP_TIME = -1;

   andor::IDevice* cameraDevice;
   andor::IEnum* triggerMode;
   andor::ICommand* startAcquisitionCommand;
   andor::ICommand* stopAcquisitionCommand;
   andor::IBufferControl* bufferControl;
   andor::ICommand* sendSoftwareTrigger;

   CEventsManager* eventsManager_;

   unsigned char* image_buffer_;

   bool is_poised_;
   bool set_internal_;
   bool in_software_;
   bool in_external_;
   bool mono12PackedMode_;
};

#endif /* _SNAPSHOTCONTROL_H_ */
