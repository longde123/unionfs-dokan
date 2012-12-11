#include "ops.h"

#include "unionfs.h"
#include "assert.h"

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

static int DOKAN_CALLBACK CloseFile(
	LPCWSTR					FileName,
	PDOKAN_FILE_INFO		DokanFileInfo)
{
	return 0;
}

static int DOKAN_CALLBACK OpenDirectory(
	LPCWSTR					FileName,
	PDOKAN_FILE_INFO		DokanFileInfo)
{
	IBranchService *branches = (IBranchService*)GetService(Branch);
	assert(branches);

	DWORD errCode;
	int idx = branches->FindBranch(AllBranches, FileName, errCode);
	if (idx < 0)
		return -1 *	errCode;// FIXME: should return error code from BranchService

	WCHAR filePath[MAX_PATH];
	if (branches->GetFilePath(i, FileName, filePath, MAX_PATH, errCode))
		return -1 * errCode;

	DWORD 

		

	return 0;
}

static int DOKAN_CALLBACK CreateDirectory(
	LPCWSTR					FileName,
	PDOKAN_FILE_INFO		DokanFileInfo)
{
	return 0;
}

static int DOKAN_CALLBACK Cleanup(
	LPCWSTR					FileName,
	PDOKAN_FILE_INFO		DokanFileInfo)
{
	return 0;
}


static int DOKAN_CALLBACK ReadFile(
	LPCWSTR				FileName,
	LPVOID				Buffer,
	DWORD				BufferLength,
	LPDWORD				ReadLength,
	LONGLONG			Offset,
	PDOKAN_FILE_INFO	DokanFileInfo)
{
	return 0;
}

static int DOKAN_CALLBACK WriteFile(
	LPCWSTR		FileName,
	LPCVOID		Buffer,
	DWORD		NumberOfBytesToWrite,
	LPDWORD		NumberOfBytesWritten,
	LONGLONG			Offset,
	PDOKAN_FILE_INFO	DokanFileInfo)
{
	return 0;
}

static int DOKAN_CALLBACK FlushFileBuffers(
	LPCWSTR		FileName,
	PDOKAN_FILE_INFO	DokanFileInfo)
{
	return 0;
}

static int DOKAN_CALLBACK GetFileInformation(
	LPCWSTR							FileName,
	LPBY_HANDLE_FILE_INFORMATION	HandleFileInformation,
	PDOKAN_FILE_INFO				DokanFileInfo)
{
	return 0;
}

static int DOKAN_CALLBACK FindFiles(
	LPCWSTR				FileName,
	PFillFindData		FillFindData, // function pointer
	PDOKAN_FILE_INFO	DokanFileInfo)
{
	return 0;
}

static int DOKAN_CALLBACK DeleteFile(
	LPCWSTR				FileName,
	PDOKAN_FILE_INFO	DokanFileInfo)
{
	return 0;
}

static int DOKAN_CALLBACK DeleteDirectory(
	LPCWSTR				FileName,
	PDOKAN_FILE_INFO	DokanFileInfo)
{
	return 0;
}

static int DOKAN_CALLBACK MoveFile(
	LPCWSTR				FileName, // existing file name
	LPCWSTR				NewFileName,
	BOOL				ReplaceIfExisting,
	PDOKAN_FILE_INFO	DokanFileInfo)
{
	return 0;
}

static int DOKAN_CALLBACK LockFile(
	LPCWSTR				FileName,
	LONGLONG			ByteOffset,
	LONGLONG			Length,
	PDOKAN_FILE_INFO	DokanFileInfo)
{
	return 0;
}

static int DOKAN_CALLBACK SetEndOfFile(
	LPCWSTR				FileName,
	LONGLONG			ByteOffset,
	PDOKAN_FILE_INFO	DokanFileInfo)
{
	return 0;
}

static int DOKAN_CALLBACK SetAllocationSize(
	LPCWSTR				FileName,
	LONGLONG			AllocSize,
	PDOKAN_FILE_INFO	DokanFileInfo)
{
	return 0;
}

static int DOKAN_CALLBACK SetFileAttributes(
	LPCWSTR				FileName,
	DWORD				FileAttributes,
	PDOKAN_FILE_INFO	DokanFileInfo)
{
	return 0;
}

static int DOKAN_CALLBACK SetFileTime(
	LPCWSTR				FileName,
	CONST FILETIME*		CreationTime,
	CONST FILETIME*		LastAccessTime,
	CONST FILETIME*		LastWriteTime,
	PDOKAN_FILE_INFO	DokanFileInfo)
{
	return 0;
}

static int DOKAN_CALLBACK UnlockFile(
	LPCWSTR				FileName,
	LONGLONG			ByteOffset,
	LONGLONG			Length,
	PDOKAN_FILE_INFO	DokanFileInfo)
{
	return 0;
}

static int DOKAN_CALLBACK GetFileSecurity(
	LPCWSTR					FileName,
	PSECURITY_INFORMATION	SecurityInformation,
	PSECURITY_DESCRIPTOR	SecurityDescriptor,
	ULONG				BufferLength,
	PULONG				LengthNeeded,
	PDOKAN_FILE_INFO	DokanFileInfo)
{
	return 0;
}


static int DOKAN_CALLBACK SetFileSecurity(
	LPCWSTR					FileName,
	PSECURITY_INFORMATION	SecurityInformation,
	PSECURITY_DESCRIPTOR	SecurityDescriptor,
	ULONG				SecurityDescriptorLength,
	PDOKAN_FILE_INFO	DokanFileInfo)
{
	return 0;
}

static int DOKAN_CALLBACK GetVolumeInformation(
	LPWSTR		VolumeNameBuffer,
	DWORD		VolumeNameSize,
	LPDWORD		VolumeSerialNumber,
	LPDWORD		MaximumComponentLength,
	LPDWORD		FileSystemFlags,
	LPWSTR		FileSystemNameBuffer,
	DWORD		FileSystemNameSize,
	PDOKAN_FILE_INFO	DokanFileInfo)
{
	return 0;
}

static int DOKAN_CALLBACK Unmount(
	PDOKAN_FILE_INFO	DokanFileInfo)
{
	return 0;
}


Operations::Operations() {
		CreateFile = CreateFile;
		OpenDirectory = OpenDirectory;
		CreateDirectory = CreateDirectory;
		Cleanup = Cleanup;
		CloseFile = CloseFile;
		ReadFile = ReadFile;
		WriteFile = WriteFile;
		FlushFileBuffers = FlushFileBuffers;
		GetFileInformation = GetFileInformation;
		FindFiles = FindFiles;
		FindFilesWithPattern = NULL;
		SetFileAttributes = SetFileAttributes;
		SetFileTime = SetFileTime;
		DeleteFile = DeleteFile;
		DeleteDirectory = DeleteDirectory;
		MoveFile = MoveFile;
		SetEndOfFile = SetEndOfFile;
		SetAllocationSize = SetAllocationSize;	
		LockFile = LockFile;
		UnlockFile = UnlockFile;
		GetFileSecurity = GetFileSecurity;
		SetFileSecurity = SetFileSecurity;
		GetDiskFreeSpace = NULL;
		GetVolumeInformation = GetVolumeInformation;
		Unmount = Unmount;
	}
}
