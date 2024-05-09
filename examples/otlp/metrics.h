#include "imetricslogger.h"

namespace metrics
{
  class MetricsLogger : public IMetricsLogger
	{
		public:
			MetricsLogger(std::string loggerName, std::string nameSpace, std::string className, opentelemetry::exporter::otlp::OtlpHttpMetricExporterOptions metrics_opts);
	
			~MetricsLogger();
	
			void Log(std::string textStr);
	};
