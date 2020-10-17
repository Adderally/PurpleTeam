#pragma once

enum ERROR_CODES {

	FILE_EXISTS = 1
};


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

    int sizeOfFile = 0;
};

bool (*getCriticalState)(HANDLE);
void (*getPriorityLevel)(HANDLE);