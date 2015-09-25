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

// HIDDLL.cpp : Defines the entry point for the DLL application.
//
#include "HID.h"
#include "HIDDLL.h"

#define VENDOR_USAGE_PAGE   0xFF00
#define VENDOR_USAGE        0x0001
/* Page-sizes for writes & reads */
//static int WrPageSize;
//static int RdPageSize;
static int devNo[DEV_NUM];
static int devNum;

/*---------------------------------------------------------------------------
   Private in-line code
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
   Functions
---------------------------------------------------------------------------*/
 int  HidInit( )
{
    int i;
    
    HID_Init();
    for ( i = 0; i < DEV_NUM ; i++ )
    {
        devNo[i] = 0;
    }
    return(1);
}

 int  HidUnInit( )
{
    HID_UnInit();
    return(1);
}


/****************************************************************************

  FUNCTION:  HIDInitDevice

  DESCRIPTION:  Setup conditions for reading/writing HID eeprom.

  PARAMETERS:  how many retries to attempt, selection for how many bytes
    in each transfer, selection for HID speed.

  RETURNS:  error code

****************************************************************************/
 int  HidInitDevice( char *productName )
{
    char buf[128];
    int i, devNumbers;
    
    /*
    * JTZ: Note that HID_FindDevices will return index to a internal
    * array which holds the matched HID interfaces.
    */
    devNumbers = HID_FindDevices( (USAGE)VENDOR_USAGE_PAGE, (USAGE)VENDOR_USAGE );
    if ( devNumbers == 0 )
    {
        return ( 0 );
    }

    devNum = 0;
    for ( i = 0 ; i < devNumbers ; i++ )
    {
        /*
        * JTZ: If there's more than one HID Device match our Page(Vendor Page/Page )
        * Let's check the ProductName, and return its first matched index to 
        * internal array.
        */
        if ( HID_GetName( i, buf, sizeof(buf)) == FALSE ) 
        {
            continue;
        }

        if ( strstr( buf, productName ) != NULL )
        {
            devNo[devNum++] = i+1;
        }
    }
    return ( devNum );
}

 int  HidGetDeviceName( int deviceNo, char *name, unsigned int sz )
{
    if ( HID_GetName( devNo[deviceNo]-1, name, sz ) == FALSE ) 
    {
        return ( 0 );
    }
    return ( 1 );
}

 int  HidGetSerialNumber( int deviceNo, char *serialNum, unsigned int sz )
{
    if ( HID_GetSerialNumber( devNo[deviceNo]-1, serialNum, sz ) == FALSE ) 
    {
        return ( 0 );
    }
    return ( 1 );
}

 int  HidOpenDevice( int deviceNo )
{
    if ( devNo[deviceNo] == 0 )
    {
        return ( 0 );
    }
    if ( HID_Open( devNo[deviceNo]-1 ) == TRUE )
    {
        return ( 1 );
    }
    return ( 0 );
}

 void  HidCloseDevice( int deviceNo )
{
    HID_Close( devNo[deviceNo]-1 );
}

 int  HidReadDevice( int deviceNo, unsigned char *buf, unsigned int sz )
{
    unsigned int cnt;
    
    if ( HID_Read( devNo[deviceNo]-1, buf, sz, &cnt ) == TRUE )
    {
        return ( (int)cnt );
    }
    return ( -1 );
}

 int  HidWriteDevice(  int deviceNo, unsigned char *buf, unsigned int sz )
{
    unsigned int cnt;
    
    if ( HID_Write( devNo[deviceNo]-1, buf, sz, &cnt ) == TRUE )
    {
        return ( (int)cnt );
    }
    return ( -1 );
}

 int  HidGetFeature( int deviceNo, unsigned char *buf, unsigned int sz )
{
    if ( HID_GetFeature( devNo[deviceNo]-1, buf, sz ) == TRUE )
    { 
        return ( 1 );
    }
    return ( 0 );
}

 int  HidSetFeature( int deviceNo, unsigned char *buf, unsigned int sz )
{
    if ( HID_SetFeature( devNo[deviceNo]-1, buf, sz ) == TRUE )
    {
        return ( 1 );
    }
    return ( 0 );
}

