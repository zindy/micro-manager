/*
 * Adapter for the Spectral/Andor/Oxford Instruments Diskovery 1 spinning disk confocal
 * microscope system
 *
 * AUTHOR:
 * Nico Stuurman
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

#include "Diskovery.h"

/**
 * Class that takes commands out of a BlockingQueue and sends them to 
 * the Diskovery controller (but only when it is not busy)
 */
MessageSender::MessageSender(
      MM::Device& device, 
      MM::Core& core, 
      std::string serialPort, 
      BlockingQueue<std::string>& blockingQueue,
      DiskoveryModel* model) :
   stop_(false),
   device_(device),
   core_(core),
   port_(serialPort),
   blockingQueue_(blockingQueue),
   model_(model)
{
}

MessageSender::~MessageSender()
{
   Shutdown();
}

void MessageSender::Shutdown()
{
   Stop();
   // send the magic command to finish off the sender thread
   blockingQueue_.push("END");
   wait();
}


int MessageSender::svc()
{
   while (!stop_) 
   {
      std::string command;
      blockingQueue_.wait_and_pop(command);
      if (command == "END")
         return 0;
      model_->WaitForDeviceBusy();
      core_.SetSerialCommand(&device_, port_.c_str(), command.c_str(), "\n");
      model_->SetDeviceBusy(true);
   }
   return 0;
}

void MessageSender::Start()
{
   stop_ = false;
   activate();
}

