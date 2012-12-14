#include "unionfs.h"

#include "branch.h"
#include "logger.h"
#include "sys.h"
#include "ops.h"

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

#include <assert.h>

using namespace UnionFS;

int __cdecl wmain(ULONG argc, PWCHAR argv[]) 
{
	BranchService branchService;
	LoggerService loggerService;
	SysService    sysService;
	ServiceRegister(Branch, &branchService);
	ServiceRegister(Logger, &loggerService);
	ServiceRegister(Sys,    &sysService);

	Options opts;
	if (!opts.Parse(argc, argv)) {
		fprintf(stderr, opts.Error());
		return -1;
	}
	
	Operations operations;
	int status = DokanMain(opts.dokanOptions(), &operations);
	switch(status)
	{
	case DOKAN_SUCCESS:
		fprintf(stderr, "Success\n");
		break;
	case DOKAN_ERROR:
		fprintf(stderr, "Error\n");
		break;
	case DOKAN_DRIVE_LETTER_ERROR:
		fprintf(stderr, "Bad Drive letter\n");
		break;
	case DOKAN_DRIVER_INSTALL_ERROR:
		fprintf(stderr, "Can't install driver\n");
		break;
	case DOKAN_START_ERROR:
		fprintf(stderr, "Driver something wrong\n");
		break;
	case DOKAN_MOUNT_ERROR:
		fprintf(stderr, "Can't assign a drive letter\n");
		break;
	case DOKAN_MOUNT_POINT_ERROR:
		fprintf(stderr, "Mount point error\n");
		break;
	default:
		fprintf(stderr, "Unknown error: %d\n", status);
		break;
	}

	return status;
}