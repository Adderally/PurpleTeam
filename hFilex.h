#pragma once

#include<Windows.h>
#include<fileapi.h>
#include <Psapi.h>
#include <tchar.h>>

#include "hError.h"		//  For reportError




DWORD
catchFileType(HANDLE file);

bool
catchFileInfo(HANDLE file);

void
displayFileAttType(ULONG fileAtt);

bool
displayFileNameSize(HANDLE process);

/* Only for formating  getProcessCreationTime(...) in 'hProcessx.cpp'*/
bool
PrintDate(LARGE_INTEGER Date);


int getNetDriveStatus(LPCSTR path);