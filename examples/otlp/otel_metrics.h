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
#include <chrono>
#include <map>
#include <memory>
#include <thread>
#include <vector>
#include "opentelemetry/context/context.h"
#include "opentelemetry/metrics/provider.h"
#include "opentelemetry/nostd/shared_ptr.h"

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

namespace otel_metrics
{
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

nostd::unique_ptr<opentelemetry::metrics::Counter<uint64_t>> get_counter(std::string fruName, std::string propertyName, std::string propertyDescription)
{
  std::string counter_name                    = fruName + "_" + propertyName + "_counter";
  auto provider                               = metrics_api::Provider::GetMeterProvider();
  nostd::shared_ptr<metrics_api::Meter> meter = provider->GetMeter(fruName, "1.2.0");
  auto int_counter                         = meter->CreateUInt64Counter(counter_name, propertyDescription);

  return int_counter;
}

nostd::shared_ptr<opentelemetry::metrics::ObservableInstrument> get_observablecounter(std::string fruName, std::string propertyName, std::string propertyDescription)
{
  std::string counter_name                    = fruName + "_" + propertyName + "_counter";
  auto provider                               = metrics_api::Provider::GetMeterProvider();
  nostd::shared_ptr<metrics_api::Meter> meter = provider->GetMeter(fruName, "1.2.0");
  auto int_observablecounter                         = meter->CreateInt64ObservableCounter(counter_name, propertyDescription);

  return int_observablecounter;
}

nostd::unique_ptr<opentelemetry::metrics::Histogram<uint64_t>> get_histogram(std::string fruName, std::string propertyName, std::string propertyDescription)
{
  std::string counter_name                    = fruName + "_" + propertyName + "_counter";
  auto provider                               = metrics_api::Provider::GetMeterProvider();
  nostd::shared_ptr<metrics_api::Meter> meter = provider->GetMeter(fruName, "1.2.0");
  auto int_histogram                         = meter->CreateUInt64Histogram(counter_name, propertyDescription);

  return int_histogram;
}

nostd::shared_ptr<opentelemetry::metrics::ObservableInstrument> get_observablegauge(std::string fruName, std::string propertyName, std::string propertyDescription)
{
  std::string counter_name                    = fruName + "_" + propertyName + "_counter";
  auto provider                               = metrics_api::Provider::GetMeterProvider();
  nostd::shared_ptr<metrics_api::Meter> meter = provider->GetMeter(fruName, "1.2.0");
  auto int_observablegauge                         = meter->CreateInt64ObservableGauge(counter_name, propertyDescription);

  return int_observablegauge;
}

nostd::unique_ptr<opentelemetry::metrics::UpDownCounter<int64_t>> get_updowncounter(std::string fruName, std::string propertyName, std::string propertyDescription)
{
  std::string counter_name                    = fruName + "_" + propertyName + "_counter";
  auto provider                               = metrics_api::Provider::GetMeterProvider();
  nostd::shared_ptr<metrics_api::Meter> meter = provider->GetMeter(fruName, "1.2.0");
  auto int_updowncounter                         = meter->CreateInt64UpDownCounter(counter_name, propertyDescription);

  return int_updowncounter;
}

nostd::shared_ptr<opentelemetry::metrics::ObservableInstrument> get_observableupdowncounter(std::string fruName, std::string propertyName, std::string propertyDescription)
{
  std::string counter_name                    = fruName + "_" + propertyName + "_counter";
  auto provider                               = metrics_api::Provider::GetMeterProvider();
  nostd::shared_ptr<metrics_api::Meter> meter = provider->GetMeter(fruName, "1.2.0");
  auto int_observableupdowncounter                         = meter->CreateInt64ObservableUpDownCounter(counter_name, propertyDescription);

  return int_observableupdowncounter;
}

}
