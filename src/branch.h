#include "unionfs.h"

namespace UnionFS 
{
	class BranchService : public IBranchService {
	public:
		BOOL AddBranch(LPCWSTR path, BranchAccessMode mode);
		~BranchService();
	};
}