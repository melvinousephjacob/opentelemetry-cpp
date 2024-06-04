#include "otel_metrics.h"

namespace metrics
{
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
			MetricsCounter(std::string fruName, std::string propertyName, std::string propertyDescription);
	
			~MetricsCounter();
	
			static void Log(std::string &textStr);
	};

MetricsCounter::MetricsCounter(std::string fruName, std::string propertyName, std::string propertyDescription)
{
	otel_metrics::InitMetrics(fruName);
	counter = otel_metrics::get_counter(fruName, propertyName, propertyDescription);
}

MetricsCounter::~MetricsCounter()
{
	otel_metrics::CleanupMetrics();
}

void MetricsCounter::Log(const std::string &textStr)
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
			MetricsObservableCounter(std::string fruName, std::string propertyName, std::string propertyDescription);
	
			~MetricsObservableCounter();
	
			void Log(std::string textStr);
	};

MetricsObservableCounter::MetricsObservableCounter(std::string fruName, std::string propertyName, std::string propertyDescription)
{
	otel_metrics::InitMetrics(fruName);
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
			MetricsHistogram(std::string fruName, std::string propertyName, std::string propertyDescription);
	
			~MetricsHistogram();
	
			void Log(std::string textStr);
	};

MetricsHistogram::MetricsHistogram(std::string fruName, std::string propertyName, std::string propertyDescription)
{
	otel_metrics::InitMetrics(fruName);
	histogram = otel_metrics::get_histogram(fruName, propertyName, propertyDescription);
}

MetricsHistogram::~MetricsHistogram()
{
	otel_metrics::CleanupMetrics();
}

void MetricsHistogram::Log(const std::string &textStr)
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
			MetricsObservableGauge(std::string fruName, std::string propertyName, std::string propertyDescription);
	
			~MetricsObservableGauge();
	
			void Log(std::string textStr);
	};

MetricsObservableGauge::MetricsObservableGauge(std::string fruName, std::string propertyName, std::string propertyDescription)
{
	otel_metrics::InitMetrics(fruName);
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
			MetricsUpDownCounter(std::string fruName, std::string propertyName, std::string propertyDescription);
	
			~MetricsUpDownCounter();
	
			void Log(std::string textStr);
	};

MetricsUpDownCounter::MetricsUpDownCounter(std::string fruName, std::string propertyName, std::string propertyDescription)
{
	otel_metrics::InitMetrics(fruName);
	updowncounter = otel_metrics::get_updowncounter(fruName, propertyName, propertyDescription);
}

MetricsUpDownCounter::~MetricsUpDownCounter()
{
	otel_metrics::CleanupMetrics();
}

void MetricsUpDownCounter::Log(std::string textStr)
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
			MetricsObservableUpDownCounter(std::string fruName, std::string propertyName, std::string propertyDescription);
	
			~MetricsObservableUpDownCounter();
	
			void Log(std::string textStr);
	};

MetricsObservableUpDownCounter::MetricsObservableUpDownCounter(std::string fruName, std::string propertyName, std::string propertyDescription)
{
	otel_metrics::InitMetrics(fruName);
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
