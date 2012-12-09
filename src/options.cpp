#include "unionfs.h"

#include <Windows.h>

namespace UnionFS {
	Options::Options() :_error(NULL) {
		ZeroMemory(&_dokanOptions, sizeof(DOKAN_OPTIONS));
		_dokanOptions.Version = DOKAN_VERSION;
		_dokanOptions.ThreadCount = 0; // use default
	}

	Options::~Options() {
		delete _error;
	}

	// Usage design:
	// Unionfs.exe <options> <mount path>
	//
	// mount path,  can be a drive letter (e.g. U:) or an empty folder (e.g. c:\build)
	// options:
	// /r <sources>, you can specify mulitple sources, separate them by colon (':').
	//               you need specify the access permission after each source by appending '=RW' or '=RO'
	//               sources listed before overrides the sources listed after.
	// /d            enable debug output
	// /t <count>    number of worker threads (default is 5)
	//
	BOOL Options::Parse(ULONG argc, PWCHAR argv[]) {
		return TRUE;
	}

}