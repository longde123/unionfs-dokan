#include <Windows.h>
#include <functional>

enum WalkPathAction {
     WalkContinue,
     WalkStop,
};
void walkPath(LPCWSTR path, const std::function<WalkPathAction(LPCWSTR)>& f);