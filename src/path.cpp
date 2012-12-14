#include "path.h"

#include <string>

void walkPath(LPCWSTR p, const std::function<WalkPathAction(LPCWSTR)>& callback) {
    std::wstring path(p);
    auto iter = path.begin();
    do {
        iter = std::find(iter, path.end(), L'\\');
        if (callback(std::wstring(path.begin(), iter).c_str()) == WalkStop)
            break;
        if (iter != path.end()) 
            iter++;
    } while (iter != path.end());
}