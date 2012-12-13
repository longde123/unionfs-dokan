#include "unionfs.h"
#include "branch.h"

#include "tchar.h"
#include <vector>
#include <string>
#include <algorithm>

namespace UnionFS {

    struct BranchOption {
        std::wstring	 path;
        BranchAccessMode mode;
        BranchOption() {}
        BranchOption(const std::wstring& path, BranchAccessMode mode) {
            this->path = path; this->mode = mode;
        }
    };

    class BranchServiceImpl {
        BranchService* _api;
        std::vector<BranchOption> _branches;
    public:
        BranchServiceImpl(BranchService* api) : _api(api) { }
        ~BranchServiceImpl() { _api = NULL; }
        BOOL AddBranch(LPCWSTR path, BranchAccessMode mode);
        int GetFileAttributesOnBranch(LPCWSTR filename, LPDWORD attrs, LPDWORD error);
        BOOL GetFilePath(LPWSTR path, size_t maxCount, int branchIndex, LPCWSTR filename);
        int RWBranch();
    private:
        void appendPath(std::wstring &base, LPCWSTR path);
    };

    // Api implementation
    BranchService::BranchService() {
        _impl = new BranchServiceImpl(this);
    }

    BranchService::~BranchService() {
        delete _impl;
    }

    IBranchService::~IBranchService() { }

    BOOL BranchService::AddBranch(LPCWSTR path, BranchAccessMode mode) {
        return _impl->AddBranch(path, mode);
    }

    int BranchService::GetFileAttributesOnBranch(LPCWSTR filename, LPDWORD attrs, LPDWORD error) {
        return _impl->GetFileAttributesOnBranch(filename, attrs, error);
    }

    BOOL BranchService::GetFilePath(LPWSTR path, size_t maxCount, int branchIndex, LPCWSTR filename) {
        return _impl->GetFilePath(path, maxCount, branchIndex, filename);
    }

    int BranchService::RWBranch() {
        return _impl->RWBranch();
    }

    // Private Implementation 
    BOOL BranchServiceImpl::AddBranch(LPCWSTR path, BranchAccessMode mode) {
        ISysService *sys = (ISysService*)GetService(Sys);
        ILoggerService *log = (ILoggerService*)GetService(Logger);

        // verify if branch already included
        auto itor = std::find_if(_branches.begin(), _branches.end(), 
            [&] (const BranchOption& o) {
                //FIXME: compare should be case insenstive
                //FIXME: verify if newPath isn't sub dir or parent dir of any existing paths.
                return o.path == path;
        }
        );
        if (itor != _branches.end()) {
            if (itor->mode == mode) {
                log->Error(L"branch already registered");
                return FALSE;// 
            }
            else {
                log->Error(L"branch already registered with different mode");
                return FALSE;//
            }
        }

        DWORD attr = sys->GetFileAttributes(path);
        if (attr == INVALID_FILE_ATTRIBUTES) {
            log->Error(L"path does not exist");
            return FALSE;
        }

        if ((attr & FILE_ATTRIBUTE_DIRECTORY) == 0) {
            log->Error(L"path is not a directory");
            return FALSE; 
        }

        //Note: the insert is an o(N) operation, an alternative would be
        //change all enumeration code to rbegin() and rend() 
        //and make code less readable. considering, AddBranch()
        //won't be a hot code path, I decide to accept it for now.
        _branches.insert(_branches.begin(), BranchOption(path, mode));
        return TRUE;
    }

    int BranchServiceImpl::GetFileAttributesOnBranch(LPCWSTR filename, LPDWORD pattrs, LPDWORD perror) {
        ISysService *sys = (ISysService*)GetService(Sys);

        DWORD attrs = 0, error = 0;
        std::wstring filePath;
        auto iter = std::find_if(_branches.begin(), _branches.end(), 
            [&] (const BranchOption& o) {
                filePath = o.path;
                appendPath(filePath, filename);
                attrs = sys->GetFileAttributes(filePath.c_str());
                error = sys->GetLastError();
                return attrs != INVALID_FILE_ATTRIBUTES;
        }
        );

        if (pattrs) *pattrs = attrs;
        if (perror) *perror = error;

        return iter == _branches.end() ? -1 : std::distance(_branches.begin(), iter);
    }

    BOOL BranchServiceImpl::GetFilePath(LPWSTR path, size_t maxCount, int branchIndex, LPCWSTR filename) {
        ILoggerService *logger = (ILoggerService*)GetService(Logger);
        if (branchIndex < 0 || branchIndex >= (int)_branches.size()) {
            logger->Error(L"Invalid branch %d", branchIndex);
            return FALSE;
        }

        std::wstring temp;
        temp += _branches[branchIndex].path;
        appendPath(temp, filename);

        if (temp.size() > maxCount) {
            logger->Error(L"Buffer too small");
            return FALSE;
        }

        wcscpy_s(path, maxCount, temp.c_str());
        return TRUE;
    }

    int BranchServiceImpl::RWBranch() {
        auto found = std::find_if(_branches.begin(), _branches.end(), 
            [](const BranchOption& o) {
                return o.mode == RW;
        }
        );
        return found == _branches.end() ? -1 : found - _branches.begin();
    }

    void BranchServiceImpl::appendPath(std::wstring &base, LPCWSTR path) {
        if (base.size() > 0 && base[base.size() - 1] != L'\\')
            base += L'\\';
        base += path;
    }

} //End of UnionFS namespace