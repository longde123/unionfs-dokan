#include "unionfs.h"
#include "branch.h"

namespace UnionFS {
	BOOL BranchService::AddBranch(LPCWSTR path, BranchAccessMode mode) {
		return FALSE;
	}

	BranchService::~BranchService() {

	}

	IBranchService::~IBranchService() {
	}
}