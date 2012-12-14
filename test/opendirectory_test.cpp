#include "unionfs.h"
#include "ops.h"

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
    ServiceRegister sysServiceRegister;
    ServiceRegister branchServiceRegister;
    ServiceRegister logServiceRegister;
public:
    OpenDirectoryTests() 
        : sysServiceRegister(Sys, mocks.InterfaceMock<ISysService>()),
        branchServiceRegister(Branch, mocks.InterfaceMock<IBranchService>()),
        logServiceRegister(Logger, &loggerMock)
    {
        branches = (IBranchService*)GetService(Branch);
        sys = (ISysService*)GetService(Sys);
    }
};

TEST(OpenDirectoryTests, Dir_in_RO) {
    mocks.OnCall(branches, IBranchService::GetFileAttributesOnBranch).Do(
        [](LPCWSTR filename,LPDWORD pattrs, LPDWORD perror) -> int {
            YAFFUT_EQUAL(L"Dir", filename);
            *pattrs = FILE_ATTRIBUTE_DIRECTORY;
            *perror = ERROR_SUCCESS;
            return 5;
        }
    );
    mocks.OnCall(branches, IBranchService::GetFilePath).Do(
        [](LPWSTR path, size_t maxCount, int branchIndex, LPCWSTR filename) -> BOOL {
            YAFFUT_EQUAL(5, branchIndex);
            YAFFUT_EQUAL(L"Dir", filename);
            wcscpy_s(path, maxCount, L"C:\\RO\\Dir");
            return TRUE;
        }
    );
    mocks.ExpectCall(sys, ISysService::CreateFile).Do(
        [](_In_ LPCWSTR lpFileName,
           _In_ DWORD dwDesiredAccess,
           _In_ DWORD dwShareMode,
           _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
           _In_ DWORD dwCreationDisposition,
           _In_ DWORD dwFlagsAndAttributes,
           _In_opt_ HANDLE hTemplateFile
          ) -> HANDLE {
            YAFFUT_EQUAL(L"C:\\RO\\Dir", lpFileName);
            YAFFUT_EQUAL(0, dwDesiredAccess);
            YAFFUT_EQUAL(dwShareMode, FILE_SHARE_READ|FILE_SHARE_WRITE);
            YAFFUT_EQUAL(dwCreationDisposition, OPEN_EXISTING);
            YAFFUT_EQUAL(dwFlagsAndAttributes, FILE_FLAG_BACKUP_SEMANTICS);
            YAFFUT_EQUAL(hTemplateFile, (const HANDLE)NULL);
            return (HANDLE)0x12345678L;
        }
    );
    loggerMock.Disable();
    YAFFUT_EQUAL(0, ops.OpenDirectory(L"Dir", &info));
}

// What's the expected behavior in this case??
//TEST(OpenDirectoryTests, Dir_in_both_branches)
//{
//}

TEST(OpenDirectoryTests, Dir_not_exist) {
    mocks.ExpectCall(branches, IBranchService::GetFileAttributesOnBranch).Do(
        [](LPCWSTR,LPDWORD pattrs, LPDWORD perror) {
            *perror = ERROR_PATH_NOT_FOUND;
            return -1;
        }
    );
    YAFFUT_EQUAL(-1 * ERROR_PATH_NOT_FOUND, ops.OpenDirectory(L"Dir", &info));
}