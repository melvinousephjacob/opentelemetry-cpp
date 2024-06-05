#include "opentelemetry/exporters/ostream/metric_exporter_factory.h"
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

#include "imetricslogger.h"
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

#include <iostream>
#include <string>

#ifdef BAZEL_BUILD
#  include "examples/common/logs_foo_library/foo_library.h"
#else
#  include "logs_foo_library/foo_library.h"
#endif

namespace trace     = opentelemetry::trace;
namespace nostd = opentelemetry::nostd;

namespace metric_sdk    = opentelemetry::sdk::metrics;
namespace common        = opentelemetry::common;
namespace metrics_api   = opentelemetry::metrics;
namespace exportermetrics = opentelemetry::exporter::metrics;
namespace metrics_sdk     = opentelemetry::sdk::metrics;

namespace internal_log = opentelemetry::sdk::common::internal_log;

namespace otel_metrics
{
void InitMetrics(const std::string &name)
{
  auto exporter = exportermetrics::OStreamMetricExporterFactory::Create();

  std::string version{"1.2.0"};
  std::string schema{"https://opentelemetry.io/schemas/1.2.0"};

  // Initialize and set the global MeterProvider
  metrics_sdk::PeriodicExportingMetricReaderOptions options;
  options.export_interval_millis = std::chrono::milliseconds(1000);
  options.export_timeout_millis  = std::chrono::milliseconds(500);

  auto reader =
      metrics_sdk::PeriodicExportingMetricReaderFactory::Create(std::move(exporter), options);

  auto u_provider = metrics_sdk::MeterProviderFactory::Create();
  auto *p         = static_cast<metrics_sdk::MeterProvider *>(u_provider.get());

  p->AddMetricReader(std::move(reader));

  // counter view
  std::string counter_name = name + "_counter";
  std::string unit         = "counter-unit";

  auto instrument_selector = metrics_sdk::InstrumentSelectorFactory::Create(
      metrics_sdk::InstrumentType::kCounter, counter_name, unit);

  auto meter_selector = metrics_sdk::MeterSelectorFactory::Create(name, version, schema);

  auto sum_view = metrics_sdk::ViewFactory::Create(name, "description", unit,
                                                   metrics_sdk::AggregationType::kSum);

  p->AddView(std::move(instrument_selector), std::move(meter_selector), std::move(sum_view));

  // observable counter view
  std::string observable_counter_name = name + "_observable_counter";

  auto observable_instrument_selector = metrics_sdk::InstrumentSelectorFactory::Create(
      metrics_sdk::InstrumentType::kObservableCounter, observable_counter_name, unit);

  auto observable_meter_selector = metrics_sdk::MeterSelectorFactory::Create(name, version, schema);

  auto observable_sum_view = metrics_sdk::ViewFactory::Create(name, "test_description", unit,
                                                              metrics_sdk::AggregationType::kSum);

  p->AddView(std::move(observable_instrument_selector), std::move(observable_meter_selector),
             std::move(observable_sum_view));

  // histogram view
  std::string histogram_name = name + "_histogram";
  unit                       = "histogram-unit";

  auto histogram_instrument_selector = metrics_sdk::InstrumentSelectorFactory::Create(
      metrics_sdk::InstrumentType::kHistogram, histogram_name, unit);

  auto histogram_meter_selector = metrics_sdk::MeterSelectorFactory::Create(name, version, schema);

  auto histogram_aggregation_config = std::unique_ptr<metrics_sdk::HistogramAggregationConfig>(
      new metrics_sdk::HistogramAggregationConfig);

  histogram_aggregation_config->boundaries_ = std::vector<double>{
      0.0, 50.0, 100.0, 250.0, 500.0, 750.0, 1000.0, 2500.0, 5000.0, 10000.0, 20000.0};

  std::shared_ptr<metrics_sdk::AggregationConfig> aggregation_config(
      std::move(histogram_aggregation_config));

  auto histogram_view = metrics_sdk::ViewFactory::Create(
      name, "description", unit, metrics_sdk::AggregationType::kHistogram, aggregation_config);

  p->AddView(std::move(histogram_instrument_selector), std::move(histogram_meter_selector),
             std::move(histogram_view));

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
  auto double_observablecounter                         = meter->CreateDoubleObservableCounter(counter_name, propertyDescription);

  double_observablecounter->AddCallback(MeasurementFetcher::Fetcher, nullptr);

  for (uint32_t i = 0; i < 20; ++i)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  return double_observablecounter;
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
  auto double_observablegauge                         = meter->CreateDoubleObservableGauge(counter_name, propertyDescription);

  double_observablegauge->AddCallback(MeasurementFetcher::Fetcher, nullptr);

  for (uint32_t i = 0; i < 20; ++i)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  return double_observablegauge;
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
  auto double_observableupdowncounter                         = meter->CreateDoubleObservableUpDownCounter(counter_name, propertyDescription);

  double_observableupdowncounter->AddCallback(MeasurementFetcher::Fetcher, nullptr);

  for (uint32_t i = 0; i < 20; ++i)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  return double_observableupdowncounter;
}

}
