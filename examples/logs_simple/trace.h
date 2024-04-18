#include "logging.h"
#include "itracelogger.h"
#include <sstream>
namespace trace
{
	std::string _moduleName;

	class TraceLogger : public ITraceLogger
	{
		public:
			TraceLogger(std::string moduleName);
	
			~TraceLogger();
	
			void TraceInfo(std::string message, std::exception exception);
		        void TraceInfo(std::string message, TraceData traceData);
		        void TraceInfo(std::string message);
		        void TraceVerbose(std::string message);
		        void TraceVerbose(std::string message, TraceData traceData);
		        void TraceVerbose(std::string message, std::exception exception);
	};
	
	TraceLogger::TraceLogger(std::string moduleName)
	{
		otel::InitTracer();
		_moduleName = moduleName;
	}
	
	TraceLogger::~TraceLogger()
	{
		otel::CleanupTracer();
	}
	
	void TraceLogger::TraceInfo(std::string message)
	{
		auto tracer = otel::get_tracer(_moduleName);

		std::unordered_map<std::string, std::string> mymap;
		mymap["description"] = message;
		mymap["NameSpace"] = "Sample namespace";
		auto scoped_span = opentelemetry::trace::Scope(tracer->StartSpan(_moduleName, mymap));
	}

	void TraceLogger::TraceInfo(std::string message, std::exception exception)
	{
		auto tracer = otel::get_tracer(_moduleName);

		std::unordered_map<std::string, std::string> mymap;
		mymap["description"] = message;
		mymap["NameSpace"] = "Sample namespace";
		mymap["ExceptionInfo"] = exception.what();
		auto scoped_span = opentelemetry::trace::Scope(tracer->StartSpan(_moduleName, mymap));
	}

	void TraceLogger::TraceInfo(std::string message, TraceData traceData)
	{
		auto tracer = otel::get_tracer(_moduleName);

		std::unordered_map<std::string, std::string> mymap;
		mymap["description"] = message;
		mymap["NameSpace"] = "Sample namespace";
		mymap["AdditionalInfo"] = traceData.AdditionalInfo.empty() ? traceData.AdditionalInfo: "";
		mymap["ContextInfo"] = traceData.ContextInfo.empty() ? traceData.ContextInfo: "";

		std::stringstream ss;
		ss << traceData.DateTime;
		std::string ts = ss.str();
		mymap["DateTime"] = ts.empty() ? ts: "";
		mymap["EventType"] = traceData.EventType.empty() ? traceData.EventType: "";
		mymap["ExceptionInfo"] = traceData.ExceptionInfo.empty() ? traceData.ExceptionInfo: "";
		mymap["StackTrace"] = traceData.StackTrace.empty() ? traceData.StackTrace: "";
		auto scoped_span = opentelemetry::trace::Scope(tracer->StartSpan(_moduleName, mymap));
	}

	void TraceLogger::TraceVerbose(std::string message)
	{
		auto tracer = otel::get_tracer(_moduleName);

		std::unordered_map<std::string, std::string> mymap;
		mymap["description"] = message;
		mymap["NameSpace"] = "Sample namespace";
		auto scoped_span = opentelemetry::trace::Scope(tracer->StartSpan(_moduleName, mymap));
	}

	void TraceLogger::TraceVerbose(std::string message, TraceData traceData)
	{
		auto tracer = otel::get_tracer(_moduleName);

		std::unordered_map<std::string, std::string> mymap;
		mymap["description"] = message;
		mymap["NameSpace"] = "Sample namespace";
		mymap["AdditionalInfo"] = traceData.AdditionalInfo.empty() ? traceData.AdditionalInfo: "";
		mymap["ContextInfo"] = traceData.ContextInfo.empty() ? traceData.ContextInfo: "";

		std::stringstream ss;
		ss << traceData.DateTime;
		std::string ts = ss.str();
		mymap["DateTime"] = ts.empty() ? ts: "";
		mymap["EventType"] = traceData.EventType.empty() ? traceData.EventType: "";
		mymap["ExceptionInfo"] = traceData.ExceptionInfo.empty() ? traceData.ExceptionInfo: "";
		mymap["StackTrace"] = traceData.StackTrace.empty() ? traceData.StackTrace: "";
		auto scoped_span = opentelemetry::trace::Scope(tracer->StartSpan(_moduleName, mymap));
	}

	void TraceLogger::TraceVerbose(std::string message, std::exception exception)
	{
		auto tracer = otel::get_tracer(_moduleName);

		std::unordered_map<std::string, std::string> mymap;
		mymap["description"] = message;
		mymap["NameSpace"] = "Sample namespace";
		mymap["ExceptionInfo"] = exception.what();
		auto scoped_span = opentelemetry::trace::Scope(tracer->StartSpan(_moduleName, mymap));
	}
}
