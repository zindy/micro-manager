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

#ifndef ANDORSDK3STRINGS_H_
#define ANDORSDK3STRINGS_H_

#include <string>

class TAndorSDK3Strings
{
public:
   static const std::string ACQUISITION_AOI;
   static const std::string PIXEL_READOUT_RATE;
   static const std::string ELECTRONIC_SHUTTERING_MODE;
   static const std::string TEMPERATURE_CONTROL;
   static const std::string PIXEL_ENCODING;
   static const std::string ACCUMULATE_COUNT;
   static const std::string TEMPERATURE_STATUS;
   static const std::string FAN_SPEED;
   static const std::string SENSOR_TEMPERATURE;
   static const std::string SPURIOUS_NOISE_FILTER;
   static const std::string STATIC_BLEMISH_CORRECTION;
   static const std::string SENSOR_COOLING;
   static const std::string OVERLAP;
   static const std::string FRAME_RATE;
   static const std::string FRAME_RATE_LIMITS;
   static const std::string TRIGGER_MODE;
   static const std::string GAIN_TEXT;
   static const std::string GLOBAL_CLEAR;
   static const std::string AUX_SOURCE;
   static const std::string AUX_SOURCE_TWO;
};


const std::string TAndorSDK3Strings::ACQUISITION_AOI("AcquisitionWindow");
const std::string TAndorSDK3Strings::PIXEL_READOUT_RATE("PixelReadoutRate");
const std::string TAndorSDK3Strings::ELECTRONIC_SHUTTERING_MODE("ElectronicShutteringMode");
const std::string TAndorSDK3Strings::TEMPERATURE_CONTROL("TemperatureControl");
const std::string TAndorSDK3Strings::PIXEL_ENCODING("PixelEncoding");
const std::string TAndorSDK3Strings::ACCUMULATE_COUNT("AccumulateCount");
const std::string TAndorSDK3Strings::TEMPERATURE_STATUS("TemperatureStatus");
const std::string TAndorSDK3Strings::FAN_SPEED("FanSpeed");
const std::string TAndorSDK3Strings::SENSOR_TEMPERATURE("SensorTemperature");
const std::string TAndorSDK3Strings::SPURIOUS_NOISE_FILTER("SpuriousNoiseFilter");
const std::string TAndorSDK3Strings::STATIC_BLEMISH_CORRECTION("StaticBlemishCorrection");
const std::string TAndorSDK3Strings::SENSOR_COOLING("SensorCooling");
const std::string TAndorSDK3Strings::OVERLAP("Overlap");
const std::string TAndorSDK3Strings::FRAME_RATE("FrameRate");
const std::string TAndorSDK3Strings::FRAME_RATE_LIMITS("FrameRateLimits");
const std::string TAndorSDK3Strings::TRIGGER_MODE("TriggerMode");
const std::string TAndorSDK3Strings::GAIN_TEXT("Sensitivity/DynamicRange");
const std::string TAndorSDK3Strings::GLOBAL_CLEAR("RollingShutterGlobalClear");
const std::string TAndorSDK3Strings::AUX_SOURCE("AuxiliaryOutSource (TTL I/O)");
const std::string TAndorSDK3Strings::AUX_SOURCE_TWO("AuxiliaryOutTwoSource (TTL I/O)");

#endif //include only once

