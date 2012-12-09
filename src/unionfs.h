#include <Windows.h>
#include "dokan.h"

namespace UnionFS {

	// I'm the options of UnionFS, I parse the command line to determine the options.
	struct Options {
		DOKAN_OPTIONS _dokanOptions;
		LPCWSTR _error;
		BOOL _isDebug;
	public:
		Options();
		~Options();
		LPCWSTR Error() const { return _error; }
		BOOL Parse(ULONG argc, PWCHAR argv[]);
	};

	// I'm the abstract interface for file system API calls.
	// I de-couple UnionFS implemention from WinAPI calls, 
	// so that some mock and unit tests can be added here.
	struct NativeFS {
		virtual ~NativeFS();
	};
}