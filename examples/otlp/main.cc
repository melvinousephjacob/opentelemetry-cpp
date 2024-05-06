// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "trace.h"

int main()
{
  trace_opts.url  = "http://opentelemetry-collector-lwshost:4318/v1/traces";
  logger_opts.url = "http://opentelemetry-collector-lwshost:4318/v1/logs";
  
  auto centralLogServer1 = new logging::CentralLogServer("Logger #1", "Sample Namespace #1", "Sample Classname #1");
  auto centralLogServer2 = new logging::CentralLogServer("Logger #1", "Sample Namespace #2", "Sample Classname #2");
  auto centralLogServer3 = new logging::CentralLogServer("Logger #1", "Sample Namespace #1", "Sample Classname #1");
  CoreLogData coreLogData;
  coreLogData.Namespace = "Sample Namespace";
  
  centralLogServer1->Log("This is a sample log message", DebugInfo, coreLogData);
  centralLogServer2->Log("HELLO", Fatal, coreLogData);
  centralLogServer3->Log("Hiiii", Error, OSEvent, coreLogData);

  //auto traceLogger = new trace::TraceLogger("Sample module");
  //TraceData traceData;
  //traceData.AdditionalInfo = "This is some additional info.";
  //traceData.DateTime = std::time(0);
  //traceLogger->TraceInfo("Hey", traceData);
}
