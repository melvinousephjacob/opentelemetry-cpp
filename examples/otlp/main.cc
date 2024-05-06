// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "trace.h"

int main()
{
  trace_opts.url  = "http://opentelemetry-collector-lwshost:4318/v1/traces";
  logger_opts.url = "http://opentelemetry-collector-lwshost:4318/v1/logs";
  auto centralLogServer = new CentralLogServer;
  CoreLogData coreLogData;
  coreLogData.Resolution = "Example core log data";
  
  centralLogServer->Log("HELLO", Fatal, coreLogData);
  
}
