#include "unionfs.h"
#include "logger.h"

#include <string>
#include <list>


namespace UnionFS {

class LoggerMock : public LoggerService {
	typedef std::list<std::wstring> wstringlist;
	wstringlist _expects;
	wstringlist _actuals;
public:
	~LoggerMock();
	void Log(LPCWSTR msg);
	void Expect(LPCWSTR msg);
};

}