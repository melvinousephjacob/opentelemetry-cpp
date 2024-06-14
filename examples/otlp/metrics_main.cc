// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0
#include "metrics.h"

int main()
{
  std::string metrics_exporter_url = "http://opentelemetry-collector-lwshost:4318/v1/metrics";

  auto counter = new MetricsCounter("Sample_FRU_Name", "Sample_Property_Name", "This is a sample property description", metrics_exporter_url);
  //MetricsCounter x;
  //std::thread counter_example (&MetricsCounter::Log, std::ref(x));
  counter->Log();

  auto observableCounter = new MetricsObservableCounter("Sample_ObservableCounter", "Sample_Property_Name", "This is a sample observable counter", metrics_exporter_url);

  auto observableGauge = new MetricsObservableGauge("Sample_ObservableGauge", "Sample_Property_Name", "This is a sample observable gauge", otel_metrics::MeasurementFetcher::Fetcher, metrics_exporter_url);

  for (uint32_t i = 0; i < 20; ++i)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  }

  auto updowncounter = new MetricsUpDownCounter("Sample_UpDownCounter", "Sample_Property_Name", "This is a sample up down counter", metrics_exporter_url);
  updowncounter->Log();

  auto observableUpDownCounter = new MetricsObservableUpDownCounter("Sample_ObservableUpDownCounter", "Sample_Property_Name", "This is a sample observable up down counter", metrics_exporter_url);

  auto histogram = new MetricsHistogram("Sample_Histogram", "Sample_Property_Name", "This is a sample histogram", metrics_exporter_url);
  //metrics::MetricsHistogram y;
  //std::thread counter_example{&foo_library::counter_example, name};
  //std::thread observable_counter_example{&foo_library::observable_counter_example, name};
  //std::thread histogram_example{&MetricsHistogram::Log};

  //counter_example.join();
  //observable_counter_example.join();
  //histogram_example.join();
  histogram->Log();
}
