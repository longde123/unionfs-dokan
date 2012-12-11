#include "unionfs.h"
#include "ops.h"

#include "yaffut.h"
#include "hippomocks.h"
#include "tchar.h"

using namespace UnionFS;

FUNC(OpenDirectory_in_RO_branch) 
{
	Operations ops;

	DOKAN_FILE_INFO info;
	int ret = ops.OpenDirectory(_T("SomeDir/SomeSubDir"), &info);

}

FUNC(OpenDirectory_in_RW_branch) 
{
}

FUNC(OpenDirectory_in_both_branches)
{
}

FUNC(OpenDirectory_not_exist)
{
	MockRepository mocks;
	IBranchService* branchMock = mocks.InterfaceMock<IBranchService>();
	
	ServiceRegister branchService(Branch, branchMock);
	DOKAN_FILE_INFO info;
	int ret = Operations().OpenDirectory(_T("SomeDir/SomeSubDir"), &info);

}