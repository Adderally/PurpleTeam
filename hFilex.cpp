#include "hFilex.h"




DWORD
catchFileType(HANDLE file) {

	lastError error_return;

	if (file != INVALID_HANDLE_VALUE) {

		DWORD type =  ::GetFileType(file);
		DWORD noErrorPls = GetLastError();
		reportError(noErrorPls);

		if (!catchFileInfo(file))
		{
			DWORD fileInfoError = ::GetLastError();
			reportError(fileInfoError);
		}

		return type;
	}
	else {

		DWORD err = ::GetLastError();
		error_return.last = err;
		reportError(err);
	}

}







bool 
PrintDate(LARGE_INTEGER Date)
{
	BOOL result;
	FILETIME fileTime;
	SYSTEMTIME systemTime;

	fileTime.dwLowDateTime = Date.LowPart;
	fileTime.dwHighDateTime = Date.HighPart;

	result = FileTimeToLocalFileTime(&fileTime, &fileTime);

	if (!result) 
		return result;
	

	result = FileTimeToSystemTime(&fileTime, &systemTime);

	if (!result) 
		return result;
	

	printf("%.2d/%.2d/%.4d %.2d:%.2d",systemTime.wMonth,systemTime.wDay,systemTime.wYear,systemTime.wHour,systemTime.wMinute);

	return true;
}







void
displayFileAttType(ULONG fileAtt) {

	if (fileAtt & 0x20) {
		printf("\nFile attribute = ARCHIVE");
		fileAtt &= ~FILE_ATTRIBUTE_ARCHIVE;
	}
	if (fileAtt & 0x800) {
		printf("\nFile attribute = COMPRESSED");
		fileAtt &= ~FILE_ATTRIBUTE_COMPRESSED;
	}
	if (fileAtt & 0x40) {
		printf("\nFile attribute = DEVICE / SYSTEM LEVEL");
		fileAtt &= ~FILE_ATTRIBUTE_DEVICE;
	}
	if (fileAtt & 0x10) {
		printf("\nFile attribute = DIRECTORY");
		fileAtt &= ~FILE_ATTRIBUTE_DIRECTORY;
	}
	if (fileAtt & 0x2) {
		printf("\nFile attribute = HIDDEN");
		fileAtt &= ~FILE_ATTRIBUTE_HIDDEN;
	}
	if (fileAtt & 0x8000) {
		printf("\nFile attribute = INTEGRITY STREAM");
		fileAtt &= ~FILE_ATTRIBUTE_INTEGRITY_STREAM;
	}
	if (fileAtt & 0x4000) {
		printf("\nFile attribute = ENCRYPTED");
		fileAtt &= ~FILE_ATTRIBUTE_ENCRYPTED;
	}
	if (fileAtt & 0x80) {
		printf("\nFile attribute = NORMAL");
		fileAtt &= ~FILE_ATTRIBUTE_NORMAL;
	}
	if (fileAtt & 0x2000) {
		printf("\nFile attribute = NOT CONTENT INDEXED");
		fileAtt &= ~FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
	}
	if (fileAtt & 0x20000) {
		printf("\nFile attribute = NO SCRUB DATA");
		fileAtt &= ~FILE_ATTRIBUTE_NO_SCRUB_DATA;
	}
	if (fileAtt & 0x1000) {
		printf("\nFile attribute = OFFLINE");
		fileAtt &= ~FILE_ATTRIBUTE_OFFLINE;
	}
	if (fileAtt & 0x1) {
		printf("\nFile attribute = FILE_ATTRIBUTE_READONLY");
		fileAtt &= ~FILE_ATTRIBUTE_READONLY;
	}
	if (fileAtt & 0x400000) {
		printf("\nFile attribute = RECALL ON DATA ACCESS");
		fileAtt &= ~FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS;
	}
	if (fileAtt & 0x40000) {
		printf("\nFile attribute = RECALL ON OPEN");
		fileAtt &= ~FILE_ATTRIBUTE_RECALL_ON_OPEN;
	}
	if (fileAtt & 0x400) {
		printf("\nFile attribute = REPARSE POINT");
		fileAtt &= ~FILE_ATTRIBUTE_REPARSE_POINT;
	}
	if (fileAtt & 0x200) {
		printf("\nFile attribute = SPARSE FILE");
		fileAtt &= ~FILE_ATTRIBUTE_SPARSE_FILE;
	}
	if (fileAtt & 0x4) {
		printf("\nFile attribute = SYSTEM");
		fileAtt &= ~FILE_ATTRIBUTE_SYSTEM;
	}
	if (fileAtt & 0x100) {
		printf("\nFile attribute = TEMPORARY");
		fileAtt &= ~FILE_ATTRIBUTE_TEMPORARY;
	}
	if (fileAtt & 0x10000) {
		printf("\nFile attribute = VIRTUAL");
		fileAtt &= ~FILE_ATTRIBUTE_VIRTUAL;
	}
}










bool
catchFileInfo(HANDLE file) {

	FILE_BASIC_INFO information_class;

	if (GetFileInformationByHandleEx(file, FileBasicInfo, &information_class, sizeof(information_class))) {

		printf("\nFile last write time = ");
		if (!PrintDate(information_class.LastWriteTime))
			printf("Can't find!");



		printf("\nFile last access time = ");
		if (!PrintDate(information_class.LastAccessTime))
			printf("Can't find!");



		printf("\nFile creation time = ");
		if (!PrintDate(information_class.CreationTime))
			printf("Can't find!");


		displayFileAttType(information_class.FileAttributes);



		FILE_STANDARD_INFO information_class_standard;

		if (GetFileInformationByHandleEx(file, FileStandardInfo, &information_class_standard, sizeof(information_class_standard))) {
			printf("\nFile links = %d", information_class_standard.NumberOfLinks);
		}
		else {
			printf("File links = Can't find!");
		}



		return true;

	}
	else {

		DWORD err = ::GetLastError();
		reportError(err);
		return false;

	}

}



bool
displayFileNameSize(HANDLE process) {

	TCHAR workingdir[MAX_PATH];
	DWORD err = 0;

	if (GetModuleFileNameEx(process, NULL, workingdir, MAX_PATH)) {

		std::wcout << "\nFilename = " << workingdir << std::endl;

		HANDLE file = CreateFileW(
			(LPCWSTR)workingdir, // Need to find way to convert TCHAR to LPCSTR
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);



		if (getNetDriveStatus((LPCSTR)workingdir) == 2) {

			err = ::GetLastError();
			reportError(err);
		}



		if (file == INVALID_HANDLE_VALUE) {

			err = ::GetLastError();
			reportError(err);

		}
		else {


			DWORD getFilesz;
			getFilesz = GetFileSize(file, NULL);


			if (getFilesz != INVALID_FILE_SIZE) {

				
				_tprintf(TEXT("File size = %d Kb"), (double)getFilesz / 1024 / 1024 / 1024);

				


			}
			else {
				printf("File Size = Can't find!\n");
				err = ::GetLastError();
				reportError(err);


				CloseHandle(process);
				return FALSE;
			}

		}

	}
	else {
		printf("File Size = Can't find!\n");
		err = ::GetLastError();
		reportError(err);


		CloseHandle(process);
		return FALSE;
	}

	return TRUE;
}






int 
getNetDriveStatus(LPCSTR path) {

	/*

	* // Guess i started working on a method that doesn't work well with modern windows.

	const char* possibleDrives = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int driveM = 0;

	for (char c = *possibleDrives; c; c = *++possibleDrives) {

		// driveM += 1
	}
	return 1;
	*/

	UINT driveReturn = GetDriveTypeA((LPCSTR)path[3]);
	DWORD err = 0;

	switch (driveReturn) {

	case 0:
		printf("\nDrive type = UNKNOWN");
		break;

	case 1:
		printf("\nDrive type = No volume found!");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		std::cout << "\t// INVALID_PATH\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		break;

	case 2:
		printf("\nDrive type = Removable");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		std::cout << "\t// Floppy, USB, flash card reader\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		break;

	case 3:
		printf("\nDrive type = Fixed");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		std::cout << "\t// HDD, SSD, USB\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		break;

	case 4:
		printf("\nDrive type = REMOTE");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		std::cout << "\t// Network drive, local NAS?\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		break;

	case 5:
		printf("\nDrive type = CD-ROM");
		break;

	case 6:
		printf("\nDrive type = RAMDISK");
		break;

	default:
		printf("Drive type = Could not find!");
		

		return 2;
	}

	return 0;
}