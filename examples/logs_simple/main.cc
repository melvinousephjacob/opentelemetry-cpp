// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "trace.h"

int main()
{
  auto centralLogServer = new logging::CentralLogServer("Logger Name", "Sample Namespace", "Sample Classname");
  CoreLogData coreLogData;
  //coreLogData.Namespace = "Sample Namespace";
  
  centralLogServer->Log("This is a sample log message", DebugInfo, coreLogData);
  centralLogServer->Log("HELLO", Fatal, coreLogData);
  centralLogServer->Log("Hiiii", Error, OSEvent, coreLogData);

  //auto traceLogger = new trace::TraceLogger("Sample module");
  //TraceData traceData;
  //traceData.AdditionalInfo = "This is some additional info.";
  //traceData.DateTime = std::time(0);
  //traceLogger->TraceInfo("Hey", traceData);
}
