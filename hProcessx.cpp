#include "hProcessx.h"



/* Getting process Priority */



void priorityLevel(HANDLE proc) {
	 
	DWORD priorityLevel;
	priorityLevel = GetPriorityClass(proc);

	switch (priorityLevel) {

	case 0x00008000:
		printf("\nPriority = Above Normal");
		break;
	case 0x00004000:
		printf("\nPriority = Below Normal");
		break;
	case 0x00000080:
		printf("\nPriority = High");
		break;
	case 0x00000040:
		printf("\nPriority = IDLE/Inactive");
		break;
	case 0x00000020:
		printf("\nPriority = Normal");
		break;
	case 0x00000100:
		printf("\nPriority = Realtime");
		break;

	default:
		printf("\nPriority = Can't find!");
		break;
	}
}


/* See if process is Critical or NOT */



bool findOutCritStatus(HANDLE proc) {

	BOOL criticalProcess;
	IsProcessCritical(proc, &criticalProcess);

	if (criticalProcess)
		return TRUE;
	else
		return FALSE;
}




/* Display a snapshot of current loaded modules / .lib files in process */



bool getProcessModules(DWORD pid) {

    HMODULE hMods[1024];
    HANDLE  proc;
    DWORD   cbNeeded;
    unsigned int i;

    DWORD err = 0;

    proc = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, pid);

    if (NULL == proc) {

        err = GetLastError();
        CloseHandle(proc);
        
        reportError(err);
        return false;

        
    }
    else {
   
        LPFN_ISWOW64PROCESS fnIsWow64Process;
        BOOL is64 = FALSE;

        fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
            GetModuleHandle(TEXT("kernel32")),
            "IsWow64Process"
        );

        if (NULL != fnIsWow64Process)
        {
            if (!fnIsWow64Process(GetCurrentProcess(), &is64))
            {
                DWORD err = ::GetLastError();
                reportError(err);
            }
        }

        if (is64)
            printf("\nProcess architecture = 32bit");
        else
            printf("\nProcess architecture = 64bit");

    }
    
    int counter = -1;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
    std::cout << "\n\n/* Catching loaded modules in process */\n\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    if (EnumProcessModulesEx(proc, hMods, sizeof(hMods), &cbNeeded, LIST_MODULES_32BIT))
    {
        for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
        {
            TCHAR modFileName[MAX_PATH];



            if (GetModuleFileNameEx(proc, hMods[i], modFileName, sizeof(modFileName) / sizeof(TCHAR)))
            {
                //  Print mod name of "file name" / location
                counter += 1;
                _tprintf(TEXT("\t[%d] %s (0x%08X)\n"), counter, modFileName, hMods[i]);
            }
        }
    }
    else {
        
        err = GetLastError();
        reportError(err);

        return false;
    }



    CloseHandle(proc);
    return true;
}
