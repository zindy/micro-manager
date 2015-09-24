/*
 * PrecisExcite controller adapter
 *
 * AUTHOR:
 * Arthur Edelstein, arthuredelstein@gmail.com, 3/17/2009
 *
 * Copyright (c) 2009 Regents of the University of California
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

#ifndef _GENERICSLM_H_
#define _GENERICSLM_H_

#include "ddraw.h"
#include "WinStuff.h"
#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/DeviceUtils.h"
#include <string>
//#include <iostream>
#include <vector>
using namespace std;

#include "DisplayAdapters.h"

vector<MonitorDevice> displays_;
RECT viewBounds;

class GenericSLMWindowsGUIThread : public MMDeviceThreadBase
{
   public:
      GenericSLMWindowsGUIThread(HWND hwnd) : 
         stop_(false), hwnd_(hwnd) {}
      ~GenericSLMWindowsGUIThread() {}
      int svc (void);

      void Stop() {stop_ = true;}
      void Start() {stop_ = false; activate();}

      int RestrictCursor();



   private:
      bool stop_;
      HWND hwnd_;
};

class GenericSLM : public CSLMBase<GenericSLM>
{
public:
   GenericSLM(const char* name);
   ~GenericSLM();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();
   
   int SetImage(unsigned char* pixels);
   int SetImage(unsigned int* pixels);
   int SetPixelsTo(unsigned char red, unsigned char green, unsigned char blue);
   int SetPixelsTo(unsigned char intensity);
   int DisplayImage();
   int SetExposure(double interval_ms);
   double GetExposure();

   unsigned int GetWidth();
   unsigned int GetHeight();
   unsigned int GetNumberOfComponents();
   unsigned int GetBytesPerPixel();

   // action interface
   // ----------------
   int OnGraphicsPort(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnInversion(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnMonochromeColor(MM::PropertyBase* pProp, MM::ActionType eAct);

   int IsSLMSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}

private:
   string graphicsPortDescription_;
   long chosenDisplayIndex_;
   long primaryDisplayIndex_;

   string name_;
   HWND wnd_;

   HDC dc_;

   int error_;

   bool initialized_;

   bool busy_;
   bool allOn_;
   bool allOff_;
   bool invert_;
   bool colorInvert_;

   long inversionNum_; // 0 or 1
   long monochromeColorNum_; // 0,1,2,3,4

   string inversionStr_;
   string monochromeColorStr_;

   GenericSLMWindowsGUIThread * thd_;

   void GenerateModeProperties();
   void GenerateGraphicsPortProperty();

   void CopyIntPixelsToBitmap(unsigned int* pixels);

   bool AttachDisplayDevice(MonitorDevice * dev);
   bool DetachDisplayDevice(MonitorDevice * dev);

   void DeployWindow();
   void RemoveWindow();

   void InitializeDrawContext();
   void DestroyDrawContext();

   void DrawImage();
   BITMAPINFO * createBitmapInfo();
   void ConvertOneByteToFour(unsigned char* pixelsIn, unsigned int * pixelsOut);

   void StripString(string& StringToModify);
   int HandleErrors();

   void MoveWindowToViewingMonitor(HWND wnd);
   bool FixWindows(HWND slmWnd);
   vector<HWND> GetWindowList();

   HDC CreateDeviceContext(string deviceName);
   static BOOL CALLBACK AddWindowToList(HWND wnd, long param);

   int FillDC(HDC hdc, COLORREF color);

   HDC memdc_;
   HBITMAP hbmp_;
   HBITMAP hbmpold_;
   unsigned int * bmpPixels_;
   HDC windc_;
   long bmWidthBytes_;

   void BlitBitmap();

   void WaitForScreenRefresh();
   IDirectDraw * ddObject_;

   WinClass * winClass_;

   POINT GetCoordsOfLeftmostMonitor();

   vector<HWND> windowList_;
   MMThreadLock windowListLock_;





   GenericSLM& operator=(GenericSLM& /*rhs*/) {assert(false); return *this;}
};

// Static function



typedef HRESULT(WINAPI * DIRECTDRAWCREATE) (GUID *, LPDIRECTDRAW *, IUnknown *);


#endif // _GENERICSLM_H_
