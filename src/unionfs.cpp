#include "unionfs.h"

#include <Windows.h>
#include <tchar.h>


int __cdecl
wmain(ULONG argc, PWCHAR argv[])
{
	UnionFS::Options opts;
	if (!opts.Parse(argc, argv))
	{
		wprintf(opts.Error());
		return -1;
	}
	return 0;
}