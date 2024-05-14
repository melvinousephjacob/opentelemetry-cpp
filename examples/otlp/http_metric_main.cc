// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#ifdef BAZEL_BUILD
#  include "examples/common/metrics_foo_library/foo_library.h"
#else
#  include "metrics_foo_library/foo_library.h"
#endif

int main(int argc, char *argv[])
{
  //std::string example_type;
  /*if (argc > 1)
  {
    exporter_options.url = argv[1];
  }

  if (argc > 2)
  {
    example_type = argv[2];
  }

  if (argc > 3)
  {
    std::string debug              = argv[3];
    exporter_options.console_debug = debug != "" && debug != "0" && debug != "no";
  }

  if (argc > 4)
  {
    std::string binary_mode = argv[4];
    if (binary_mode.size() >= 3 && binary_mode.substr(0, 3) == "bin")
    {
      exporter_options.content_type = otlp_exporter::HttpRequestContentType::kBinary;
    }
  }

  if (exporter_options.console_debug)
  {
    internal_log::GlobalLogHandler::SetLogLevel(internal_log::LogLevel::Debug);
  }

  exporter_options.console_debug = true;
  internal_log::GlobalLogHandler::SetLogLevel(internal_log::LogLevel::Debug);*/

  exporter_options.url = "http://opentelemetry-collector-lwshost:4318/v1/metrics";
  std::string example_type{"all"};
  // Removing this line will leave the default noop MetricProvider in place.
  InitMetrics();
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
  CleanupMetrics();
}
