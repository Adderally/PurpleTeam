#pragma once

#include <Windows.h>
#include <iostream>
#include <string>

struct lastError {
	DWORD last;
};


void reportError(DWORD err);