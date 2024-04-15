// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/exporters/ostream/log_record_exporter.h"
#include "opentelemetry/exporters/ostream/common_utils.h"
#include "opentelemetry/sdk/instrumentationscope/instrumentation_scope.h"
#include "opentelemetry/sdk/logs/read_write_log_record.h"
#include "opentelemetry/sdk/resource/resource.h"
#include "opentelemetry/sdk_config.h"

#include <iostream>
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
    sout_ << "MR_OTEL_ConsoleLog{"
          << "  timestamp          : " << log_record->GetTimestamp().time_since_epoch().count() << ", "
          << "  observed_timestamp : "
          << log_record->GetObservedTimestamp().time_since_epoch().count() << ", "
          << "  severity_num       : " << static_cast<std::uint32_t>(log_record->GetSeverity()) << ", "
          << "  severity_text      : ";

    std::uint32_t severity_index = static_cast<std::uint32_t>(log_record->GetSeverity());
    if (severity_index >= std::extent<decltype(opentelemetry::logs::SeverityNumToText)>::value)
    {
      sout_ << "Invalid severity(" << severity_index << "), ";
    }
    else
    {
      sout_ << opentelemetry::logs::SeverityNumToText[severity_index] << ", ";
    }

    sout_ << "  body               : ";
    opentelemetry::exporter::ostream_common::print_value(log_record->GetBody(), sout_);
    sout_ << ",  resource           : , ";
    printAttributes(log_record->GetResource().GetAttributes());

    sout_ << ",  attributes         : ";

    printAttributes(log_record->GetAttributes());

    sout_ << ",  event_id           : " << event_id << ", "
          << "  event_name         : " << log_record->GetEventName() << ", "
          << "  trace_id           : " << std::string(trace_id, trace_id_len) << ", "
          << "  span_id            : " << std::string(span_id, span_id__len) << ", "
          << "  trace_flags        : " << std::string(trace_flags, trace_flags_len) << ", "
          << "  scope              : , "
          << "    name             : " << log_record->GetInstrumentationScope().GetName() << ", "
          << "    version          : " << log_record->GetInstrumentationScope().GetVersion() << ", "
          << "    schema_url       : " << log_record->GetInstrumentationScope().GetSchemaURL() << ", "
          << "    attributes       : ";

    printAttributes(log_record->GetInstrumentationScope().GetAttributes());
    sout_ << ", }\n";
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

void OStreamLogRecordExporter::printAttributes(
    const std::unordered_map<std::string, sdkcommon::OwnedAttributeValue> &map)
{
  for (const auto &kv : map)
  {
    sout_ << kv.first << ": ";
    opentelemetry::exporter::ostream_common::print_value(kv.second, sout_);
    sout_ << ", ";
  }
}

void OStreamLogRecordExporter::printAttributes(
    const std::unordered_map<std::string, opentelemetry::common::AttributeValue> &map)
{
  for (const auto &kv : map)
  {
    sout_ << kv.first << ": ";
    opentelemetry::exporter::ostream_common::print_value(kv.second, sout_);
    sout_ << ", ";
  }
}

}  // namespace logs
}  // namespace exporter
OPENTELEMETRY_END_NAMESPACE
