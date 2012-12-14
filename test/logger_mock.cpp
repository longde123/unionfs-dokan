#include "logger_mock.h"

#include "yaffut.h"

namespace UnionFS {

    LoggerMock::LoggerMock() : _enabled(true) { }

    void LoggerMock::Enable() {
        _enabled = true;
    }

    void LoggerMock::Disable() {
        _enabled = false;
    }

    void LoggerMock::Log(LPCWSTR msg) {
        _actuals.push_back(msg);
    }

    void LoggerMock::Expect(LPCWSTR msg) {
        _expects.push_back(msg);
    }

    static std::string narrow(const std::wstring& wide) {
        return std::string(wide.begin(), wide.end());
    }

    LoggerMock::~LoggerMock() {
        if (!_enabled)
            return;

        auto actual = _actuals.begin();
        auto expect = _expects.begin();

        while (actual != _actuals.end() && expect != _expects.end()) {
            YAFFUT_EQUAL(narrow(*expect), narrow(*actual));
            expect++;
            actual++;
        }

        if (actual != _actuals.end()) {
            YAFFUT_FAIL("have unexpected log outputs");
        }
        if (expect != _expects.end()) {
            YAFFUT_FAIL("still have unsatisfied expected outputs");
        }
    }

}