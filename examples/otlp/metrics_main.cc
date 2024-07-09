// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0
#include "metrics.h"

int main()
{
  auto observableGauge = new DevicePropertyLogger("Sample_ObservableGauge", "Sample_Property_Name", "This is a sample observable gauge", 30, false, otel_metrics::MeasurementFetcher::Fetcher, metrics_exporter_url);

  for (uint32_t i = 0; i < 20; ++i)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  auto histogram = new HistogramLogger("Sample_Histogram", "Sample_Property_Name", 30, 5, 2, 0, 20, "m");
  //metrics::MetricsHistogram y;
  //std::thread counter_example{&foo_library::counter_example, name};
  //std::thread observable_counter_example{&foo_library::observable_counter_example, name};
  //std::thread histogram_example{&MetricsHistogram::Log};

  //counter_example.join();
  //observable_counter_example.join();
  //histogram_example.join();
  std::vector<long> data{ 10,11,12,13,14,16 };
  histogram->LogData(data);
}
