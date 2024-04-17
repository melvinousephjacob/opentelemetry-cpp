// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "logging.h"

int main()
{
  auto centralLogServer = new logging::CentralLogServer("Sample module", "1.0");
  CoreLogData coreLogData;
  coreLogData.Namespace = "Sample Namespace";
  
  centralLogServer->Log("This is a sample log message", DebugInfo, coreLogData);
  centralLogServer->Log("HELLO", Fatal, coreLogData);
}
