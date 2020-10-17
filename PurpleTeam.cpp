// PurpleTeam.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <psapi.h>


#include "accctrl.h"
#include "aclapi.h"





#pragma comment(lib, "advapi32.lib")

// PurpleTeam
#include "PurpleTeam.h"
#include "hProcessx.h"







    int main(int argc, char* pidd[])
    {
        TCHAR    workingdir[MAX_PATH];
        HANDLERS accountHandle = HANDLERS();
        
        
        DWORD pid = atoi(pidd[1]);

        HANDLE process;


        process = OpenProcess(
            PROCESS_ALL_ACCESS, FALSE,
            pid
        );

        

        if (process == NULL) {

            CloseHandle(process);
            
            printf(">\tFailed opening process\n");
            printf(" \tTrying again with lower permissions!\n");
            
            process = OpenProcess(
                PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                FALSE,
                pid
            );

            /* If still null, process must be SYSTEM */

            if (process == NULL) {

                printf(" \tFailed!\n\n");
                accountHandle.error_c = ::GetLastError();
                reportError(accountHandle.error_c);

                return EXIT_FAILURE;
            }

        }


        /* Opening a handle to the file, using OPEN_EXISTING because... if it's running it should exist, right????? lol */


        if (GetModuleFileNameEx(process, NULL, workingdir, MAX_PATH)) {
          
            
            
            accountHandle.hFile = CreateFileW(
                (LPCWSTR)workingdir, // Need to find way to convert TCHAR to LPCSTR
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );



            /* NEED TO FIND WAY TO CONVERT TCHAR TO LPCSTR! */




            if (accountHandle.hFile == INVALID_HANDLE_VALUE) {

                accountHandle.error_c = ::GetLastError();
                reportError(accountHandle.error_c);
            }
            else {

                DWORD getFilesz;
                getFilesz = GetFileSize(accountHandle.hFile, NULL);

                if (getFilesz != INVALID_FILE_SIZE) {
                    //  BYTE bytes[sizeof(DWORD)];
                    //  memcpy(bytes, &fileSize, sizeof(DWORD));
                    //_tprintf(TEXT("File size = %10d Mb\n"), getFilesz / 1024 / 1024);
                    accountHandle.sizeOfFile = getFilesz / 1024 / 1024;
                }
                else {
                    accountHandle.error_c = ::GetLastError();
                    reportError(accountHandle.error_c);
                    CloseHandle(process);
                }
            }
            
        }
        else {
            accountHandle.error_c = ::GetLastError();
            reportError(accountHandle.error_c);
            CloseHandle(process);
        }

        
        

        
       
        
        

        
        accountHandle.dwRtnCode = GetSecurityInfo(
            accountHandle.hFile,
            SE_FILE_OBJECT,
            OWNER_SECURITY_INFORMATION,
            &accountHandle.pSidOwner,
            NULL,
            NULL,
            NULL,
            &accountHandle.pSD
        );

        
        

        
        accountHandle.bRtnBool = LookupAccountSid(
            NULL,           
            accountHandle.pSidOwner,
            accountHandle.AcctName,
            (LPDWORD)&accountHandle.dwAcctName,
            accountHandle.DomainName,
            (LPDWORD)&accountHandle.dwDomainName,
            &accountHandle.eUse
        );

        
        accountHandle.AcctName = (LPTSTR)GlobalAlloc(
            GMEM_FIXED,
            accountHandle.dwAcctName
        );

        
        



        accountHandle.DomainName = (LPTSTR)GlobalAlloc(
            GMEM_FIXED,
            accountHandle.dwDomainName
        );

        
        

        
        accountHandle.bRtnBool = LookupAccountSid(
            NULL,                   
            accountHandle.pSidOwner,
            accountHandle.AcctName,
            (LPDWORD)&accountHandle.dwAcctName,
            accountHandle.DomainName,
            (LPDWORD)&accountHandle.dwDomainName,
            &accountHandle.eUse
        );

      


        if (accountHandle.bRtnBool == TRUE) {


            _tprintf(TEXT("Account/Owner = %s\n"), accountHandle.AcctName);
            CloseHandle(accountHandle.hFile);

            
        }

        


        getCriticalState = &findOutCritStatus;


        if ((*getCriticalState)(process)) {



            printf("Critical = True");
            std::wcout << "\nFilename = " << workingdir << std::endl;
            _tprintf(TEXT("File size = %10d Mb\n"), accountHandle.sizeOfFile / 1024 / 1024);
            getPriorityLevel = &priorityLevel;
            (*getPriorityLevel)(process);

            
        }
        else {



            printf("Critical = False");
            std::wcout << "\nFilename = " << workingdir << std::endl;
            _tprintf(TEXT("File size = %10d Mb\n"), accountHandle.sizeOfFile / 1024 / 1024);
            getPriorityLevel = &priorityLevel;
            (*getPriorityLevel)(process);
        }





        

            
        





        /* Instead of passing around handles, just gonna make a new one with the raw input DWORD */
        if (!getProcessModules(pid)) {
            printf("\n<!>\t\t\tFailed catching modules!\n");
        }
        
        system("pause");

    }
    
