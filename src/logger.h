#include "unionfs.h"

namespace UnionFS
{
	class LoggerService : public ILoggerService {
	public:
		LoggerService();
		virtual ~LoggerService();
		void Error(LPCWSTR format, ...);
		void Warning(LPCWSTR format, ...);
		void Debug(LPCWSTR format, ...);

	protected:
		virtual void Log(LPCWSTR msg);
	};
}