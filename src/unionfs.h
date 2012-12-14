#pragma once
#define NOMINMAX
#include <Windows.h>
#include "dokan.h"

namespace UnionFS {
    // I'm the options of UnionFS, I parse the command line to determine the options.
    struct Options {
        DOKAN_OPTIONS _dokanOptions;
        LPSTR _error;
    public:
        Options();
        ~Options();
        LPCSTR Error() const { return _error; }
        PDOKAN_OPTIONS dokanOptions() { return &_dokanOptions; }
        BOOL Parse(ULONG argc, PWCHAR argv[]);
    };

    // I'm the service manager, I maintain the available services and their implementations.
    enum Service { Sys = 0, Logger, Branch };
    LPVOID GetService(Service svc);
    class ServiceRegister {
        Service _svc;
        LPVOID _obj;
    public:
        ServiceRegister(Service svc, LPVOID obj);
        ~ServiceRegister();
    };


    // I'm branch service, I remember a list of branches and their access modes,
    // so I am responsible for find the right branch to work on.
    enum BranchAccessMode { RO, RW };
    struct IBranchService {
        virtual ~IBranchService() = 0;
        virtual BOOL AddBranch(LPCWSTR path, BranchAccessMode mode) = 0;
        virtual int GetFileAttributesOnBranch(LPCWSTR filename, LPDWORD attrs, LPDWORD error) = 0;
        virtual BOOL GetFilePath(LPWSTR path, size_t maxCount, int branchIndex, LPCWSTR filename) = 0;
        virtual int RWBranch() = 0;
    };

    // I'm the abstract interface for file system API calls.
    // I de-couple UnionFS implemention from call system APIs directly, 
    // so that hippo mock framework can create mock and write unit tests here.
    struct ISysService {
        virtual HANDLE CreateFile(
            _In_ LPCWSTR lpFileName,
            _In_ DWORD dwDesiredAccess,
            _In_ DWORD dwShareMode,
            _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
            _In_ DWORD dwCreationDisposition,
            _In_ DWORD dwFlagsAndAttributes,
            _In_opt_ HANDLE hTemplateFile
            ) = 0 ;
        virtual BOOL CreateDirectory(
            _In_ LPCWSTR lpPathName,
            _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes
            ) = 0;
        virtual DWORD GetFileAttributes(_In_ LPCTSTR lpFileName) = 0;
        virtual DWORD GetLastError() = 0;
        virtual ~ISysService()= 0;
    };

    struct ILoggerService {
        virtual void Error(LPCWSTR format, ...) = 0;
        virtual void Warning(LPCWSTR format, ...) = 0;
        virtual void Debug(LPCWSTR format, ...) = 0;
        virtual ~ILoggerService() = 0;
    protected:
        virtual void Log(LPCWSTR msg) = 0;
    };
}