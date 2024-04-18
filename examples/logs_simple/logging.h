#include "otel.h"
#include "iloggingserver.h"
namespace logging
{
	std::string _moduleName;
	std::string _version;
	class CentralLogServer : public ICentralLogServer
	{
		public:
			CentralLogServer(std::string moduleName, std::string version);
	
			~CentralLogServer();
	
			void Log(std::string message, Severity severity, CoreLogData coreLogData);
			void Log(std::string message, Severity severity, std::exception exception, CoreLogData coreLogData);
			void Log(std::string message, Severity severity, InfoCategory infoCategory, CoreLogData coreLogData);

			void LogWithSeverity(nostd::shared_ptr<logs::Logger> logger, Severity severity, std::unordered_map<std::string, std::string> mymap, std::string message);
	};
	
	CentralLogServer::CentralLogServer(std::string moduleName, std::string version)
	{
		otel::InitLogger();
		_moduleName = moduleName;
		_version = version;
	}
	
	CentralLogServer::~CentralLogServer()
	{
		otel::CleanupLogger();
	}
	
	void CentralLogServer::Log(std::string message, Severity severity, CoreLogData coreLogData)
	{
		auto logger = otel::get_logger(_moduleName, _version);
	
		std::unordered_map<std::string, std::string> mymap;
		mymap["NameSpace"] = coreLogData.Namespace;
			
		LogWithSeverity(logger, severity, mymap, message);
	}

	void LogWithSeverity(nostd::shared_ptr<logs::Logger> logger, Severity severity, std::unordered_map<std::string, std::string> mymap, std::string message)
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
	
	void CentralLogServer::Log(std::string message, Severity severity, std::exception exception, CoreLogData coreLogData)
	{
		auto logger = otel::get_logger(_moduleName, _version);
	
		std::unordered_map<std::string, std::string> mymap;
		mymap["ExceptionInfo"] = exception.what();

		LogWithSeverity(logger, severity, mymap, message);
	}
	
	void CentralLogServer::Log(std::string message, Severity severity, InfoCategory infoCategory, CoreLogData coreLogData)
	{
		auto logger = otel::get_logger(_moduleName, _version);
	
		std::unordered_map<std::string, std::string> mymap;
		mymap["InfoCategory"] = std::to_string(infoCategory);

		LogWithSeverity(logger, severity, mymap, message);
	}

}  // namespace
