#include "imetricslogger.h"

namespace metrics
{
  class MetricsLogger : public IMetricsLogger
	{
		public:
			MetricsLogger(std::string fruName, std::string propertyName, std::string propertyDescription, opentelemetry::exporter::otlp::OtlpHttpMetricExporterOptions metrics_opts);
	
			~MetricsLogger();
	
			void Log(std::string textStr);
	};
