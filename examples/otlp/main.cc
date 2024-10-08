// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "trace.h"

int main()
{
  std::string trace_endpoint_url  = "http://opentelemetry-collector-lwshost:4318/v1/traces";
  std::string log_endpoint_url = "http://opentelemetry-collector-lwshost:4318/v1/logs";
  
  auto centralLogServer1 = new logging::CentralLogServer("Logger #1", "Sample Namespace #1", "Sample Classname #1");
  auto centralLogServer2 = new logging::CentralLogServer("Logger #1", "Sample Namespace #2", "Sample Classname #2");
  auto centralLogServer3 = new logging::CentralLogServer("Logger #1", "Sample Namespace #1", "Sample Classname #1");
  
  centralLogServer1->Log("This is a sample log message with Debug severity", DebugInfo);
  centralLogServer2->Log("This is a sample log message with Fatal severity", Fatal);
  centralLogServer3->Log("This is a sample log message with Error severity and OSEvent info category", Error, OSEvent);

  auto traceLogger = new traces::TraceLogger("Sample module");
  TraceData traceData;
  traceData.AdditionalInfo = "This is some additional info.";
  traceData.DateTime = std::time(0);
  traceLogger->TraceInfo("This is a sample trace message", traceData);

  /*auto deviceinfologger = new logging::DeviceInfo("Sample_file_name");
  deviceinfologger->Log("Sample info");
  deviceinfologger->Log("More sample info");
  deviceinfologger->Log("Even more sample info");*/
}
