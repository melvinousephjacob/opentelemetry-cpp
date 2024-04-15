// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/exporters/ostream/log_record_exporter.h"
#include "opentelemetry/exporters/ostream/common_utils.h"
#include "opentelemetry/sdk/instrumentationscope/instrumentation_scope.h"
#include "opentelemetry/sdk/logs/read_write_log_record.h"
#include "opentelemetry/sdk/resource/resource.h"
#include "opentelemetry/sdk_config.h"

#include <iostream>
#include <string>
#include <mutex>
#include <type_traits>

namespace nostd     = opentelemetry::nostd;
namespace sdklogs   = opentelemetry::sdk::logs;
namespace sdkcommon = opentelemetry::sdk::common;
OPENTELEMETRY_BEGIN_NAMESPACE
namespace exporter
{
namespace logs
{

/*********************** Constructor ***********************/

OStreamLogRecordExporter::OStreamLogRecordExporter(std::ostream &sout) noexcept : sout_(sout) {}

/*********************** Exporter methods ***********************/

std::unique_ptr<sdklogs::Recordable> OStreamLogRecordExporter::MakeRecordable() noexcept
{
  return std::unique_ptr<sdklogs::Recordable>(new sdklogs::ReadWriteLogRecord());
}

sdk::common::ExportResult OStreamLogRecordExporter::Export(
    const nostd::span<std::unique_ptr<sdklogs::Recordable>> &records) noexcept
{
  if (isShutdown())
  {
    OTEL_INTERNAL_LOG_ERROR("[Ostream Log Exporter] Exporting "
                            << records.size() << " log(s) failed, exporter is shutdown");
    return sdk::common::ExportResult::kFailure;
  }

  for (auto &record : records)
  {
    auto log_record = std::unique_ptr<sdklogs::ReadWriteLogRecord>(
        static_cast<sdklogs::ReadWriteLogRecord *>(record.release()));

    if (log_record == nullptr)
    {
      // TODO: Log Internal SDK error "recordable data was lost"
      continue;
    }

    int64_t event_id = log_record->GetEventId();

    // Convert trace, spanid, traceflags into exportable representation
    constexpr int trace_id_len    = 32;
    constexpr int span_id__len    = 16;
    constexpr int trace_flags_len = 2;

    char trace_id[trace_id_len]       = {0};
    char span_id[span_id__len]        = {0};
    char trace_flags[trace_flags_len] = {0};

    log_record->GetTraceId().ToLowerBase16(trace_id);
    log_record->GetSpanId().ToLowerBase16(span_id);
    log_record->GetTraceFlags().ToLowerBase16(trace_flags);

    // Print out each field of the log record, noting that severity is separated
    // into severity_num and severity_text

    std::string logRecord = "MR_OTEL_ConsoleLog{ ";
          logRecord.append("  timestamp          : ");
          logRecord.append(std::to_string(log_record->GetTimestamp().time_since_epoch().count()));
          logRecord.append(", ");
    
          logRecord.append("  observed_timestamp : ");
          logRecord.append(std::to_string(log_record->GetObservedTimestamp().time_since_epoch().count()));
          logRecord.append(", ");
    
          logRecord.append("  severity_num       : ");
    logRecord.append(static_cast<std::string>(log_record->GetSeverity()));
    logRecord.append(", ");
    
          logRecord.append("  severity_text      : ");

    std::uint32_t severity_index = static_cast<std::uint32_t>(log_record->GetSeverity());
    if (severity_index >= std::extent<decltype(opentelemetry::logs::SeverityNumToText)>::value)
    {
      sout_ << "Invalid severity(" << severity_index << ")\n";
    }
    else
    {
      logRecord.append(opentelemetry::logs::SeverityNumToText[severity_index]);
      logRecord.append(", ");
    }

    logRecord.append("  body               : ");
    logRecord.append(static_cast<std::string>(log_record->GetBody()));
    logRecord.append(", ");
    //opentelemetry::exporter::ostream_common::print_value(log_record->GetBody(), sout_);
    logRecord.append("  resource           : ");
    logRecord.append(printAttributes(log_record->GetResource().GetAttributes()));
    logRecord.append(", ");
    //printAttributes(log_record->GetResource().GetAttributes(), logRecord);

    logRecord.append("  attributes         : ");
    logRecord.append(printAttributes(log_record->GetAttributes()));
    logRecord.append(", ");
    

    //printAttributes(log_record->GetAttributes(), "\n    ");

          logRecord.append("  event_id           : ");
    logRecord.append(event_id);
    logRecord.append(", ");
    
          logRecord.append("  event_name         : ");
    logRecord.append(log_record->GetEventName());
    logRecord.append(", ");
    
          logRecord.append("  trace_id           : ");
    logRecord.append(std::string(trace_id, trace_id_len));
    logRecord.append(", ");
    
          logRecord.append("  span_id            : ");
    logRecord.append(std::string(span_id, span_id__len));
    logRecord.append(", ");
    
          logRecord.append("  trace_flags        : ");
    logRecord.append(std::string(trace_flags, trace_flags_len));
    logRecord.append(", ");
    
          logRecord.append("  scope              : , ");
    
          logRecord.append("    name             : ");
    logRecord.append(log_record->GetInstrumentationScope().GetName());
    logRecord.append(", ");
    
          logRecord.append("    version          : ");
    logRecord.append(log_record->GetInstrumentationScope().GetVersion());
    logRecord.append(", ");
    
         logRecord.append("    schema_url       : ");
    logRecord.append(log_record->GetInstrumentationScope().GetSchemaURL());
    logRecord.append(", ");
    
          logRecord.append("    attributes       : ");
    logRecord.append(printAttributes(log_record->GetInstrumentationScope().GetAttributes()));
    logRecord.append(", ");
    
          logRecord.append("}");

    //printAttributes(log_record->GetInstrumentationScope().GetAttributes());
    sout_ << logRecord << "\n";
  }

  return sdk::common::ExportResult::kSuccess;
}

bool OStreamLogRecordExporter::ForceFlush(std::chrono::microseconds /* timeout */) noexcept
{
  sout_.flush();
  return true;
}

bool OStreamLogRecordExporter::Shutdown(std::chrono::microseconds) noexcept
{
  const std::lock_guard<opentelemetry::common::SpinLockMutex> locked(lock_);
  is_shutdown_ = true;
  return true;
}

bool OStreamLogRecordExporter::isShutdown() const noexcept
{
  const std::lock_guard<opentelemetry::common::SpinLockMutex> locked(lock_);
  return is_shutdown_;
}

std::string OStreamLogRecordExporter::printAttributes(
    const std::unordered_map<std::string, sdkcommon::OwnedAttributeValue> &map)
{
  std::string logRecord = "";
  for (const auto &kv : map)
  {
    logRecord.append(kv.first + ": " + static_cast<std::string>(kv.second) + ", ");
    //opentelemetry::exporter::ostream_common::print_value(kv.second, sout_);
  }

  return logRecord;
}

std::string OStreamLogRecordExporter::printAttributes(
    const std::unordered_map<std::string, opentelemetry::common::AttributeValue> &map)
{
  std::string logRecord = "";
  for (const auto &kv : map)
  {
    logRecord.append(kv.first + ": " + static_cast<std::string>(kv.second) + ", ");
    //opentelemetry::exporter::ostream_common::print_value(kv.second, sout_);
  }

  return logRecord;
}

}  // namespace logs
}  // namespace exporter
OPENTELEMETRY_END_NAMESPACE
