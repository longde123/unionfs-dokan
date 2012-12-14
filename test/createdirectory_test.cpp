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
    mocks.ExpectCall(branches, IBranchService::GetFileAttributesOnBranch).Do(
        [](LPCWSTR filename, LPDWORD, LPDWORD) -> int {
            YAFFUT_EQUAL(L"SubDir", filename);
            return -1;
    }
    );
    mocks.ExpectCall(branches, IBranchService::RWBranch).Return(0);
    mocks.ExpectCall(branches, IBranchService::GetFilePath).Do(
        [](LPWSTR path, size_t maxCount, int branchIndex, LPCWSTR filename) -> BOOL {
            YAFFUT_EQUAL(0, branchIndex);
            YAFFUT_EQUAL(L"SubDir", filename);
            wcscpy_s(path, maxCount, L"C:\\RW\\SubDir");
            return TRUE;
    }
    );
    loggerMock.Expect(L"Debug:CreateDirectory C:\\RW\\SubDir");
    mocks.ExpectCall(sys, ISysService::CreateDirectory).With(L"C:\\RW\\SubDir", NULL).Return(TRUE);

    YAFFUT_EQUAL(0, ops.CreateDirectory(L"SubDir", &info));
}

TEST(CreateDirectoryTests, one_level_already_exist) {
    mocks.ExpectCall(branches, IBranchService::GetFileAttributesOnBranch).Do(
        [](LPCTSTR filename, LPDWORD pattrs, LPDWORD perror) -> int {
            YAFFUT_EQUAL(L"AlreadyExistsDir", filename);
            *pattrs = FILE_ATTRIBUTE_DIRECTORY;
            *perror = ERROR_SUCCESS;
            return 0;
    }
    );
    YAFFUT_EQUAL(-1 * ERROR_ALREADY_EXISTS, 
        ops.CreateDirectory(L"AlreadyExistsDir", &info));
}


TEST(CreateDirectoryTests, two_level_parent_exist) {
    // Assume we have two branches, 0 -> RO, 1 -> RW;
    // 0 already has a dir names Parent, 1 is empty.
    mocks.OnCall(branches, IBranchService::GetFileAttributesOnBranch).Do(
        [&](LPCTSTR filename, LPDWORD pattrs, LPDWORD perror) -> int {
            if (wcscmp(filename, L"Parent\\New") == 0)
                return -1;
            if (wcscmp(filename, L"Parent") == 0) {
                *pattrs = FILE_ATTRIBUTE_DIRECTORY;
                *perror = ERROR_SUCCESS;
                return 1;
            }
            YAFFUT_FAIL("unknown input");
            return -1;
    }
    );
    // RW branch is 1
    mocks.OnCall(branches, IBranchService::RWBranch).Return(1);

    mocks.OnCall(branches, IBranchService::GetFilePath).Do(
        [&](LPWSTR path, size_t maxCount, int branchIndex, LPCWSTR filename) -> BOOL {
            YAFFUT_EQUAL(1, branchIndex);
            if (wcscmp(L"Parent", filename) == 0) 
                wcscpy_s(path, maxCount, L"C:\\RW\\Parent");
            else if (wcscmp(L"Parent\\New", filename) == 0)
                wcscpy_s(path, maxCount, L"C:\\RW\\Parent\\New");
            else
                YAFFUT_FAIL("unknown filename");
            return TRUE;
    }
    );

    mocks.ExpectCall(sys, ISysService::CreateDirectory).With(L"C:\\RW\\Parent").Return(TRUE);
    mocks.ExpectCall(sys, ISysService::CreateDirectory).With(L"C:\\RW\\Parent\\New").Return(TRUE);

    // verify log output.
    loggerMock.Expect(L"Debug:CreateDirectory C:\\RW\\Parent");
    loggerMock.Expect(L"Debug:CreateDirectory C:\\RW\\Parent\\New");

    YAFFUT_EQUAL(0, ops.CreateDirectory(L"Parent\\New", &info));
} 

TEST(CreateDirectoryTests, two_level_parent_not_exist) {
    mocks.OnCall(branches, IBranchService::GetFileAttributesOnBranch).Return(-1);
    YAFFUT_EQUAL(-1 * ERROR_PATH_NOT_FOUND, ops.CreateDirectory(L"Parent\\New", &info));
}
