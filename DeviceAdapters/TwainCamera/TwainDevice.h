
/*
 * implementation classes for Twain Camera
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

#pragma once
#include <vector>
#include <string>
#include <map>

typedef unsigned short uint16_t;

union rectangle_t
{
	long long values_;
	uint16_t lefttoprightbottom_[4];
};

// forward declaration allows use of container object's facilities, but keeps the include depdencies simple
class TwainCamera;

// the implementation class for the Twain Library
class TwImpl;

class TwainDevice
{
private:
	// disallow default ctor
	TwainDevice(void);
public:
	// the only public ctor
	TwainDevice(TwainCamera* pcamera__);

	// make this virtual if the class is to be extended.
	~TwainDevice(void);

	char* GetImage(int& imh__, int& imw__, char& bpp__);
	void EnableCamera(const bool enable);
	std::vector<std::string> AvailableSources(void);
	void SelectAndOpenSource(std::string);
	void SetROIRectangle( const uint16_t& left__, const uint16_t& top__, const uint16_t& right__, const uint16_t& bottom__, uint16_t* pactualwidth__ = NULL, uint16_t* pactualheight__ = NULL, unsigned char* pactualdepth__ = NULL);
	void GetROIRectangle(uint16_t& left__, uint16_t& top__, uint16_t& right__, uint16_t& bottom__);
	void GetWholeCaptureRectangle(uint16_t& left__, uint16_t& top__, uint16_t& right__, uint16_t& bottom__);
	void GetActualImageSize(uint16_t& imheight__, uint16_t& imwidth__, char& bytesppixel__);
	void GetActualImageSize(uint16_t& left__, uint16_t& top__, uint16_t& right__, uint16_t& bottom__, uint16_t& imheight__, uint16_t& imwidth__, char& bytesppixel__);

	void ClearROI(uint16_t* pactualwidth__ = NULL, uint16_t* pactualheight__ = NULL, unsigned char* pactualdepth__ = NULL);

	std::string TwainDevice::CurrentSource();
   void LaunchVendorSettings(void);
	void StopTwain(void);
	void CurrentlySelectedSource(const std::string sourceName);
	std::string CurrentlySelectedSource(void);


private:

	TwImpl* pTwImpl_;
	std::map<long long, std::pair<uint16_t,uint16_t> > roiImageSizes_;
	char currentpixeldepth_;


	// TwainDevice   is an adapter from the implemenation to DeviceBase instantiation
	
	// the wrapper class
	TwainCamera* pcamera_;

	// for performance measurement:
	double imageTransferStartTime_; 
	double previousImageStartTime_;


};



