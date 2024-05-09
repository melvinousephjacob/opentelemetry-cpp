// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "trace.h"

int main()
{
  auto centralLogServer1 = new logging::CentralLogServer("Logger #1", "Sample Namespace #1", "Sample Classname #1");
  auto centralLogServer2 = new logging::CentralLogServer("Logger #1", "Sample Namespace #2", "Sample Classname #2");
  auto centralLogServer3 = new logging::CentralLogServer("Logger #1", "Sample Namespace #1", "Sample Classname #1");
  
  centralLogServer1->Log("This is a sample log message with Debug severity", DebugInfo);
  centralLogServer2->Log("This is a sample log message with Fatal severity", Fatal);
  centralLogServer3->Log("This is a sample log message with Error severity and OSEvent info category", Error, OSEvent);

  auto traceLogger = new traces::TraceLogger("Sample module", trace_opts);
  TraceData traceData;
  traceData.AdditionalInfo = "This is some additional info.";
  traceData.DateTime = std::time(0);
  traceLogger->TraceInfo("This is a sample trace message", traceData);
}
