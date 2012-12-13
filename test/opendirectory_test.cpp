#include "unionfs.h"
#include "ops.h"

#include "branch.h"

#include "yaffut.h"
#include "hippomocks.h"
#include "logger_mock.h"

using namespace UnionFS;

class OpenDirectoryTests {
public:
	// this must be defined before sysServiceRegister and branchServiceRegister 
	// to have correct initialization order.
	MockRepository mocks;
	Operations     ops;
	IBranchService *branches;
	ISysService    *sys;
	DOKAN_FILE_INFO info;
	LoggerMock      loggerMock;
private:
	BranchService branchesImpl;
	ServiceRegister sysServiceRegister;
	ServiceRegister branchServiceRegister;
	ServiceRegister logServiceRegister;
public:
	OpenDirectoryTests() 
		: sysServiceRegister(Sys, mocks.InterfaceMock<ISysService>()),
		  branchServiceRegister(Branch, &branchesImpl),
		  logServiceRegister(Logger, &loggerMock)
	{
		branches = (IBranchService*)GetService(Branch);
		sys = (ISysService*)GetService(Sys);
	}
};

TEST(OpenDirectoryTests, Dir_in_RO) 
{
	int ret = ops.OpenDirectory(L"SomeDir/SomeSubDir", &info);

}

TEST(OpenDirectoryTests, Dir_in_RW) 
{
}

TEST(OpenDirectoryTests, Dir_in_both_branches)
{
}

TEST(OpenDirectoryTests, Dir_in_RO_but_deleted_in_RW)
{

}

//TEST(OpenDirectoryTests, Dir_not_exist)
//{
//	// Setup expected API call sequence for AddBranch
//	mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:/readonly").Return(FILE_ATTRIBUTE_DIRECTORY);
//	mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:/writable").Return(FILE_ATTRIBUTE_DIRECTORY);
//
//	// Setup branches.
//	YAFFUT_EQUAL(TRUE, branches->AddBranch(L"C:/readonly", RO));
//	YAFFUT_EQUAL(TRUE, branches->AddBranch(L"C:/writable", RW));
//
//	// Setup expected API call sequence for AddBranch
//	mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:/writable/SomeDir/SubDir").Return(INVALID_FILE_ATTRIBUTES);
//	mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:/readonly/SomeDir/SubDir").Return(INVALID_FILE_ATTRIBUTES);
//
//	DOKAN_FILE_INFO info;
//	YAFFUT_EQUAL(INVALID_FILE_ATTRIBUTES, ops.OpenDirectory(L"SomeDir/SomeSubDir", &info));
//}