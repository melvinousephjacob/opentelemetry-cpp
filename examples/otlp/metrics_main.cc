// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0
#include "metrics.h"

int main()
{
  opentelemetry::exporter::otlp::OtlpHttpMetricExporterOptions exporter_options;
  exporter_options.url = "http://opentelemetry-collector-lwshost:4318/v1/metrics";

  auto meter = new metrics::MetricsLogger("Sample FRU Name", "Sample Property Name", "This is a sample property description", exporter_options);

  meter->Log("Hello");
}
