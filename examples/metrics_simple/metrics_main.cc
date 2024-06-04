// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0
#include "metrics.h"

int main()
{
  //std::string a{"Hello"};
  auto counter = new MetricsCounter("Sample_FRU_Name", "Sample_Property_Name", "This is a sample property description");
  //MetricsCounter x;
  //std::thread counter_example (&MetricsCounter::Log, std::ref(x));
  counter->Log();

  auto observableCounter = new metrics::MetricsObservableCounter("Sample_ObservableCounter", "Sample_Property_Name", "This is a sample observable counter");

  auto observableGauge = new metrics::MetricsObservableGauge("Sample_ObservableGauge", "Sample_Property_Name", "This is a sample observable gauge");

  auto updowncounter = new metrics::MetricsUpDownCounter("Sample_UpDownCounter", "Sample_Property_Name", "This is a sample up down counter");
  updowncounter->Log("Hello");

  auto observableUpDownCounter = new metrics::MetricsObservableUpDownCounter("Sample_ObservableUpDownCounter", "Sample_Property_Name", "This is a sample observable up down counter");

  auto histogram = new MetricsHistogram("Sample_Histogram", "Sample_Property_Name", "This is a sample histogram");
  //metrics::MetricsHistogram y;
  //std::thread counter_example{&foo_library::counter_example, name};
  //std::thread observable_counter_example{&foo_library::observable_counter_example, name};
  //std::thread histogram_example{&MetricsHistogram::Log};

  //counter_example.join();
  //observable_counter_example.join();
  //histogram_example.join();
  histogram->Log();
}
