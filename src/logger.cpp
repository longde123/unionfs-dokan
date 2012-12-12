#include "logger.h"

#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include <string>
#include "tchar.h"


namespace UnionFS {

LoggerService::LoggerService(){ }
LoggerService::~LoggerService() { }

ILoggerService::~ILoggerService() {}


// copy prefix to msg, return pointer advance prefix
template<size_t _Size>
static WCHAR* appendPrefix(WCHAR (&msg)[_Size], LPCWSTR prefix) {
	wcscpy_s(msg, prefix);
	return msg + wcslen(prefix);
}

void LoggerService::Error(LPCWSTR format, ...) {
	WCHAR msg[512];
	WCHAR *next = appendPrefix(msg, _T("Error:"));

	va_list args;
	va_start(args, format);
	vswprintf_s(next, msg + 512 - next, format, args);
	va_end(args);

	Log(msg);
}
void LoggerService::Warning(LPCWSTR format, ...) {
	WCHAR msg[512];
	LPWSTR next = appendPrefix(msg, _T("Warning:"));
	va_list args;
	va_start(args, format);
	vswprintf_s(next, msg + 512 - next, format, args);
	va_end(args);
	Log(msg);
}

void LoggerService::Debug(LPCWSTR format, ...) {
	WCHAR msg[512];
	LPWSTR next = appendPrefix(msg, _T("Debug:"));
	va_list args;
	va_start(args, format);
	vswprintf_s(next, msg + 512 - next, format, args);
	va_end(args);
	Log(msg);
}

void LoggerService::Log(LPCWSTR msg) {
	fwprintf_s(stderr, msg);
}


}

