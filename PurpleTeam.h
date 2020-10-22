#pragma once
#include <cstddef>

enum ERROR_CODES {

	FILE_EXISTS = 1
};




typedef struct uniVars {
    DWORD sizeOfFile = NULL;    //    Reps size of the given file.
    TCHAR* fileName = NULL;     //    Reps File name, same as TCHAR xxx[buff].
}UNI_VARS;




struct HANDLERS {

    DWORD dwRtnCode = 0;
    DWORD error_c = 0;
    PSID pSidOwner = NULL;
    BOOL bRtnBool = TRUE;
    LPTSTR AcctName = NULL;
    LPTSTR DomainName = NULL;
    DWORD dwAcctName = 1, dwDomainName = 1;
    SID_NAME_USE eUse = SidTypeUnknown;
    HANDLE hFile;
    PSECURITY_DESCRIPTOR pSD = NULL;

    
};

bool (*getCriticalState)(HANDLE);
void (*getPriorityLevel)(HANDLE);