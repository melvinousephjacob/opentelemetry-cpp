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
		//foo_library();
	}
	
	void CentralLogServer::Log(std::string message, Severity severity, InfoCategory infoCategory, CoreLogData coreLogData)
	{
		//foo_library();
	}

}  // namespace
