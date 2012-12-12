#include "ops.h"

#include "unionfs.h"

#include "assert.h"
#include "tchar.h"

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
	//IBranchService *branches = (IBranchService*)GetService(Branch);
	//assert(branches);

	//DWORD errCode;
	//int idx = branches->FindBranch(AllBranches, FileName, errCode);
	//if (idx < 0)
	//	return -1 *	errCode;// FIXME: should return error code from BranchService

	//WCHAR filePath[MAX_PATH];
	//if (branches->GetFilePath(i, FileName, filePath, MAX_PATH, errCode))
	//	return -1 * errCode;

	//DWORD 

		

	return 0;
}

// ERROR_ALREADY_EXISTS
// ERROR_PATH_NOT_FOUND
static int DOKAN_CALLBACK CreateDirectory(
	LPCWSTR					FileName,
	PDOKAN_FILE_INFO		DokanFileInfo)
{
	ILoggerService *logger = (ILoggerService*)GetService(Logger);
	IBranchService *branches = (IBranchService*)GetService(Branch);
	ISysService    *sys = (ISysService*)GetService(Sys);

	DWORD error, attrs;
	int index = branches->GetFileAttributesOnBranch(FileName, &attrs, &error);
	
	if (index >= 0)
		return -1 * ERROR_ALREADY_EXISTS;

	WCHAR filePath[MAX_PATH];
	branches->GetFilePath(filePath, MAX_PATH, branches->RWBranch(), FileName);

	logger->Debug(_T("CreateDirectory %s\n"), filePath);

	if (!sys->CreateDirectory(filePath, NULL)) {
		error = sys->GetLastError();
		logger->Debug(_T("\t error code = %d\n"), error);
		return -1 * error;
	}

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
		CreateFile = UnionFS::CreateFile;
		OpenDirectory = UnionFS::OpenDirectory;
		CreateDirectory = UnionFS::CreateDirectory;
		Cleanup = UnionFS::Cleanup;
		CloseFile = UnionFS::CloseFile;
		ReadFile = UnionFS::ReadFile;
		WriteFile = UnionFS::WriteFile;
		FlushFileBuffers = UnionFS::FlushFileBuffers;
		GetFileInformation = UnionFS::GetFileInformation;
		FindFiles = UnionFS::FindFiles;
		FindFilesWithPattern = NULL;
		SetFileAttributes = UnionFS::SetFileAttributes;
		SetFileTime = UnionFS::SetFileTime;
		DeleteFile = UnionFS::DeleteFile;
		DeleteDirectory = UnionFS::DeleteDirectory;
		MoveFile = UnionFS::MoveFile;
		SetEndOfFile = UnionFS::SetEndOfFile;
		SetAllocationSize = UnionFS::SetAllocationSize;	
		LockFile = UnionFS::LockFile;
		UnlockFile = UnionFS::UnlockFile;
		GetFileSecurity = UnionFS::GetFileSecurity;
		SetFileSecurity = UnionFS::SetFileSecurity;
		GetDiskFreeSpace = NULL;
		GetVolumeInformation = UnionFS::GetVolumeInformation;
		Unmount = UnionFS::Unmount;
	}
}
