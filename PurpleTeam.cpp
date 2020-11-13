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
#include "hFilex.h"







    int main(int argc, char* pidd[])
    {

        TCHAR    workingdir[MAX_PATH];

        HANDLERS accountHandle = HANDLERS();
        
        UNI_VARS from_main; // struct used for much much later in development.
        
        
        DWORD pid = atoi(pidd[1]);

        HANDLE process;


        process = OpenProcess(
            PROCESS_QUERY_INFORMATION |
            PROCESS_VM_READ,
            FALSE,
            pid
        );

        /* NEED TO FIX */
        

        if (process == NULL) {

            CloseHandle(process);
            
            printf(">\tFailed opening process\n");
            printf(" \tTrying again with lower permissions!\n");
            
            process = OpenProcess(
                PROCESS_QUERY_INFORMATION | 
                PROCESS_VM_READ,
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


        /* Opening a handle to the file, using OPEN_EXISTING because... if it's running it should exist, right????? lol 


        if (GetModuleFileNameEx(process, NULL, workingdir, MAX_PATH)) {
          
            std::wcout << "\nFilename = " << workingdir << std::endl;
            
            accountHandle.hFile = CreateFileW(
                (LPCWSTR)workingdir, // Need to find way to convert TCHAR to LPCSTR
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );
            from_main.fileName = workingdir;






            if (accountHandle.hFile == INVALID_HANDLE_VALUE) {

                accountHandle.error_c = ::GetLastError();
                reportError(accountHandle.error_c);
                
            }
            else {

                
                DWORD getFilesz;
                getFilesz = GetFileSize(accountHandle.hFile, NULL);


                if (getFilesz != INVALID_FILE_SIZE) {
                    
                    //accountHandle.sizeOfFile = getFilesz / 1024 / 1024;
                    _tprintf(TEXT("File size = %10d Mb\n"), getFilesz / 1024 / 1024);

                    from_main.sizeOfFile = getFilesz;
                    

                }
                else {
                    printf("File Size = Can't find!\n");
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
        */

        
        

        accountHandle.hFile = CreateFileW(
            (LPCWSTR)workingdir, // Need to find way to convert TCHAR to LPCSTR
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
       
        
        

        
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

      
        /* Display group that owns file */


        if (accountHandle.bRtnBool == TRUE) {


            _tprintf(TEXT("Group/Owner = %s\n"), accountHandle.AcctName);
            CloseHandle(accountHandle.hFile);

            
        }



        




        
        /* Catch IF critical */


        getCriticalState = &findOutCritStatus;


        if ((*getCriticalState)(process)) {



            printf("Critical = True");
            


            


            /* Catch priority level */


            priorityLevel(process, pid);
            
        }
        else {



            printf("Critical = False");
            


            

            /* Catch priority level */
            

            priorityLevel(process, pid);
            
        }



        CloseHandle(accountHandle.hFile);       //  This handle is no longer used ...




        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        printf("\n\n[File information]");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);





        if (GetModuleFileNameEx(process, NULL, workingdir, MAX_PATH)) {

            if (process != NULL)
                displayFileNameSize(process);
            //  TODO
        }




        HANDLE fileTypeHandle = CreateFileW(    //  new handle for grabing different types of values.
            (LPCWSTR)workingdir, 
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );



        







        /* Start of catching file property chain */

        lastError *error = new lastError();

        if (error->last == NO_ERROR) {

            DWORD typeof = catchFileType(fileTypeHandle);

            switch (typeof) {

            case 0x0002:
                std::cout << "\nFile type = Character / LPT device OR console" << std::endl;
                break;
            case 0x0001:
                std::cout << "\nFile type = Disk" << std::endl;
                break;
            case 0x0003:
                std::cout << "\nFile type = Socket, named pipe, or anonymous pipe" << std::endl;
                break;
            case 0x8000:
                std::cout << "\nFile type = Unused / FILE_TYPE_REMOTE" << std::endl;
                break;
            case 0x0000:
                std::cout << "\nFile type = Unknown" << std::endl;
                break;

            default:
                std::cout << "\nFile type = Issue with matching values!" << std::endl;
                break;
            }
        }
        else {
            std::cout << "\nFile type = Can't read!" << std::endl;
        }

        delete error;




        /* Listing 32bit modules loaded into process */

        if (!getProcessModules(pid)) {
            printf("\n<!>\t\t\tFailed catching modules!\n");
        }


        accountHandle = { };
        //  delete accountHandle;

        
        system("pause");

    }
    
