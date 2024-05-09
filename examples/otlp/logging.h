#include "otel.h"
#include "iloggingserver.h"
namespace logging
{
	std::string _moduleName;
	std::string _version;
	nostd::shared_ptr<logs::Logger> logger;
	class CentralLogServer : public ICentralLogServer
	{
		public:
			CentralLogServer(std::string loggerName, std::string nameSpace, std::string className, opentelemetry::exporter::otlp::OtlpHttpLogRecordExporterOptions logger_opts);
	
			~CentralLogServer();
	
			void Log(std::string message, Severity severity);
			void Log(std::string message, Severity severity, std::exception exception);
			void Log(std::string message, Severity severity, InfoCategory infoCategory);
		private:
			void LogWithSeverity(nostd::shared_ptr<logs::Logger> logger, Severity severity, std::unordered_map<std::string, std::string> mymap, std::string message);
	};
	
	CentralLogServer::CentralLogServer(std::string loggerName, std::string nameSpace, std::string className, opentelemetry::exporter::otlp::OtlpHttpLogRecordExporterOptions logger_opts)
	{
		otel::InitLogger(logger_opts);
		logger = otel::get_logger(loggerName, nameSpace, className);
	}
	
	CentralLogServer::~CentralLogServer()
	{
		otel::CleanupLogger();
	}

	void CentralLogServer::LogWithSeverity(nostd::shared_ptr<logs::Logger> logger, Severity severity, std::unordered_map<std::string, std::string> mymap, std::string message)
	{
		if(severity == Info)
			logger->Info(message, mymap);
		else if(severity == Warning)
			logger->Warn(message, mymap);
		else if(severity == Error)
			logger->Error(message, mymap);
		else if(severity == Fatal || severity == Hazard)
			logger->Fatal(message, mymap);
		else
			logger->Debug(message, mymap);
	}
	
	void CentralLogServer::Log(std::string message, Severity severity)
	{
		//auto logger = otel::get_logger(_moduleName, _version);
	
		std::unordered_map<std::string, std::string> mymap;
		//mymap["NameSpace"] = coreLogData.Namespace;
			
		CentralLogServer::LogWithSeverity(logger, severity, mymap, message);
	}
	
	void CentralLogServer::Log(std::string message, Severity severity, std::exception exception)
	{
		//auto logger = otel::get_logger(_moduleName, _version);
	
		std::unordered_map<std::string, std::string> mymap;
		mymap["ExceptionInfo"] = exception.what();

		CentralLogServer::LogWithSeverity(logger, severity, mymap, message);
	}
	
	void CentralLogServer::Log(std::string message, Severity severity, InfoCategory infoCategory)
	{
		//auto logger = otel::get_logger(_moduleName, _version);
	
		std::unordered_map<std::string, std::string> mymap;
		mymap["InfoCategory"] = std::to_string(infoCategory);

		CentralLogServer::LogWithSeverity(logger, severity, mymap, message);
	}

}  // namespace
