/*
 * Scion Firewire Camera Device Adapter
 *
 * AUTHOR:
 * Scion Corporation, 2009
 *
 * Copyright (c) 2004-2009 Scion Corporation
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

#ifdef WIN32
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
   #define snprintf _snprintf 
#endif

#include	"ScionCamera.h"

// local references

void	sLogMessage(char msg[]);
void	sLogReset(void);


//----------------------------------------------------------------------------
//
//	LogMessage - routine to write message to log plug-in logfile
//
//----------------------------------------------------------------------------

#ifdef	LOG_ENABLED
void sLogMessage(char msg[])
{ 
#ifdef	WIN32
HFILE file;
OFSTRUCT of;

if (OpenFile("mm_sfwcam.log",&of,OF_EXIST)==HFILE_ERROR)
    file = OpenFile("mm_sfwcam.log",&of,OF_CREATE|OF_WRITE);
else
    file = OpenFile("mm_sfwcam.log",&of,OF_WRITE);

_llseek(file,0,FILE_END);
_lwrite(file,msg,lstrlen(msg));
_lclose(file);

#else
FILE     *fp;
  
fp = fopen("mm_sfwcam.log", "a+");
if (fp != NULL)
    {
    fprintf(fp, "%s\n", msg);
    fclose(fp);
    }
#endif

return;
}
#else
void sLogMessage(char[]) {}
#endif


//----------------------------------------------------------------------------
//
//	LogReset - routine to reset plug-in logfile
//
//----------------------------------------------------------------------------

void sLogReset(void)
{  
#ifdef	LOG_ENABLED

#ifdef	WIN32
HFILE		file;
OFSTRUCT	of;

if((file = OpenFile("mm_sfwcam.log", &of, OF_DELETE)) != HFILE_ERROR)
	{
	_lclose(file);		
	} 

#else
FILE     *fp;
  
fp = fopen("mm_sfwcam.log", "w");
if (fp != NULL)
    {
    fseek(fp, 0l, SEEK_SET);
    fclose(fp);
    }
#endif

#endif

return;
}


