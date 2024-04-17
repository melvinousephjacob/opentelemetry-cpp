// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "otel.h"
namespace
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
		otel::InitTracer();
		_moduleName = moduleName;
		_version = version;
	}
	
	CentralLogServer::~CentralLogServer()
	{
		otel::CleanupTracer();
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

int main()
{
  auto centralLogServer = new CentralLogServer("Sample module", "1.0");
  CoreLogData coreLogData;
  coreLogData.Namespace = "Sample Namespace";
  
  centralLogServer->Log("This is a sample log message", DebugInfo, coreLogData);
  centralLogServer->Log("HELLO", Fatal, coreLogData);
}
