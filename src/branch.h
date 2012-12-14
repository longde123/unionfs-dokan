#include "unionfs.h"

namespace UnionFS 
{
	class BranchServiceImpl;

	class BranchService : public IBranchService {
		friend class BranchServiceImpl;
		BranchServiceImpl *_impl;
	public:
		BranchService();
		BOOL AddBranch(LPCWSTR path, BranchAccessMode mode);
		int GetFileAttributesOnBranch(LPCWSTR filename, LPDWORD attrs, LPDWORD error);
		BOOL GetFilePath(LPWSTR path, size_t maxCount, int branchIndex, LPCWSTR filename);
		int RWBranch();
		~BranchService();
	};
}