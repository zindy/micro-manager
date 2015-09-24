/*
 * Interface for the Spectral LMM5
 *
 * AUTHOR:
 * Francis McCloy
 *
 * Copyright (c) 2009 Regents of the University of California
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

#ifndef _SHAMROCK_H_
#define _SHAMROCK_H_

#include "../../MMDevice/ModuleInterface.h"
#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/MMDevice.h"
#include <stdint.h>
#include <cstdio>
#include <string>
#include <vector>

class AndorShamrock : public CDeviceBase<MM::Generic, AndorShamrock>
{
public:
  AndorShamrock();
  ~AndorShamrock();

  //Required by MM Device
  int Initialize();

  int Shutdown();
  void GetName(char* pszName) const;
  bool Busy();

  int OnSetWavelength(MM::PropertyBase* pProp, MM::ActionType eAct);
  int OnSetGrating(MM::PropertyBase* pProp, MM::ActionType eAct);
  int OnSetInputSideSlitWidth(MM::PropertyBase* pProp, MM::ActionType eAct);
  int OnSetInputDirectSlitWidth(MM::PropertyBase* pProp, MM::ActionType eAct);
  int OnSetOutputSideSlitWidth(MM::PropertyBase* pProp, MM::ActionType eAct);

  int SetSlitWidth(MM::PropertyBase* pProp, MM::ActionType eAct, int slit);

  int OnSetOutputDirectSlitWidth(MM::PropertyBase* pProp, MM::ActionType eAct);
  int OnSetFilter(MM::PropertyBase* pProp, MM::ActionType eAct);
  int OnSetPixelWidth(MM::PropertyBase* pProp, MM::ActionType eAct);
  int OnSetNumberOfPixels(MM::PropertyBase* pProp, MM::ActionType eAct);
  int OnSetShutter(MM::PropertyBase* pProp, MM::ActionType eAct);
  int OnSetInputPort(MM::PropertyBase* pProp, MM::ActionType eAct);
  int OnSetGratingOffset(MM::PropertyBase* pProp, MM::ActionType eAct);
  int OnSetDetectorOffset(MM::PropertyBase* pProp, MM::ActionType eAct);

  int setPort(MM::PropertyBase* pProp, MM::ActionType eAct, int flipper);

  int OnSetOutputPort(MM::PropertyBase* pProp, MM::ActionType eAct);
  int OnSetFocusMirror(MM::PropertyBase* pProp, MM::ActionType eAct);

  void SetGratingsProperty();
  void SetCoefficientsProperty();
  void SetWavelengthProperty();
	void SetRayleighWavelengthProperty();
  void SetPixelWidthProperty();
  void SetNumberPixelsProperty();
  void SetFilterProperty();
  void SetSlitProperty();
  void SetShutterProperty();
  void SetFlipperProperty();
  void SetDetectorOffsetProperty();
  void SetGratingOffsetProperty();
  void SetFlipperMirrorProperty();
  void SetFocusMirrorProperty();
	int GetDetectorOffsetIndices(int *index1, int *index2);
private:
  std::vector<std::string> mvGratings;
  std::vector<std::string> mvFilters;
  std::vector<std::string> mvShutters;
};

#endif _SHAMROCK_H_
