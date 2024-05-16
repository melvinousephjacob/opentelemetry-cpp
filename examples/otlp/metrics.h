#include "otel.h"

namespace metrics
{
  nostd::unique_ptr<opentelemetry::metrics::Counter<uint64_t>> meter;
  class MetricsLogger : public IMetricsLogger
	{
		public:
			MetricsLogger(std::string fruName, std::string propertyName, std::string propertyDescription, opentelemetry::exporter::otlp::OtlpHttpMetricExporterOptions metrics_opts);
	
			~MetricsLogger();
	
			void Log(std::string textStr);
	};

MetricsLogger::MetricsLogger(std::string fruName, std::string propertyName, std::string propertyDescription, opentelemetry::exporter::otlp::OtlpHttpMetricExporterOptions metrics_opts)
{
	otel::InitMetrics(metrics_opts);
	meter = otel::get_meter(fruName, propertyName, propertyDescription);
}

MetricsLogger::~MetricsLogger()
{
	otel::CleanupMetrics();
}

void MetricsLogger::Log(std::string textStr)
{
	meter->Add(1);
}
}



