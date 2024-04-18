#include "logging.h"
#include "itracelogger.h"
namespace trace
{
	std::string _moduleName;

	class TraceLogger : public ITraceLogger
	{
		public:
			TraceLogger(std::string moduleName);
	
			~TraceLogger();
	
			void TraceInfo(std::string message, std::exception exception);
		        void TraceInfo(std::string message, TraceData data);
		        void TraceInfo(std::string message);
		        void TraceVerbose(std::string message);
		        void TraceVerbose(std::string message, TraceData data);
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

	void TraceLogger::TraceInfo(std::string message, TraceData data)
	{
		auto tracer = otel::get_tracer(_moduleName);

		std::unordered_map<std::string, std::string> mymap;
		mymap["description"] = message;
		mymap["NameSpace"] = "Sample namespace";
		mymap["ExceptionInfo"] = exception.what();
		auto scoped_span = opentelemetry::trace::Scope(tracer->StartSpan(_moduleName, mymap));
	}

	void TraceLogger::TraceVerbose(std::string message)
	{
		auto tracer = otel::get_tracer(_moduleName);

		std::unordered_map<std::string, std::string> mymap;
		mymap["description"] = message;
		mymap["NameSpace"] = "Sample namespace";
		mymap["ExceptionInfo"] = exception.what();
		auto scoped_span = opentelemetry::trace::Scope(tracer->StartSpan(_moduleName, mymap));
	}

	void TraceLogger::TraceVerbose(std::string message, TraceData data)
	{
		auto tracer = otel::get_tracer(_moduleName);

		std::unordered_map<std::string, std::string> mymap;
		mymap["description"] = message;
		mymap["NameSpace"] = "Sample namespace";
		mymap["ExceptionInfo"] = exception.what();
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
