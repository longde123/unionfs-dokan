#include "unionfs.h"

#include "branch.h"

#include "yaffut.h"
#include "hippomocks.h"
#include "logger_mock.h"
#include "tchar.h"

using namespace UnionFS;

class BranchTests {
protected:
	MockRepository  mocks;
	LoggerMock		loggerMock;
	IBranchService *branches;
	ISysService    *sys;
	ILoggerService *log;
private:
	BranchService   _branches;
	ServiceRegister _branchesRegister;
	ServiceRegister _sysRegister;
	ServiceRegister _logRegister;
protected:
	BranchTests() 
		: _branchesRegister(Branch, &_branches),
		  _sysRegister(Sys, mocks.InterfaceMock<ISysService>()),
		  _logRegister(Logger, &loggerMock)
	{
		branches = (IBranchService*)GetService(Branch);
		sys      = (ISysService*)GetService(Sys);
		log      = (ILoggerService*)GetService(Logger);
	}
};


TEST(BranchTests, AddBranch_simple)
{
	mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(_T("C:\\RO")).Return(FILE_ATTRIBUTE_DIRECTORY);
	YAFFUT_EQUAL(TRUE, branches->AddBranch(_T("C:\\RO"), RO));
}

TEST(BranchTests, AddBranch_added_path)
{
	mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(_T("C:\\RO")).Return(FILE_ATTRIBUTE_DIRECTORY);
	loggerMock.Expect(_T("Error:branch already registered"));
	loggerMock.Expect(_T("Error:branch already registered with different mode"));

	YAFFUT_EQUAL(TRUE,  branches->AddBranch(_T("C:\\RO"), RO));
	YAFFUT_EQUAL(FALSE, branches->AddBranch(_T("C:\\RO"), RO)); //readd should be rejected.
	YAFFUT_EQUAL(FALSE, branches->AddBranch(_T("C:\\RO"), RW)); //even different mode.
}

TEST(BranchTests, AddBranch_path_not_exist)
{
	mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(_T("C:\\NotExist")).Return(INVALID_FILE_ATTRIBUTES);
	loggerMock.Expect(_T("Error:path does not exist"));

	YAFFUT_EQUAL(FALSE, branches->AddBranch(_T("C:\\NotExist"), RO));
}

TEST(BranchTests, AddBranch_path_is_a_file_path)
{
	mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(_T("C:\\MyFile")).Return(FILE_ATTRIBUTE_ARCHIVE);
	loggerMock.Expect(_T("Error:path is not a directory"));

	YAFFUT_EQUAL(FALSE, branches->AddBranch(_T("C:\\MyFile"), RO));
}

TEST(BranchTests, GetFileAttributesOnBranch) {

	mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(_T("C:\\RO")).Return(FILE_ATTRIBUTE_DIRECTORY);
	mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(_T("C:\\RW")).Return(FILE_ATTRIBUTE_DIRECTORY);
	branches->AddBranch(_T("C:\\RO"), RO);
	branches->AddBranch(_T("C:\\RW"), RW);

    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(_T("C:\\RW\\MyDir\\MyFile")).Return(FILE_ATTRIBUTE_ARCHIVE);
	mocks.ExpectCall(sys, ISysService::GetLastError).Return(ERROR_SUCCESS);
	DWORD attrs = 0, error = 0;
	YAFFUT_EQUAL(0, branches->GetFileAttributesOnBranch(_T("MyDir\\MyFile"), &attrs, &error));
}

TEST(BranchTests, GetFilePath_simple)
{
	mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RO").Return(FILE_ATTRIBUTE_DIRECTORY);
	branches->AddBranch(L"C:\\RO", RO);

	WCHAR filePath[MAX_PATH];
	YAFFUT_EQUAL(TRUE, branches->GetFilePath(filePath, MAX_PATH, 0, L"MyFile"));
	YAFFUT_EQUAL(L"C:\\RO\\MyFile", filePath);
}

TEST(BranchTests, GetFilePath_invalid_branch)
{
    loggerMock.Expect(L"Error:Invalid branch 0");

	WCHAR filePath[MAX_PATH];
	YAFFUT_EQUAL(FALSE, branches->GetFilePath(filePath, MAX_PATH, 0, L"MyFile"));
}

TEST(BranchTests, GetFilePath_buffer_too_small)
{
	mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RO").Return(FILE_ATTRIBUTE_DIRECTORY);
	branches->AddBranch(L"C:\\RO", RO);

    loggerMock.Expect(L"Error:Buffer too small");
	WCHAR filePath[1];
	YAFFUT_EQUAL(FALSE, branches->GetFilePath(filePath, 1, 0, L"MyFile"));
}

TEST(BranchTests, RWBranch) {
	mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(_T("C:\\RO")).Return(FILE_ATTRIBUTE_DIRECTORY);
	mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(_T("C:\\RW")).Return(FILE_ATTRIBUTE_DIRECTORY);
	branches->AddBranch(_T("C:\\RO"), RO);
	branches->AddBranch(_T("C:\\RW"), RW);

	YAFFUT_EQUAL(0, branches->RWBranch());
}


TEST(BranchTests, RWBranch_not_found) {
	mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(_T("C:\\RO")).Return(FILE_ATTRIBUTE_DIRECTORY);
	mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(_T("C:\\RW")).Return(FILE_ATTRIBUTE_DIRECTORY);
	branches->AddBranch(_T("C:\\RO"), RO);
	branches->AddBranch(_T("C:\\RW"), RO); // on purpose

	YAFFUT_EQUAL(-1, branches->RWBranch());
}
