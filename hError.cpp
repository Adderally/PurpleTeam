
#include"hError.h"

/*
TODO: Make errors red? for some extra scenery effect, black and white can make this seem less dramatic.

	  Also need to make this a BOOL, so i can condtion check if the error handler method also has an error. :O
*/

void reportError(DWORD err) {


	// Not gonna lie, like 95% of this function was ripped from stackoverflow. :|

	if (err != 0) {

		LPSTR returnMessageBuff = nullptr;

		size_t size = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			err,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&returnMessageBuff,
			0,
			NULL
		);

		std::string errorMessage(returnMessageBuff, size);
		LocalFree(returnMessageBuff);

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		std::cout << "\n\n[System]\t";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		std::cout << errorMessage << std::endl;
	}
	/*
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		std::cout << "\n\n[System]\t";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		std::cout << "Unknown error!" << std::endl;

	}
	*/
}