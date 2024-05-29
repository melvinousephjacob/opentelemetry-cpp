#include "otel_metrics.h"

namespace metrics
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
	
			void Log(std::string textStr);
	};

MetricsCounter::MetricsCounter(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url)
{
	exporter_options.url = metrics_url;
	otel_metrics::InitMetrics(exporter_options);
	counter = otel_metrics::get_counter(fruName, propertyName, propertyDescription);
}

MetricsCounter::~MetricsCounter()
{
	otel_metrics::CleanupMetrics();
}

void MetricsCounter::Log(std::string textStr)
{
	counter->Add(1);
}

///////////////////////////////////////////////OBSERVABLE COUNTER/////////////////////////////////////////
class MetricsObservableCounter : public IMetricsLogger
	{
		public:
			MetricsObservableCounter(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url);
	
			~MetricsObservableCounter();
	
			void Log(std::string textStr);
	};

MetricsObservableCounter::MetricsObservableCounter(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url)
{
	exporter_options.url = metrics_url;
	otel_metrics::InitMetrics(exporter_options);
	observablecounter = otel_metrics::get_observablecounter(fruName, propertyName, propertyDescription);
}

MetricsObservableCounter::~MetricsObservableCounter()
{
	otel_metrics::CleanupMetrics();
}

void MetricsObservableCounter::Log(std::string textStr)
{
	
}


///////////////////////////////////////////////HISTOGRAM/////////////////////////////////////////
class MetricsHistogram : public IMetricsLogger
	{
		public:
			MetricsHistogram(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url);
	
			~MetricsHistogram();
	
			void Log(std::string textStr);
	};

MetricsHistogram::MetricsHistogram(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url)
{
	exporter_options.url = metrics_url;
	otel_metrics::InitMetrics(exporter_options);
	histogram = otel_metrics::get_histogram(fruName, propertyName, propertyDescription);
}

MetricsHistogram::~MetricsHistogram()
{
	otel_metrics::CleanupMetrics();
}

void MetricsHistogram::Log(std::string textStr)
{
	
}


///////////////////////////////////////////////OBSERVABLE GAUGE/////////////////////////////////////////
class MetricsObservableGauge : public IMetricsLogger
	{
		public:
			MetricsObservableGauge(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url);
	
			~MetricsObservableGauge();
	
			void Log(std::string textStr);
	};

MetricsObservableGauge::MetricsObservableGauge(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url)
{
	exporter_options.url = metrics_url;
	otel_metrics::InitMetrics(exporter_options);
	observablegauge = otel_metrics::get_observablegauge(fruName, propertyName, propertyDescription);
}

MetricsObservableGauge::~MetricsObservableGauge()
{
	otel_metrics::CleanupMetrics();
}

void MetricsObservableGauge::Log(std::string textStr)
{
	
}


///////////////////////////////////////////////UP DOWN COUNTER/////////////////////////////////////////
class MetricsUpDownCounter : public IMetricsLogger
	{
		public:
			MetricsUpDownCounter(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url);
	
			~MetricsUpDownCounter();
	
			void Log(std::string textStr);
	};

MetricsUpDownCounter::MetricsUpDownCounter(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url)
{
	exporter_options.url = metrics_url;
	otel_metrics::InitMetrics(exporter_options);
	updowncounter = otel_metrics::get_updowncounter(fruName, propertyName, propertyDescription);
}

MetricsUpDownCounter::~MetricsUpDownCounter()
{
	otel_metrics::CleanupMetrics();
}

void MetricsUpDownCounter::Log(std::string textStr)
{
	
}


///////////////////////////////////////////////OBSERVABLE UP DOWN COUNTER/////////////////////////////////////////
class MetricsObservableUpDownCounter : public IMetricsLogger
	{
		public:
			MetricsObservableUpDownCounter(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url);
	
			~MetricsObservableUpDownCounter();
	
			void Log(std::string textStr);
	};

MetricsObservableUpDownCounter::MetricsObservableUpDownCounter(std::string fruName, std::string propertyName, std::string propertyDescription, std::string metrics_url)
{
	exporter_options.url = metrics_url;
	otel_metrics::InitMetrics(exporter_options);
	observableupdowncounter = otel_metrics::get_observableupdowncounter(fruName, propertyName, propertyDescription);
}

MetricsObservableUpDownCounter::~MetricsObservableUpDownCounter()
{
	otel_metrics::CleanupMetrics();
}

void MetricsObservableUpDownCounter::Log(std::string textStr)
{
	
}
}
