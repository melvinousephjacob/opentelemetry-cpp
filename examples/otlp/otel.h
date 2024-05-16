#include "opentelemetry/exporters/otlp/otlp_http_exporter_factory.h"
#include "opentelemetry/exporters/otlp/otlp_http_log_record_exporter_factory.h"
#include "opentelemetry/exporters/otlp/otlp_http_log_record_exporter_options.h"
#include "opentelemetry/logs/provider.h"
#include "opentelemetry/logs/severity.h"
#include "opentelemetry/sdk/common/global_log_handler.h"
#include "opentelemetry/sdk/logs/logger_provider_factory.h"
#include "opentelemetry/sdk/logs/processor.h"
#include "opentelemetry/sdk/logs/simple_log_record_processor_factory.h"
#include "opentelemetry/sdk/trace/processor.h"
#include "opentelemetry/sdk/trace/simple_processor_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#include "opentelemetry/trace/provider.h"
#include "iloggingserver.h"
#include "imetricslogger.h"
#include <map>
#include <string>
#include "opentelemetry/logs/provider.h"
#include "opentelemetry/sdk/version/version.h"
#include "opentelemetry/trace/provider.h"
#include <chrono>
#include <thread>

#include "opentelemetry/exporters/otlp/otlp_http_metric_exporter_factory.h"
#include "opentelemetry/exporters/otlp/otlp_http_metric_exporter_options.h"
#include "opentelemetry/metrics/provider.h"
#include "opentelemetry/sdk/common/global_log_handler.h"
#include "opentelemetry/sdk/metrics/aggregation/default_aggregation.h"
#include "opentelemetry/sdk/metrics/export/periodic_exporting_metric_reader.h"
#include "opentelemetry/sdk/metrics/export/periodic_exporting_metric_reader_factory.h"
#include "opentelemetry/sdk/metrics/meter.h"
#include "opentelemetry/sdk/metrics/meter_context_factory.h"
#include "opentelemetry/sdk/metrics/meter_provider.h"
#include "opentelemetry/sdk/metrics/meter_provider_factory.h"

#include <memory>
#include <thread>

// sdk::TracerProvider and sdk::LoggerProvider is just used to call ForceFlush and prevent to cancel
// running exportings when destroy and shutdown exporters.It's optional to users.
#include "opentelemetry/sdk/logs/logger_provider.h"
#include "opentelemetry/sdk/trace/tracer_provider.h"

#include <iostream>
#include <string>

#ifdef BAZEL_BUILD
#  include "examples/common/logs_foo_library/foo_library.h"
#else
#  include "logs_foo_library/foo_library.h"
#endif

namespace trace     = opentelemetry::trace;
namespace otlp      = opentelemetry::exporter::otlp;
namespace logs_sdk  = opentelemetry::sdk::logs;
namespace logs      = opentelemetry::logs;
namespace trace_sdk = opentelemetry::sdk::trace;
namespace nostd = opentelemetry::nostd;

namespace metric_sdk    = opentelemetry::sdk::metrics;
namespace common        = opentelemetry::common;
namespace metrics_api   = opentelemetry::metrics;

namespace internal_log = opentelemetry::sdk::common::internal_log;

namespace otel
{
//opentelemetry::exporter::otlp::OtlpHttpExporterOptions trace_opts;
void InitTracer(opentelemetry::exporter::otlp::OtlpHttpExporterOptions trace_opts)
{
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

//opentelemetry::exporter::otlp::OtlpHttpLogRecordExporterOptions logger_opts;
void InitLogger(opentelemetry::exporter::otlp::OtlpHttpLogRecordExporterOptions logger_opts)
{
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

void InitMetrics(opentelemetry::exporter::otlp::OtlpHttpMetricExporterOptions exporter_options)
{
  auto exporter = opentelemetry::exporter::otlp::OtlpHttpMetricExporterFactory::Create(exporter_options);

  std::string version{"1.2.0"};
  std::string schema{"https://opentelemetry.io/schemas/1.2.0"};

  // Initialize and set the global MeterProvider
  metric_sdk::PeriodicExportingMetricReaderOptions reader_options;
  reader_options.export_interval_millis = std::chrono::milliseconds(1000);
  reader_options.export_timeout_millis  = std::chrono::milliseconds(500);

  auto reader =
      metric_sdk::PeriodicExportingMetricReaderFactory::Create(std::move(exporter), reader_options);

  auto context = metric_sdk::MeterContextFactory::Create();
  context->AddMetricReader(std::move(reader));

  auto u_provider = metric_sdk::MeterProviderFactory::Create(std::move(context));
  std::shared_ptr<opentelemetry::metrics::MeterProvider> provider(std::move(u_provider));

  metrics_api::Provider::SetMeterProvider(provider);
}

void CleanupMetrics()
{
  std::shared_ptr<metrics_api::MeterProvider> none;
  metrics_api::Provider::SetMeterProvider(none);
}

nostd::shared_ptr<logs::Logger> get_logger(std::string _loggerName, std::string _nameSpace, std::string _className)
  {
    auto provider = logs::Provider::GetLoggerProvider();
    return provider->GetLogger(_loggerName, "Module Name", "1.0", "", opentelemetry::common::MakeAttributes({{"Namespace", _nameSpace}, {"ClassName", _className}}));
  }

  nostd::shared_ptr<trace_api::Tracer> get_tracer(std::string _moduleName)
  {
    auto provider = trace::Provider::GetTracerProvider();
    return provider->GetTracer(_moduleName, OPENTELEMETRY_SDK_VERSION);
  }

nostd::unique_ptr<opentelemetry::metrics::Counter<uint64_t>> get_meter(std::string fruName, std::string propertyName, std::string propertyDescription)
{
  std::string counter_name                    = fruName + "_" + propertyName + "_counter";
  auto provider                               = metrics_api::Provider::GetMeterProvider();
  nostd::shared_ptr<metrics_api::Meter> meter = provider->GetMeter(fruName, "1.2.0");
  auto int_counter                         = meter->CreateUInt64Counter(counter_name, propertyDescription);

  return int_counter;
}

}
