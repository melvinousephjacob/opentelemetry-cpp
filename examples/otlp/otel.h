#include "opentelemetry/exporters/otlp/otlp_http_exporter_factory.h"
#include "opentelemetry/exporters/otlp/otlp_http_log_record_exporter_factory.h"
#include "opentelemetry/exporters/otlp/otlp_http_log_record_exporter_options.h"
#include "opentelemetry/logs/provider.h"
#include "opentelemetry/logs/severity.h"
#include "opentelemetry/sdk/common/global_log_handler.h"
#include "opentelemetry/sdk/logs/logger_provider_factory.h"
#include "opentelemetry/sdk/logs/processor.h"
#include "opentelemetry/sdk/logs/simple_log_record_processor_factory.h"
#include "opentelemetry/sdk/logs/logger_provider.h"
#include "opentelemetry/sdk/trace/tracer_provider.h"
#include "opentelemetry/sdk/trace/processor.h"
#include "opentelemetry/sdk/trace/simple_processor_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#include "opentelemetry/trace/provider.h"
#include "opentelemetry/sdk/version/version.h"
#include "iloggingserver.h"
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <memory>
#include <iostream>

namespace trace     = opentelemetry::trace;
namespace otlp      = opentelemetry::exporter::otlp;
namespace logs_sdk  = opentelemetry::sdk::logs;
namespace logs      = opentelemetry::logs;
namespace trace_sdk = opentelemetry::sdk::trace;
namespace nostd = opentelemetry::nostd;
namespace common        = opentelemetry::common;

namespace otel
{
opentelemetry::exporter::otlp::OtlpHttpExporterOptions trace_opts;
void InitTracer()
{
  trace_opts.url = "http://opentelemetry-collector-lwshost:4318/v1/traces";
  std::cout << "Using " << trace_opts.url << " to export trace spans." << std::endl;
  // Create OTLP exporter instance
  auto exporter  = otlp::OtlpHttpExporterFactory::Create(trace_opts);
  auto processor = trace_sdk::SimpleSpanProcessorFactory::Create(std::move(exporter));
  std::shared_ptr<opentelemetry::trace::TracerProvider> provider =
      trace_sdk::TracerProviderFactory::Create(std::move(processor));
  // Set the global trace provider
  trace::Provider::SetTracerProvider(provider);
}

void CleanupTracer()
{
  // We call ForceFlush to prevent to cancel running exportings, It's optional.
  opentelemetry::nostd::shared_ptr<opentelemetry::trace::TracerProvider> provider =
      trace::Provider::GetTracerProvider();
  if (provider)
  {
    static_cast<trace_sdk::TracerProvider *>(provider.get())->ForceFlush();
  }

  std::shared_ptr<opentelemetry::trace::TracerProvider> none;
  trace::Provider::SetTracerProvider(none);
}

opentelemetry::exporter::otlp::OtlpHttpLogRecordExporterOptions logger_opts;
void InitLogger()
{
  logger_opts.url = "http://opentelemetry-collector-lwshost:4318/v1/logs";
  std::cout << "Using " << logger_opts.url << " to export log records." << std::endl;
  logger_opts.console_debug = true;
  // Create OTLP exporter instance
  auto exporter  = otlp::OtlpHttpLogRecordExporterFactory::Create(logger_opts);
  auto processor = logs_sdk::SimpleLogRecordProcessorFactory::Create(std::move(exporter));
  std::shared_ptr<logs::LoggerProvider> provider =
      logs_sdk::LoggerProviderFactory::Create(std::move(processor));

  opentelemetry::logs::Provider::SetLoggerProvider(provider);
}

void CleanupLogger()
{
  // We call ForceFlush to prevent to cancel running exportings, It's optional.
  opentelemetry::nostd::shared_ptr<logs::LoggerProvider> provider =
      logs::Provider::GetLoggerProvider();
  if (provider)
  {
    static_cast<logs_sdk::LoggerProvider *>(provider.get())->ForceFlush();
  }

  std::shared_ptr<logs::LoggerProvider> none;
  opentelemetry::logs::Provider::SetLoggerProvider(none);
}
nostd::shared_ptr<logs::Logger> get_logger(std::string _loggerName, std::string _nameSpace, std::string _className)
  {
    auto provider = logs::Provider::GetLoggerProvider();
    return provider->GetLogger(_loggerName, "Module Name", "1.0", "", opentelemetry::common::MakeAttributes({{"Namespace", _nameSpace}, {"ClassName", _className}, {"LogType", "MR_OTEL_CentralLog"}}));
  }

nostd::shared_ptr<logs::Logger> get_logger(std::string filename)
  {
    auto provider = logs::Provider::GetLoggerProvider();
    return provider->GetLogger("DeviceInfo", "Module Name", "1.0", "", opentelemetry::common::MakeAttributes({{"Filename", filename}, {"LogType", "MR_OTEL_DeviceInfo"}}));
  }

  nostd::shared_ptr<trace_api::Tracer> get_tracer(std::string _moduleName)
  {
    auto provider = trace::Provider::GetTracerProvider();
    return provider->GetTracer(_moduleName, OPENTELEMETRY_SDK_VERSION);
  }
}
