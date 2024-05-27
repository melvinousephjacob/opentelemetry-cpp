// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0
#include "metrics.h"

int main()
{
  std::string metrics_exporter_url = "http://opentelemetry-collector-lwshost:4318/v1/metrics";

  auto meter = new metrics::MetricsCounter("Sample_FRU_Name", "Sample_Property_Name", "This is a sample property description", metrics_exporter_url);

  meter->Log("Hello");
}
