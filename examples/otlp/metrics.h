
#include "otel_metrics.h"
#include <vector>

namespace
{
  nostd::unique_ptr<opentelemetry::metrics::Histogram<uint64_t>> histogram;
  nostd::shared_ptr<opentelemetry::metrics::ObservableInstrument> observablegauge;

///////////////////////////////////////////////HISTOGRAM/////////////////////////////////////////
class HistogramLogger : public IHistogramLogger
	{
		public:
			HistogramLogger(std::string fruName, std::string propertyName, unsigned int historyLength, unsigned int numberOfBins, unsigned int binWidth, int min, int max, std::string siUnit);
	
			~HistogramLogger();
	
			void LogData(std::vector<long> histogramData);
	};

HistogramLogger::HistogramLogger(std::string fruName, std::string propertyName, unsigned int historyLength, unsigned int numberOfBins, unsigned int binWidth, int min, int max, std::string siUnit)
{
	otel_metrics::InitMetrics(fruName);
	histogram = otel_metrics::get_histogram(fruName, propertyName, historyLength, numberOfBins, binWidth, min, max, siUnit);
}

HistogramLogger::~HistogramLogger()
{
	otel_metrics::CleanupMetrics();
}

void HistogramLogger::LogData(std::vector<long> histogramData)
{
	auto context           = opentelemetry::context::Context{};
	for (auto& i: histogramData)
  	{
		histogram->Record(i, context);
    	  	std::this_thread::sleep_for(std::chrono::milliseconds(500));
  	}
}

///////////////////////////////////////////////OBSERVABLE GAUGE/////////////////////////////////////////
class DevicePropertyLogger
	{
		public:
			DevicePropertyLogger(std::string fruName, std::string propertyName, std::string propertyDescription, int historyL, bool timeStampProvided, opentelemetry::metrics::ObservableCallbackPtr callback);
	
			~DevicePropertyLogger();

			void Log();
	};

DevicePropertyLogger::DevicePropertyLogger(std::string fruName, std::string propertyName, std::string propertyDescription, int historyL, bool timeStampProvided, opentelemetry::metrics::ObservableCallbackPtr callback)
{
	otel_metrics::InitMetrics(fruName);
	observablegauge = otel_metrics::get_observablegauge(fruName, propertyName, propertyDescription, historyL, timeStampProvided, callback);
}

DevicePropertyLogger::~DevicePropertyLogger()
{
	otel_metrics::CleanupMetrics();
}

DevicePropertyLogger::Log()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}


}
