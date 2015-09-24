/*
 * SLM device instance wrapper
 *
 * AUTHOR:
 * Mark Tsuchida
 *
 * Copyright (c) 2014 Regents of the University of California,
 * All Rights reserved
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SLMInstance.h"


int SLMInstance::SetImage(unsigned char* pixels) { return GetImpl()->SetImage(pixels); }
int SLMInstance::SetImage(unsigned int* pixels) { return GetImpl()->SetImage(pixels); }
int SLMInstance::DisplayImage() { return GetImpl()->DisplayImage(); }
int SLMInstance::SetPixelsTo(unsigned char intensity) { return GetImpl()->SetPixelsTo(intensity); }
int SLMInstance::SetPixelsTo(unsigned char red, unsigned char green, unsigned char blue) { return GetImpl()->SetPixelsTo(red, green, blue); }
int SLMInstance::SetExposure(double interval_ms) { return GetImpl()->SetExposure(interval_ms); }
double SLMInstance::GetExposure() { return GetImpl()->GetExposure(); }
unsigned SLMInstance::GetWidth() { return GetImpl()->GetWidth(); }
unsigned SLMInstance::GetHeight() { return GetImpl()->GetHeight(); }
unsigned SLMInstance::GetNumberOfComponents() { return GetImpl()->GetNumberOfComponents(); }
unsigned SLMInstance::GetBytesPerPixel() { return GetImpl()->GetBytesPerPixel(); }
int SLMInstance::IsSLMSequenceable(bool& isSequenceable)
{ return GetImpl()->IsSLMSequenceable(isSequenceable); }
int SLMInstance::GetSLMSequenceMaxLength(long& nrEvents)
{ return GetImpl()->GetSLMSequenceMaxLength(nrEvents); }
int SLMInstance::StartSLMSequence() { return GetImpl()->StartSLMSequence(); }
int SLMInstance::StopSLMSequence() { return GetImpl()->StopSLMSequence(); }
int SLMInstance::ClearSLMSequence() { return GetImpl()->ClearSLMSequence(); }
int SLMInstance::AddToSLMSequence(const unsigned char * pixels)
{ return GetImpl()->AddToSLMSequence(pixels); }
int SLMInstance::AddToSLMSequence(const unsigned int * pixels)
{ return GetImpl()->AddToSLMSequence(pixels); }
int SLMInstance::SendSLMSequence() { return GetImpl()->SendSLMSequence(); }
