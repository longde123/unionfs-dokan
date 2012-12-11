#include "unionfs.h"
#include "ops.h"

#include "yaffut.h"
#include "hippomocks.h"

using namespace UnionFS;
FUNC(CreateFile_CREATE_ALWAYS) 
{
	Operations ops;
	MockRepository mocks;
	ISysService* sysMock = mocks.InterfaceMock<ISysService>();
	IBranchService* branchMock = mocks.InterfaceMock<IBranchService>();
}