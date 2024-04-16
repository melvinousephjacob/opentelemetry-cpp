// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/exporters/ostream/span_exporter_factory.h"
#include "opentelemetry/sdk/trace/exporter.h"
#include "opentelemetry/sdk/trace/processor.h"
#include "opentelemetry/sdk/trace/simple_processor_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#include "opentelemetry/trace/provider.h"

#include "opentelemetry/exporters/ostream/log_record_exporter.h"
#include "opentelemetry/logs/provider.h"
#include "opentelemetry/sdk/logs/logger_provider_factory.h"
#include "opentelemetry/sdk/logs/processor.h"
#include "opentelemetry/sdk/logs/simple_log_record_processor_factory.h"

#include "opentelemetry/logs/provider.h"
#include "opentelemetry/logs/severity.h"
#include "iloggingserver.h"
#include <map>
#include <unordered_map>
#include <string>
#include "opentelemetry/logs/provider.h"
#include "opentelemetry/sdk/version/version.h"
#include "opentelemetry/trace/provider.h"
#include <chrono>
#include <thread>
//#define OPENTELEMETRY_STL_VERSION 2017

#ifdef BAZEL_BUILD
#  include "examples/common/logs_foo_library/foo_library.h"
#else
#  include "logs_foo_library/foo_library.h"
#endif

namespace logs_api      = opentelemetry::logs;
namespace logs_sdk      = opentelemetry::sdk::logs;
namespace logs_exporter = opentelemetry::exporter::logs;
namespace logs      = opentelemetry::logs;

namespace trace_api      = opentelemetry::trace;
namespace trace_sdk      = opentelemetry::sdk::trace;
namespace trace_exporter = opentelemetry::exporter::trace;
namespace nostd = opentelemetry::nostd;

namespace
{
void InitTracer()
{
  // Create ostream span exporter instance
  auto exporter  = trace_exporter::OStreamSpanExporterFactory::Create();
  auto processor = trace_sdk::SimpleSpanProcessorFactory::Create(std::move(exporter));
  std::shared_ptr<trace_api::TracerProvider> provider =
      trace_sdk::TracerProviderFactory::Create(std::move(processor));

  // Set the global trace provider
  trace_api::Provider::SetTracerProvider(provider);
}

void CleanupTracer()
{
  std::shared_ptr<trace_api::TracerProvider> none;
  trace_api::Provider::SetTracerProvider(none);
}

void InitLogger()
{
  // Create ostream log exporter instance
  auto exporter =
      std::unique_ptr<logs_sdk::LogRecordExporter>(new logs_exporter::OStreamLogRecordExporter);
  auto processor = logs_sdk::SimpleLogRecordProcessorFactory::Create(std::move(exporter));
  std::shared_ptr<logs_api::LoggerProvider> provider(
      logs_sdk::LoggerProviderFactory::Create(std::move(processor)));

  // Set the global logger provider
  logs_api::Provider::SetLoggerProvider(provider);
}

void CleanupLogger()
{
  std::shared_ptr<logs_api::LoggerProvider> none;
  logs_api::Provider::SetLoggerProvider(none);
}

nostd::shared_ptr<logs::Logger> get_logger()
{
  auto provider = logs::Provider::GetLoggerProvider();
  return provider->GetLogger("MR_DPC_Logger", "MR_DPC", "1.0");
}

std::map<Severity, opentelemetry::logs::Severity> severityMap = 
{
	{None, opentelemetry::logs::Severity::kInvalid},
	{DebugInfo, opentelemetry::logs::Severity::kDebug},
	{DebugVerbose, opentelemetry::logs::Severity::kDebug2},
	{Info, opentelemetry::logs::Severity::kInfo},
	{Warning, opentelemetry::logs::Severity::kWarn},
	{Error, opentelemetry::logs::Severity::kError},
	{Fatal, opentelemetry::logs::Severity::kFatal},
	{Hazard, opentelemetry::logs::Severity::kFatal4}
};

class CentralLogServer : public ICentralLogServer
{
	public:
		CentralLogServer();

		~CentralLogServer();

		void Log(std::string message, Severity severity, CoreLogData coreLogData);
		void Log(std::string message, Severity severity, std::exception exception, CoreLogData coreLogData);
		void Log(std::string message, Severity severity, InfoCategory infoCategory, CoreLogData coreLogData);
};

CentralLogServer::CentralLogServer()
{
	InitLogger();
	InitTracer();
}

CentralLogServer::~CentralLogServer()
{
	CleanupTracer();
	CleanupLogger();
}

void CentralLogServer::Log(std::string message, Severity severity, CoreLogData coreLogData)
{
	auto logger = get_logger();
	opentelemetry::logs::Severity otelSeverity;
	if(severityMap.find(severity) != severityMap.end())
		otelSeverity = severityMap[severity];
	else
		otelSeverity = opentelemetry::logs::Severity::kInvalid;

	std::unordered_map<std::string, std::string> mymap = {{ "First", "first" },
                                    { "Second","second" }
                                    { "Third", "third" }
                                                 }; 

	logger.Debug("Outputting a map ", mymap);
	//logger->Log(otelSeverity, message + " " + coreLogData.Resolution + " " + "Attributes: {"Modulename": "SampleModuleName", "Namespace": "SampleNamespace"});
}

void CentralLogServer::Log(std::string message, Severity severity, std::exception exception, CoreLogData coreLogData)
{
	foo_library();
}

void CentralLogServer::Log(std::string message, Severity severity, InfoCategory infoCategory, CoreLogData coreLogData)
{
	foo_library();
}

}  // namespace

int main()
{
  auto centralLogServer = new CentralLogServer;
  CoreLogData coreLogData;
  coreLogData.Resolution = "Example core log data";
  
  centralLogServer->Log("This is a sample log message", DebugInfo, coreLogData);
  centralLogServer->Log("HELLO", Fatal, coreLogData);
}
