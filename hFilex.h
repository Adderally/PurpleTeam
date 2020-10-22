#pragma once

#include<Windows.h>
#include<fileapi.h>

#include "hError.h"		//  For reportError




DWORD
catchFileType(HANDLE file);

bool
catchFileInfo(HANDLE file);

void
displayFileAttType(ULONG fileAtt);