// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0
#include "metrics.h"

int main()
{
  auto meter = new metrics::MetricsCounter("Sample_FRU_Name", "Sample_Property_Name", "This is a sample property description");
  meter->Log("Hello");

  auto observableCounter = new metrics::MetricsObservableCounter("Sample_ObservableCounter", "Sample_Property_Name", "This is a sample observable counter");

  auto observableGauge = new metrics::MetricsObservableGauge("Sample_ObservableGauge", "Sample_Property_Name", "This is a sample observable gauge");

  auto observableUpDownCounter = new metrics::MetricsObservableUpDownCounter("Sample_ObservableUpDownCounter", "Sample_Property_Name", "This is a sample observable up down counter");

  auto histogram = new metrics::MetricsHistogram("Sample_Histogram", "Sample_Property_Name", "This is a sample histogram");
}
