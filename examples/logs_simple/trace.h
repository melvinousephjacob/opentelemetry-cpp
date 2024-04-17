#include "otel.h"
#include "itracelogger.h"
namespace trace
{
	std::string _moduleName;

	class TraceLogger : public ITraceLogger
	{
		public:
			TraceLogger();
	
			~TraceLogger();
	
			void Log(std::string message, Severity severity, CoreLogData coreLogData);
			void Log(std::string message, Severity severity, std::exception exception, CoreLogData coreLogData);
			void Log(std::string message, Severity severity, InfoCategory infoCategory, CoreLogData coreLogData);
	};
	
	TraceLogger::TraceLogger()
	{
		otel::InitTracer();
	}
	
	TraceLogger::~TraceLogger()
	{
		otel::CleanupTracer();
	}
	
	void CentralLogServer::Log(std::string message, Severity severity, CoreLogData coreLogData)
	{
		auto tracer = otel::get_tracer(_moduleName);
	
		auto scoped_span = trace::Scope(tracer->StartSpan(_moduleName));
	}
	
	void CentralLogServer::Log(std::string message, Severity severity, std::exception exception, CoreLogData coreLogData)
	{
		//foo_library();
	}
	
	void CentralLogServer::Log(std::string message, Severity severity, InfoCategory infoCategory, CoreLogData coreLogData)
	{
		//foo_library();
	}

}
