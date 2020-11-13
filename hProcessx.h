#pragma once

#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <winver.h>
#include <string>
#include <Psapi.h>
#include <iostream>

#include"hError.h" // reportError
#include"hFilex.h" // printData




enum hTHREAD_FAIL_TYPES {

	FAILED_HANDLE = 2,
	FAILED_FIRST  = 3
};




typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS)
(HANDLE, PBOOL);

bool 
getProcessModules(DWORD pid);

void 
priorityLevel(HANDLE proc, DWORD pid);

bool 
findOutCritStatus(HANDLE proc);

int
getThreadCount(DWORD proc);


bool
getProcessCreationTime(HANDLE proc);