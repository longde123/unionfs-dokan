#include "sys.h"

#include "windows.h"

namespace UnionFS
{

SysService::~SysService() {}

ISysService::~ISysService() {}

DWORD SysService::GetFileAttributes(LPCTSTR lpFileName) {
	return GetFileAttributes(lpFileName);
}

HANDLE SysService::CreateFile(
	_In_ LPCWSTR lpFileName,
	_In_ DWORD dwDesiredAccess,
	_In_ DWORD dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_ DWORD dwCreationDisposition,
	_In_ DWORD dwFlagsAndAttributes,
	_In_opt_ HANDLE hTemplateFile
	) {
		return CreateFile(lpFileName,  
						  dwDesiredAccess, 
						  dwShareMode, 
						  lpSecurityAttributes, 
						  dwCreationDisposition, 
						  dwFlagsAndAttributes, 
						  hTemplateFile);
}


BOOL SysService::CreateDirectory(
	_In_ LPCWSTR lpPathName,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes
	) {
	return CreateDirectory(lpPathName, lpSecurityAttributes);
}

DWORD SysService::GetLastError(VOID) {
	return GetLastError();
}

}