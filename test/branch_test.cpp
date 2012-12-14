#include "unionfs.h"

#include "branch.h"

#include "yaffut.h"
#include "hippomocks.h"
#include "logger_mock.h"

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
    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RO").Return(FILE_ATTRIBUTE_DIRECTORY);
    YAFFUT_EQUAL(TRUE, branches->AddBranch(L"C:\\RO", RO));
}

TEST(BranchTests, AddBranch_added_path)
{
    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RO").Return(FILE_ATTRIBUTE_DIRECTORY);
    loggerMock.Expect(L"Error:branch already registered");
    loggerMock.Expect(L"Error:branch already registered with different mode");

    YAFFUT_EQUAL(TRUE,  branches->AddBranch(L"C:\\RO", RO));
    YAFFUT_EQUAL(FALSE, branches->AddBranch(L"C:\\RO", RO)); //readd should be rejected.
    YAFFUT_EQUAL(FALSE, branches->AddBranch(L"C:\\RO", RW)); //even different mode.
}

TEST(BranchTests, AddBranch_path_not_exist)
{
    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\NotExist").Return(INVALID_FILE_ATTRIBUTES);
    loggerMock.Expect(L"Error:path does not exist");

    YAFFUT_EQUAL(FALSE, branches->AddBranch(L"C:\\NotExist", RO));
}

TEST(BranchTests, AddBranch_path_is_a_file_path) {
    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\MyFile").Return(FILE_ATTRIBUTE_ARCHIVE);
    loggerMock.Expect(L"Error:path is not a directory");

    YAFFUT_EQUAL(FALSE, branches->AddBranch(L"C:\\MyFile", RO));
}


class GetFileAttributesOnBranchTests : public BranchTests {
public:
    GetFileAttributesOnBranchTests() {
        // set up two branches one for RW another for RW.
        mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RO").Return(FILE_ATTRIBUTE_DIRECTORY);
        mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RW").Return(FILE_ATTRIBUTE_DIRECTORY);
        branches->AddBranch(L"C:\\RO", RO);
        branches->AddBranch(L"C:\\RW", RW);
    }
};

TEST(GetFileAttributesOnBranchTests, file_in_rw_branch) {
    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RW\\MyFile").Return(FILE_ATTRIBUTE_ARCHIVE);
    mocks.ExpectCall(sys, ISysService::GetLastError).Return(ERROR_SUCCESS);
    DWORD attrs = 0, error = 0;
    YAFFUT_EQUAL(0, branches->GetFileAttributesOnBranch(L"MyFile", &attrs, &error));
}

TEST(GetFileAttributesOnBranchTests, file_in_ro_branch) {
    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RW\\MyFile").Return(INVALID_FILE_ATTRIBUTES);
    mocks.ExpectCall(sys, ISysService::GetLastError).Return(ERROR_PATH_NOT_FOUND);

    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RO\\MyFile").Return(FILE_ATTRIBUTE_ARCHIVE);
    mocks.ExpectCall(sys, ISysService::GetLastError).Return(ERROR_SUCCESS);

    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RW\\.unionfs\\MyFile_deleted").Return(INVALID_FILE_ATTRIBUTES);

    DWORD attrs = 0, error = 0;
    YAFFUT_EQUAL(1, branches->GetFileAttributesOnBranch(L"MyFile", &attrs, &error));
    YAFFUT_EQUAL(FILE_ATTRIBUTE_ARCHIVE, attrs);
    YAFFUT_EQUAL(ERROR_SUCCESS, error);
}

//same as file doesn't exist at all
TEST(GetFileAttributesOnBranchTests, file_in_rw_branch_deleted) {
    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RW\\MyFile").Return(INVALID_FILE_ATTRIBUTES);
    mocks.ExpectCall(sys, ISysService::GetLastError).Return(ERROR_PATH_NOT_FOUND);
    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RO\\MyFile").Return(INVALID_FILE_ATTRIBUTES);
    mocks.ExpectCall(sys, ISysService::GetLastError).Return(ERROR_PATH_NOT_FOUND);
    DWORD attrs = 0, error = 0;
    YAFFUT_EQUAL(-1, branches->GetFileAttributesOnBranch(L"MyFile", &attrs, &error));
    YAFFUT_EQUAL(INVALID_FILE_ATTRIBUTES, attrs);
    YAFFUT_EQUAL(ERROR_PATH_NOT_FOUND, error);
}


TEST(GetFileAttributesOnBranchTests, file_in_ro_branch_deleted) {
    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RW\\MyFile").Return(INVALID_FILE_ATTRIBUTES);
    mocks.ExpectCall(sys, ISysService::GetLastError).Return(ERROR_PATH_NOT_FOUND);

    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RO\\MyFile").Return(FILE_ATTRIBUTE_ARCHIVE);
    mocks.ExpectCall(sys, ISysService::GetLastError).Return(ERROR_SUCCESS);

    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RW\\.unionfs\\MyFile_deleted").Return(FILE_ATTRIBUTE_ARCHIVE);

    DWORD attrs = 0, error = 0;
    YAFFUT_EQUAL(-1, branches->GetFileAttributesOnBranch(L"MyFile", &attrs, &error));
    YAFFUT_EQUAL(INVALID_FILE_ATTRIBUTES, attrs);
    YAFFUT_EQUAL(ERROR_PATH_NOT_FOUND, error);
}

// multiple level dir
TEST(GetFileAttributesOnBranchTests, multi_level_file_in_ro_branch_file_deleted) {
    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RW\\MyDir\\MyFile").Return(INVALID_FILE_ATTRIBUTES);
    mocks.ExpectCall(sys, ISysService::GetLastError).Return(ERROR_PATH_NOT_FOUND);

    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RO\\MyDir\\MyFile").Return(FILE_ATTRIBUTE_ARCHIVE);
    mocks.ExpectCall(sys, ISysService::GetLastError).Return(ERROR_SUCCESS);

    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RW\\.unionfs\\MyDir_deleted").Return(INVALID_FILE_ATTRIBUTES);
    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RW\\.unionfs\\MyDir\\MyFile_deleted").Return(FILE_ATTRIBUTE_ARCHIVE);

    DWORD attrs = 0, error = 0;
    YAFFUT_EQUAL(-1, branches->GetFileAttributesOnBranch(L"MyDir\\MyFile", &attrs, &error));
    YAFFUT_EQUAL(INVALID_FILE_ATTRIBUTES, attrs);
    YAFFUT_EQUAL(ERROR_PATH_NOT_FOUND, error);
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
    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RO").Return(FILE_ATTRIBUTE_DIRECTORY);
    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RW").Return(FILE_ATTRIBUTE_DIRECTORY);
    branches->AddBranch(L"C:\\RO", RO);
    branches->AddBranch(L"C:\\RW", RW);

    YAFFUT_EQUAL(0, branches->RWBranch());
}


//TEST(BranchTests, RWBranch_not_found) {
//    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RO").Return(FILE_ATTRIBUTE_DIRECTORY);
//    mocks.ExpectCall(sys, ISysService::GetFileAttributes).With(L"C:\\RW").Return(FILE_ATTRIBUTE_DIRECTORY);
//    branches->AddBranch(L"C:\\RO", RO);
//    branches->AddBranch(L"C:\\RW", RO); // on purpose
//
//    YAFFUT_EQUAL(-1, branches->RWBranch());
//}
