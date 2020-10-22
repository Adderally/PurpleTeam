#pragma once

#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <winver.h>
#include <string>
#include <Psapi.h>
#include <iostream>

#include"hError.h"

typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

bool getProcessModules(DWORD pid);

void priorityLevel(HANDLE proc);

bool findOutCritStatus(HANDLE proc);
