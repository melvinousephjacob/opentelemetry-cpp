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

#include "opentelemetry/metrics/provider.h"
#include "opentelemetry/sdk/metrics/aggregation/default_aggregation.h"
#include "opentelemetry/sdk/metrics/aggregation/histogram_aggregation.h"
#include "opentelemetry/sdk/metrics/export/periodic_exporting_metric_reader_factory.h"
#include "opentelemetry/sdk/metrics/meter.h"
#include "opentelemetry/sdk/metrics/meter_provider.h"
#include "opentelemetry/sdk/metrics/meter_provider_factory.h"
#include "opentelemetry/sdk/metrics/push_metric_exporter.h"
#include "opentelemetry/sdk/metrics/view/instrument_selector_factory.h"
#include "opentelemetry/sdk/metrics/view/meter_selector_factory.h"
#include "opentelemetry/sdk/metrics/view/view_factory.h"
#include <chrono>
#include <thread>

#include "ihistogramlogger.h"
#include <chrono>
#include <map>
#include <memory>
#include <thread>
#include <vector>
#include "opentelemetry/context/context.h"
#include "opentelemetry/metrics/provider.h"
#include "opentelemetry/nostd/shared_ptr.h"

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

namespace metrics_sdk    = opentelemetry::sdk::metrics;
namespace common        = opentelemetry::common;
namespace metrics_api   = opentelemetry::metrics;

namespace internal_log = opentelemetry::sdk::common::internal_log;

namespace otel_metrics
{
void InitMetrics(const std::string &name)
{
  opentelemetry::exporter::otlp::OtlpHttpMetricExporterOptions exporter_options;
  exporter_options.url = "http://opentelemetry-collector-lwshost:4318/v1/metrics";
  auto exporter = opentelemetry::exporter::otlp::OtlpHttpMetricExporterFactory::Create(exporter_options);

  std::string version{"1.2.0"};
  std::string schema{"https://opentelemetry.io/schemas/1.2.0"};

  // Initialize and set the global MeterProvider
  metrics_sdk::PeriodicExportingMetricReaderOptions reader_options;
  reader_options.export_interval_millis = std::chrono::milliseconds(1000);
  reader_options.export_timeout_millis  = std::chrono::milliseconds(500);

  auto reader =
      metrics_sdk::PeriodicExportingMetricReaderFactory::Create(std::move(exporter), reader_options);

  auto context = metrics_sdk::MeterContextFactory::Create();
  context->AddMetricReader(std::move(reader));

  auto u_provider = metrics_sdk::MeterProviderFactory::Create(std::move(context));
  std::shared_ptr<opentelemetry::metrics::MeterProvider> provider(std::move(u_provider));

  metrics_api::Provider::SetMeterProvider(provider);
}

void CleanupMetrics()
{
  std::shared_ptr<metrics_api::MeterProvider> none;
  metrics_api::Provider::SetMeterProvider(none);
}

std::map<std::string, std::string> get_random_attr()
{
  const std::vector<std::pair<std::string, std::string>> labels = {{"key1", "value1"},
                                                                   {"key2", "value2"},
                                                                   {"key3", "value3"},
                                                                   {"key4", "value4"},
                                                                   {"key5", "value5"}};
  return std::map<std::string, std::string>{labels[rand() % (labels.size() - 1)],
                                            labels[rand() % (labels.size() - 1)]};
}

class MeasurementFetcher
{
public:
  static void Fetcher(opentelemetry::metrics::ObserverResult observer_result, void * /* state */)
  {
    if (nostd::holds_alternative<
            nostd::shared_ptr<opentelemetry::metrics::ObserverResultT<double>>>(observer_result))
    {
      double random_incr = (rand() % 5) + 1.1;
      value_ += random_incr;
      std::map<std::string, std::string> labels = get_random_attr();
      nostd::get<nostd::shared_ptr<opentelemetry::metrics::ObserverResultT<double>>>(
          observer_result)
          ->Observe(value_, labels);
    }
  }
  static double value_;
};
double MeasurementFetcher::value_ = 0.0;

nostd::unique_ptr<opentelemetry::metrics::Histogram<uint64_t>> get_histogram(std::string fruName, std::string propertyName, unsigned int historyLength, unsigned int numberOfBins, unsigned int binWidth, int min, int max, std::string siUnit)
{
  std::string attributes = "FRUName: " +fruName+ ", PropertyName: " +propertyName+ ", HistoryLength: " +std::to_string(historyLength)+ ", NumberOfBins: " +std::to_string(numberOfBins)+ ", BinWidth: " +std::to_string(binWidth)+ ", Min: " +std::to_string(min)+ ", Max: " +std::to_string(max);
  std::string name                    = "MR_OTEL_Histogram";
  auto provider                               = metrics_api::Provider::GetMeterProvider();
  nostd::shared_ptr<metrics_api::Meter> meter = provider->GetMeter(name, "1.2.0");
  auto int_histogram                         = meter->CreateUInt64Histogram(name, attributes, siUnit);

  return int_histogram;
}

nostd::shared_ptr<opentelemetry::metrics::ObservableInstrument> get_observablegauge(std::string fruName, std::string propertyName, std::string propertyDescription, int historyL, bool timeStampProvided, opentelemetry::metrics::ObservableCallbackPtr callback)
{
  std::string attributes = "FRUName: " +fruName+ ", PropertyName: " +propertyName+ ", PropertyDescription: " +propertyDescription+ ", HistoryLength: " +std::to_string(historyL)+ ", TimeStampProvided: " +std::to_string(timeStampProvided);
  std::string name                    = "MR_OTEL_DeviceProperty";
  auto provider                               = metrics_api::Provider::GetMeterProvider();
  nostd::shared_ptr<metrics_api::Meter> meter = provider->GetMeter(name, "1.2.0");
  auto double_observablegauge                         = meter->CreateDoubleObservableGauge(name, attributes);

  double_observablegauge->AddCallback(callback, nullptr);

  return double_observablegauge;
}

}
