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

#ifndef _AOIPROPERTY_H_
#define _AOIPROPERTY_H_

#include <map>
#include <vector>
#include "MMDeviceConstants.h"
#include "Property.h"
#include "atcore.h"

class ICallBackManager;

namespace andor
{
   class ISubject;
   class IInteger;
}

class TAOIProperty
{
public:
   TAOIProperty(const std::string & MM_name, ICallBackManager* callback, bool readOnly);
   ~TAOIProperty();

   void Update(andor::ISubject* Subject);
   int OnAOI(MM::PropertyBase* pProp, MM::ActionType eAct);
   typedef MM::Action<TAOIProperty> CPropertyAction;

   AT_64 GetWidth();
   AT_64 GetHeight();
   AT_64 GetLeftOffset();
   AT_64 GetTopOffset();
   unsigned GetBytesPerPixel();
   AT_64 GetStride();
   double GetBytesPerPixelF();
   void SetReadOnly(bool set_to);
   const char* SetCustomAOISize(unsigned left, unsigned top, unsigned width, unsigned height);
   const char* ResetToFullImage();

private:
   typedef std::map<long long, int> TMapAOIIndexType;
   typedef std::map<long long, long long> TMapAOIWidthHeightType;
   typedef std::map<std::string, int> TMapAOIGUIListType;
   typedef std::vector<long long> TVectorXYType;

   void populateWidthMaps(bool fullAoiControl);
   void populateLeftTopVectors();
   void populateAOIGUIList(TMapAOIIndexType::iterator iterIndex, TMapAOIWidthHeightType::iterator iter, AT_64 i64_sensorWidth);
   void findBestR2AOICoords(TMapAOIIndexType::iterator iter, AT_64 i64_sensorWidth, AT_64 i64_sensorHeight);
   void setFeature(long data);

   ICallBackManager* callback_;
   andor::IInteger* aoi_height_;
   andor::IInteger* aoi_width_;
   andor::IInteger* aoi_top_;
   andor::IInteger* aoi_left_;
   andor::IInteger* aoi_stride_;
   MM::Property* pbProp_;
   std::string customStr_;

   TMapAOIIndexType aoiWidthIndexMap_;
   TMapAOIWidthHeightType aoiWidthHeightMap_;
   TMapAOIGUIListType aoiGUIList_;
   TVectorXYType leftX_;
   TVectorXYType topY_;
   bool fullAoiControl_;
};

#endif // _AOIPROPERTY_H_
