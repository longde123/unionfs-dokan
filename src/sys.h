#include "unionfs.h"

namespace UnionFS
{

class SysService : public ISysService {
public:
	HANDLE CreateFile(
		_In_ LPCWSTR lpFileName,
		_In_ DWORD dwDesiredAccess,
		_In_ DWORD dwShareMode,
		_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		_In_ DWORD dwCreationDisposition,
		_In_ DWORD dwFlagsAndAttributes,
		_In_opt_ HANDLE hTemplateFile
		);
	BOOL CreateDirectory(
		_In_ LPCWSTR lpPathName,
		_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes
		);
	DWORD GetFileAttributes(LPCTSTR lpFileName);
	DWORD GetLastError(VOID);
	~SysService();
};

}