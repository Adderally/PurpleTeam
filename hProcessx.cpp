#include "hProcessx.h"



/* Getting process Priority */



void priorityLevel(HANDLE proc, DWORD pid) {
	 
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
    
   // getProcessCreationTime(proc);

    
    if (getThreadCount(pid) != 1) {

        printf("\n<!>\t\t\tFailed building handle!!\n");
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
    MODULEINFO info_of_dll;
    DWORD   cbNeeded;
    unsigned int i;

    DWORD err = 0;

    proc = OpenProcess(
        PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, pid);

    if (NULL == proc) {

        err = GetLastError();
        CloseHandle(proc);
        
        reportError(err);
        return false;

        
    }
    
    

    int counter = -1;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
    std::cout << "\n\n/* Catching loaded modules in process */\n\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);



    if (EnumProcessModulesEx(proc, hMods, sizeof(hMods), &cbNeeded, 0x03)) // Listing all
    {
        for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
        {
            TCHAR modFileName[MAX_PATH];



            if (GetModuleFileNameEx(proc, hMods[i], modFileName, sizeof(modFileName) / sizeof(TCHAR)))
            {
               
                

                if (GetModuleInformation(proc, hMods[i], &info_of_dll, sizeof(MODULEINFO))) {

                    counter += 1;
                    _tprintf(TEXT("\t[%d] %s | Entry point = 0x%08X | Load point = 0x%08X | Size of Image (Bytes) = %d\n\n"), counter, modFileName, info_of_dll.EntryPoint, info_of_dll.lpBaseOfDll, info_of_dll.SizeOfImage);
                }
                else {
                    counter += 1;
                    _tprintf(TEXT("\t[%d] %s (0x%08X)\n"), counter, modFileName, hMods[i]);
                }
                //  Print mod name of "file name" / location
                //counter += 1;
                //_tprintf(TEXT("\t[%d] %s (0x%08X)\n"), counter, modFileName, hMods[i]);
            }
        }
    }
    else {
        
        err = GetLastError();
        reportError(err);
        CloseHandle(proc);

        return false;
    }



    CloseHandle(proc);
    return true;
}








/*       All thread stuff       */        


int
getThreadCount(DWORD proc) {


    HANDLE thread_snap = INVALID_HANDLE_VALUE;
    THREADENTRY32   threads;
    DWORD err = 0;

    thread_snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0x0);

    if (thread_snap == INVALID_HANDLE_VALUE) {

        err = ::GetLastError();
        reportError(err);
        CloseHandle(thread_snap);

        return FAILED_HANDLE; // 2
    }
    threads.dwSize = sizeof(THREADENTRY32);



    /* First thread, main? */

    if (!Thread32First(thread_snap, &threads)) {

        err = ::GetLastError();
        reportError(err);
        CloseHandle(thread_snap);

        return FAILED_FIRST; // 3
    }
    else{

        int tcount = 0;
        printf("\n\n");

        do {
            
            if (threads.th32OwnerProcessID == proc) {

                _tprintf(TEXT("[%d] Thread ID = 0x%08X\n"),tcount, threads.th32ThreadID);
                // Need to find way to cross reference thread priority with process priority
                // https://docs.microsoft.com/en-us/windows/win32/procthread/scheduling-priorities shows that each value can mean something different depending on process priority.
                _tprintf(TEXT("    Priority  = %d\n\n"), threads.tpBasePri);
                

                tcount += 1;
            }
        } while (Thread32Next(thread_snap, &threads));
    }


    CloseHandle(thread_snap);
    return 1;
}







bool
getProcessCreationTime(HANDLE proc) {

    

    return true;
}