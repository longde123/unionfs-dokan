#include "unionfs.h"
#include "ops.h"

#include "yaffut.h"
#include "hippomocks.h"
#include "logger_mock.h"

using namespace UnionFS;


class CreateDirectoryTests {
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

	ServiceRegister sysServiceRegister;
	ServiceRegister branchServiceRegister;
	ServiceRegister logServiceRegister;
public:
	CreateDirectoryTests() 
		: sysServiceRegister(Sys, mocks.InterfaceMock<ISysService>()),
		  branchServiceRegister(Branch, mocks.InterfaceMock<IBranchService>()),
		  logServiceRegister(Logger, &loggerMock)
	{
		branches = (IBranchService*)GetService(Branch);
		sys = (ISysService*)GetService(Sys);
	}
};

TEST(CreateDirectoryTests, simple_one_level) {
    //auto call = mocks.ExpectCall(branches, IBranchService::GetFileAttributesOnBranch);
    //mocks.ExpectCall(branches, IBranchService::GetFilePath).With(NullType, NullType, 0, NullType);
    //mocks.ExpectCall(sys, ISysService::CreateDirectory).With(L"C:\\RW\\SubDir", NULL).Return(TRUE);
    //mocks.ExpectCall(sys, ISysService::GetLastError).Return(ERROR_SUCCESS);

	//YAFFUT_EQUAL(0, ops.CreateDirectory(L"SubDir", &info));
}

//TEST(CreateDirectoryTests, two_level) {
//
//	YAFFUT_EQUAL(0, ops.CreateDirectory(_T("AlreadyExistsDir\\SubDir"), info));
//}
//
//TEST(CreateDirectoryTests, already_exists) {
//	YAFFUT_EQUAL(-1 * ERROR_ALREADY_EXISTS, 
//               ops.CreateDirectory(_T("AlreadyExistsDir"), info));
//}
