// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0
#inlcude "otel.h"

#ifdef BAZEL_BUILD
#  include "examples/common/metrics_foo_library/foo_library.h"
#else
#  include "metrics_foo_library/foo_library.h"
#endif

int main(int argc, char *argv[])
{
  opentelemetry::exporter::otlp::OtlpHttpMetricExporterOptions exporter_options;
  exporter_options.url = "http://opentelemetry-collector-lwshost:4318/v1/metrics";
  std::string example_type{"all"};
  // Removing this line will leave the default noop MetricProvider in place.
  otel::InitMetrics();
  std::string name{"otlp_http_metric_example"};

  /*if (example_type == "counter")
  {
    foo_library::counter_example(name);
  }
  else if (example_type == "observable_counter")
  {
    foo_library::observable_counter_example(name);
  }
  else if (example_type == "histogram")
  {
    foo_library::histogram_example(name);
  }
  else
  {
    std::thread counter_example{&foo_library::counter_example, name};
    std::thread observable_counter_example{&foo_library::observable_counter_example, name};
    std::thread histogram_example{&foo_library::histogram_example, name};

    counter_example.join();
    observable_counter_example.join();
    histogram_example.join();
  }*/

  foo_library::observable_counter_example(name);
  otel::CleanupMetrics();
}
