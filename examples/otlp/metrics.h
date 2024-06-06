
#include "otel_metrics.h"

namespace
{
  opentelemetry::exporter::otlp::OtlpHttpMetricExporterOptions exporter_options;
  nostd::unique_ptr<opentelemetry::metrics::Counter<uint64_t>> counter;
  nostd::shared_ptr<opentelemetry::metrics::ObservableInstrument> observablecounter;
  nostd::unique_ptr<opentelemetry::metrics::Histogram<uint64_t>> histogram;
  nostd::shared_ptr<opentelemetry::metrics::ObservableInstrument> observablegauge;
  nostd::unique_ptr<opentelemetry::metrics::UpDownCounter<int64_t>> updowncounter;
  nostd::shared_ptr<opentelemetry::metrics::ObservableInstrument> observableupdowncounter;

  ///////////////////////////////////////////////COUNTER/////////////////////////////////////////
  class MetricsCounter : public IMetricsLogger
	{
		public:
			MetricsCounter(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url);
	
			~MetricsCounter();
	
			void Log();
	};

MetricsCounter::MetricsCounter(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url)
{
	exporter_options.url = metrics_url;
	otel_metrics::InitMetrics(fruName, exporter_options);
	counter = otel_metrics::get_counter(fruName, propertyName, propertyDescription);
}

MetricsCounter::~MetricsCounter()
{
	otel_metrics::CleanupMetrics();
}

void MetricsCounter::Log()
{
	for (uint32_t i = 0; i < 20; ++i)
  	{
		counter->Add(1);
    	  	std::this_thread::sleep_for(std::chrono::milliseconds(500));
  	}
}

///////////////////////////////////////////////OBSERVABLE COUNTER/////////////////////////////////////////
class MetricsObservableCounter : public IMetricsLogger
	{
		public:
			MetricsObservableCounter(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url);
	
			~MetricsObservableCounter();
	
			void Log();
	};

MetricsObservableCounter::MetricsObservableCounter(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url)
{
	exporter_options.url = metrics_url;
	otel_metrics::InitMetrics(fruName, exporter_options);
	observablecounter = otel_metrics::get_observablecounter(fruName, propertyName, propertyDescription);
}

MetricsObservableCounter::~MetricsObservableCounter()
{
	otel_metrics::CleanupMetrics();
}

void MetricsObservableCounter::Log()
{
	
}


///////////////////////////////////////////////HISTOGRAM/////////////////////////////////////////
class MetricsHistogram : public IMetricsLogger
	{
		public:
			MetricsHistogram(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url);
	
			~MetricsHistogram();
	
			void Log();
	};

MetricsHistogram::MetricsHistogram(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url)
{
	exporter_options.url = metrics_url;
	otel_metrics::InitMetrics(fruName, exporter_options);
	histogram = otel_metrics::get_histogram(fruName, propertyName, propertyDescription);
}

MetricsHistogram::~MetricsHistogram()
{
	otel_metrics::CleanupMetrics();
}

void MetricsHistogram::Log()
{
	auto context           = opentelemetry::context::Context{};
	for (uint32_t i = 0; i < 20; ++i)
  	{
		histogram->Record(25, context);
    	  	std::this_thread::sleep_for(std::chrono::milliseconds(500));
  	}
}


///////////////////////////////////////////////OBSERVABLE GAUGE/////////////////////////////////////////
class MetricsObservableGauge : public IMetricsLogger
	{
		public:
			MetricsObservableGauge(std::string fruName, std::string propertyName, std::string propertyDescription,  opentelemetry::metrics::ObservableCallbackPtr callback, std::string metrics_url);
	
			~MetricsObservableGauge();
	
			void Log();
	};

MetricsObservableGauge::MetricsObservableGauge(std::string fruName, std::string propertyName, std::string propertyDescription, opentelemetry::metrics::ObservableCallbackPtr callback, std::string metrics_url)
{
	exporter_options.url = metrics_url;
	otel_metrics::InitMetrics(fruName, exporter_options);
	observablegauge = otel_metrics::get_observablegauge(fruName, propertyName, propertyDescription, callback);
}

MetricsObservableGauge::~MetricsObservableGauge()
{
	otel_metrics::CleanupMetrics();
}

void MetricsObservableGauge::Log()
{
	
}


///////////////////////////////////////////////UP DOWN COUNTER/////////////////////////////////////////
class MetricsUpDownCounter : public IMetricsLogger
	{
		public:
			MetricsUpDownCounter(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url);
	
			~MetricsUpDownCounter();
	
			void Log();
	};

MetricsUpDownCounter::MetricsUpDownCounter(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url)
{
	exporter_options.url = metrics_url;
	otel_metrics::InitMetrics(fruName, exporter_options);
	updowncounter = otel_metrics::get_updowncounter(fruName, propertyName, propertyDescription);
}

MetricsUpDownCounter::~MetricsUpDownCounter()
{
	otel_metrics::CleanupMetrics();
}

void MetricsUpDownCounter::Log()
{
	for (uint32_t i = 0; i < 20; ++i)
  	{
		updowncounter->Add(2);
    	  	std::this_thread::sleep_for(std::chrono::milliseconds(500));
  	}
}


///////////////////////////////////////////////OBSERVABLE UP DOWN COUNTER/////////////////////////////////////////
class MetricsObservableUpDownCounter : public IMetricsLogger
	{
		public:
			MetricsObservableUpDownCounter(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url);
	
			~MetricsObservableUpDownCounter();
	
			void Log();
	};

MetricsObservableUpDownCounter::MetricsObservableUpDownCounter(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url)
{
	exporter_options.url = metrics_url;
	otel_metrics::InitMetrics(fruName, exporter_options);
	observableupdowncounter = otel_metrics::get_observableupdowncounter(fruName, propertyName, propertyDescription);
}

MetricsObservableUpDownCounter::~MetricsObservableUpDownCounter()
{
	otel_metrics::CleanupMetrics();
}

void MetricsObservableUpDownCounter::Log()
{
	
}
}
