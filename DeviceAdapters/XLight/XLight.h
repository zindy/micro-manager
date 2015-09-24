/*
 * CrEST XLight adapter
 *
 * AUTHOR:
 * E. Chiarappa echiarappa@libero.it, 01/20/2014
 * Based on CARVII adapter by  G. Esteban Fernandez.
 *
 * Copyright (c) 2014 Crestoptics s.r.l.
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

#ifndef _XLUPD_H_
#define _XLUPD_H_

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include <string>
#include <map>

//////////////////////////////////////////////////////////////////////////////
// Error codes
//
#define ERR_UNKNOWN_COMMAND          10002
#define ERR_UNKNOWN_POSITION         10003
#define ERR_HALT_COMMAND             10004

class Hub : public CGenericBase<Hub> {
public:
    Hub();
    ~Hub();

    // Device API
    // ----------
    int Initialize();
    int Shutdown();

    void GetName(char* pszName) const;
    bool Busy();

    // action interface
    // ----------------
    int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
    bool initialized_;
    // MMCore name of serial port
    std::string port_;
    // Command exchange with MMCore
    std::string command_;
};
 

 
 
class Dichroic : public CStateDeviceBase<Dichroic> {
public:
    Dichroic();
    ~Dichroic();

    // MMDevice API                                                           
    // ------------                                                           
    int Initialize();
    int Shutdown();

    void GetName(char* pszName) const;
    bool Busy();

    unsigned long GetNumberOfPositions()const {
        return numPos_;
    }

    // action interface                                                       
    // ----------------                                                       
    int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
    bool initialized_;
    unsigned numPos_;
    long pos_;
    std::string name_;
};

 
class Emission : public CStateDeviceBase<Emission> {
public:
    Emission();
    ~Emission();

    // MMDevice API                                                           
    // ------------                                                           
    int Initialize();
    int Shutdown();

    void GetName(char* pszName) const;
    bool Busy();

    unsigned long GetNumberOfPositions()const {
        return numPos_;
    }

    // action interface                                                       
    // ----------------                                                       
    int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
    bool initialized_;
    unsigned numPos_;
    long pos_;
    std::string name_;
}; 

class SpinMotor : public CStateDeviceBase<SpinMotor> {
public:
    SpinMotor();
    ~SpinMotor();

    // MMDevice API                                                           
    // ------------                                                           
    int Initialize();
    int Shutdown();

    void GetName(char* pszName) const;
    bool Busy();

    unsigned long GetNumberOfPositions()const {
        return numPos_;
    }

    // action interface                                                       
    // ----------------                                                       
    int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
    bool initialized_;
    unsigned numPos_;
    long pos_;
    std::string name_;
};

class DiskSlider : public CStateDeviceBase<DiskSlider> {
public:
    DiskSlider();
    ~DiskSlider();

    // MMDevice API                                                           
    // ------------                                                           
    int Initialize();
    int Shutdown();

    void GetName(char* pszName) const;
    bool Busy();

    unsigned long GetNumberOfPositions()const {
        return numPos_;
    }

    // action interface                                                       
    // ----------------                                                       
    int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
    bool initialized_;
    unsigned numPos_;
    long pos_;
    std::string name_;
};

 
class TouchScreen : public CStateDeviceBase<TouchScreen> {
public:
    TouchScreen();
    ~TouchScreen();

    // MMDevice API                                                           
    // ------------                                                           
    int Initialize();
    int Shutdown();

    void GetName(char* pszName) const;
    bool Busy();

    unsigned long GetNumberOfPositions()const {
        return numPos_;
    }

    // action interface                                                       
    // ----------------                                                       
    int OnState(MM::PropertyBase* pProp, MM::ActionType eAct);

private:
    bool initialized_;
    unsigned numPos_;
    long pos_;
    std::string name_;
};

#endif //_XLIGHT_H_
