#include "otel_metrics.h"

namespace metrics
{
  nostd::unique_ptr<opentelemetry::metrics::Counter<uint64_t>> meter;
  class MetricsCounterLogger : public IMetricsLogger
	{
		public:
			MetricsCounterLogger(std::string fruName, std::string propertyName, std::string propertyDescription, opentelemetry::exporter::otlp::OtlpHttpMetricExporterOptions metrics_opts);
	
			~MetricsCounterLogger();
	
			void Log(std::string textStr);
	};

MetricsCounterLogger::MetricsCounterLogger(std::string fruName, std::string propertyName, std::string propertyDescription, opentelemetry::exporter::otlp::OtlpHttpMetricExporterOptions metrics_opts)
{
	otel_metrics::InitMetrics(metrics_opts);
	meter = otel_metrics::get_counter(fruName, propertyName, propertyDescription);
}

MetricsCounterLogger::~MetricsCounterLogger()
{
	otel_metrics::CleanupMetrics();
}

void MetricsCounterLogger::Log(std::string textStr)
{
	meter->Add(1);
}
}



