#include "unionfs.h"

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

namespace UnionFS {

static int DOKAN_CALLBACK CreateFile(
	LPCWSTR					FileName,
	DWORD					AccessMode,
	DWORD					ShareMode,
	DWORD					CreationDisposition,
	DWORD					FlagsAndAttributes,
	PDOKAN_FILE_INFO		DokanFileInfo)
{
	return 0;
}


struct Operations : public DOKAN_OPERATIONS {
	Operations() {
		this->CreateFile = UnionFS::CreateFile;
	}
};

}


int __cdecl wmain(ULONG argc, PWCHAR argv[]) {
	UnionFS::Options opts;
	if (!opts.Parse(argc, argv)) {
		fprintf(stderr, opts.Error());
		return -1;
	}

	UnionFS::Operations operations;
	int status = DokanMain(&opts._dokanOptions, &operations);
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