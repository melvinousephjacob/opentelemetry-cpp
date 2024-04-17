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

namespace otel
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
  
  nostd::shared_ptr<logs::Logger> get_logger(std::string _moduleName, std::string _version)
  {
    auto provider = logs::Provider::GetLoggerProvider();
    return provider->GetLogger("MR_DPC_Logger", _moduleName, _version);
  }

  nostd::shared_ptr<trace_api::Tracer> get_tracer(std::string _moduleName)
  {
    auto provider = trace_api::Provider::GetTracerProvider();
    return provider->GetTracer(_moduleName, OPENTELEMETRY_SDK_VERSION);
  }
}
