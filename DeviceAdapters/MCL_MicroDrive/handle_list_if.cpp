/*
 * Copyright (c) 2008, Mad City Labs Inc.
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

/*
File:		handle_list_if.cpp
Copyright:	Mad City Labs Inc., 2008
License:	Distributed under the BSD license.
*/
#include "heap.h"
#include "device_list.h"
#include "handle_list_if.h"
#include "HandleListType.h"

#include <windows.h>

HANDLE gHandleListMutex = 0;
DeviceList<HandleListType> *gHandleList = 0;

//Called once in dllmain process attach.
bool HandleListCreate()
{	
	//Create a mutex variable to control access to the HandleList
	::gHandleListMutex = CreateMutex(NULL, FALSE, NULL);
	if(::gHandleListMutex == 0)
		return false;
	
	::gHandleList =	(DeviceList<HandleListType>*)GlobalHeapAllocate(sizeof(DeviceList<HandleListType>));
	if(::gHandleList == 0)
	{
		ReleaseMutex(::gHandleListMutex);
		CloseHandle(::gHandleListMutex);
		return false;
	}

	::gHandleList->Init(GlobalHeapGetHandle());

	//Release the lock on the mutex we created
	ReleaseMutex(::gHandleListMutex);

	return true;
}

//Called once in dllmain process detach.
void HandleListDestroy()
{
	gHandleList->RemoveAll();

	GlobalHeapFree(::gHandleList);

	CloseHandle(::gHandleListMutex);
}

int HandleListAddToLockedList(HandleListType* pUsbDevice)
{
	if(!pUsbDevice)
		return -1;

	return ::gHandleList->AddToList(pUsbDevice);
}

void HandleListLock()
{
	WaitForSingleObject(::gHandleListMutex, INFINITE);
}

void HandleListUnlock()
{
	ReleaseMutex(::gHandleListMutex);
}

int HandleListCount()
{
	WaitForSingleObject(::gHandleListMutex, INFINITE);

	int numDevices = ::gHandleList->GetDeviceCount();

	ReleaseMutex(::gHandleListMutex);

	return numDevices;
}

bool HandleExistsOnLockedList(HandleListType* device)
{
	if (!device)
		return false;

	List<HandleListType>* curItem = ::gHandleList->GetHead();
	while(curItem)
	{
		if (*curItem->pd == *device){
			return true;
		}
		curItem = curItem->next;
	}
	return false;
}

bool HandleExistsOnLockedList(int handle)
{
	List<HandleListType>* curItem = ::gHandleList->GetHead();
	while(curItem)
	{
		if (curItem->pd->getHandle() == handle){
			return true;
		}

		curItem = curItem->next;
	}
	return false;
}

int HandleListRemoveSingleItem(HandleListType* device)
{
	if (!device)
		return -1;

	return ::gHandleList->DeleteSingleItem(device);
}
