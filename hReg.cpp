/*
#include <winreg.h>
#include <tchar.h>
#include <winerror.h>


#include "hReg.h"


int 
openUsrKey() {

	HKEY key;
	DWORD err = 0;
	long pass_or_fail = RegOpenKeyEx(
		HKEY_CURRENT_USER,
		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),
		0,
		KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_SET_VALUE,
		&key
	);

	if (pass_or_fail != ERROR_SUCCESS) {

		err = ::GetLastError();
		reportError(err);
		
		return FAIL_OPEN_KEY;
	}
}
*/