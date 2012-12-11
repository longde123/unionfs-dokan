#include "unionfs.h"

namespace UnionFS
{

class SysService : public ISysService {
public:
	virtual HANDLE CreateFile(
		_In_ LPCWSTR lpFileName,
		_In_ DWORD dwDesiredAccess,
		_In_ DWORD dwShareMode,
		_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		_In_ DWORD dwCreationDisposition,
		_In_ DWORD dwFlagsAndAttributes,
		_In_opt_ HANDLE hTemplateFile
		);
	DWORD GetFileAttributes(LPCTSTR lpFileName);
	~SysService();
};

}