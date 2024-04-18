// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/exporters/ostream/span_exporter.h"
#include "opentelemetry/exporters/ostream/common_utils.h"

#include <iostream>
#include <mutex>
#include "opentelemetry/sdk_config.h"

namespace nostd     = opentelemetry::nostd;
namespace trace_sdk = opentelemetry::sdk::trace;
namespace trace_api = opentelemetry::trace;
namespace sdkcommon = opentelemetry::sdk::common;

OPENTELEMETRY_BEGIN_NAMESPACE
namespace exporter
{
namespace trace
{

std::ostream &operator<<(std::ostream &os, trace_api::SpanKind span_kind)
{
  switch (span_kind)
  {
    case trace_api::SpanKind::kClient:
      return os << "Client";
    case trace_api::SpanKind::kInternal:
      return os << "Internal";
    case trace_api::SpanKind::kServer:
      return os << "Server";
    case trace_api::SpanKind::kProducer:
      return os << "Producer";
    case trace_api::SpanKind::kConsumer:
      return os << "Consumer";
  };
  return os << "";
}

OStreamSpanExporter::OStreamSpanExporter(std::ostream &sout) noexcept : sout_(sout) {}

std::unique_ptr<trace_sdk::Recordable> OStreamSpanExporter::MakeRecordable() noexcept
{
  return std::unique_ptr<trace_sdk::Recordable>(new trace_sdk::SpanData);
}

sdk::common::ExportResult OStreamSpanExporter::Export(
    const nostd::span<std::unique_ptr<trace_sdk::Recordable>> &spans) noexcept
{
  if (isShutdown())
  {
    OTEL_INTERNAL_LOG_ERROR("[Ostream Trace Exporter] Exporting "
                            << spans.size() << " span(s) failed, exporter is shutdown");
    return sdk::common::ExportResult::kFailure;
  }

  for (auto &recordable : spans)
  {
    auto span = std::unique_ptr<trace_sdk::SpanData>(
        static_cast<trace_sdk::SpanData *>(recordable.release()));

    if (span != nullptr)
    {

      char trace_id[32]       = {0};
      char span_id[16]        = {0};
      char parent_span_id[16] = {0};

      span->GetTraceId().ToLowerBase16(trace_id);
      span->GetSpanId().ToLowerBase16(span_id);
      span->GetParentSpanId().ToLowerBase16(parent_span_id);

      sout_ << "MR_OTEL_ConsoleTrace{"
            << "  name          : " << span->GetName()
            << " ,  trace_id      : " << std::string(trace_id, 32)
            << " , span_id       : " << std::string(span_id, 16)
            << " , tracestate    : " << span->GetSpanContext().trace_state()->ToHeader()
            << " , parent_span_id: " << std::string(parent_span_id, 16)
            << " , start         : " << span->GetStartTime().time_since_epoch().count()
            << " , duration      : " << span->GetDuration().count()
            << " , description   : " << span->GetDescription()
            << " , span kind     : " << span->GetSpanKind()
            << " , status        : " << statusMap[int(span->GetStatus())]
            << " , attributes    : , ";
      printAttributes(span->GetAttributes());
      sout_ << "  events        : ";
      printEvents(span->GetEvents());
      sout_ << " ,  links         : ";
      printLinks(span->GetLinks());
      sout_ << " ,  resources     : , ";
      printResources(span->GetResource());
      sout_ << "  instr-lib     : ";
      printInstrumentationScope(span->GetInstrumentationScope());
      sout_ << ", }\n";
    }
  }

  return sdk::common::ExportResult::kSuccess;
}

bool OStreamSpanExporter::ForceFlush(std::chrono::microseconds /* timeout */) noexcept
{
  sout_.flush();
  return true;
}

bool OStreamSpanExporter::Shutdown(std::chrono::microseconds /* timeout */) noexcept
{
  const std::lock_guard<opentelemetry::common::SpinLockMutex> locked(lock_);
  is_shutdown_ = true;
  return true;
}

bool OStreamSpanExporter::isShutdown() const noexcept
{
  const std::lock_guard<opentelemetry::common::SpinLockMutex> locked(lock_);
  return is_shutdown_;
}
void OStreamSpanExporter::printAttributes(
    const std::unordered_map<std::string, sdkcommon::OwnedAttributeValue> &map)
{
  for (const auto &kv : map)
  {
    sout_ << kv.first << ": ";
    opentelemetry::exporter::ostream_common::print_value(kv.second, sout_);
    sout_ << " , ";
  }
}

void OStreamSpanExporter::printEvents(const std::vector<trace_sdk::SpanDataEvent> &events)
{
  for (const auto &event : events)
  {
    sout_ << "{"
          << "  name          : " << event.GetName()
          << " ,  timestamp     : " << event.GetTimestamp().time_since_epoch().count()
          << " ,  attributes    : ";
    printAttributes(event.GetAttributes());
    sout_ << " , }";
  }
}

void OStreamSpanExporter::printLinks(const std::vector<trace_sdk::SpanDataLink> &links)
{
  for (const auto &link : links)
  {
    char trace_id[32] = {0};
    char span_id[16]  = {0};
    link.GetSpanContext().trace_id().ToLowerBase16(trace_id);
    link.GetSpanContext().span_id().ToLowerBase16(span_id);
    sout_ << "{"
          << "  trace_id      : " << std::string(trace_id, 32)
          << " ,  span_id       : " << std::string(span_id, 16)
          << " ,  tracestate    : " << link.GetSpanContext().trace_state()->ToHeader()
          << " ,  attributes    : ";
    printAttributes(link.GetAttributes());
    sout_ << " , }";
  }
}

void OStreamSpanExporter::printResources(const opentelemetry::sdk::resource::Resource &resources)
{
  auto attributes = resources.GetAttributes();
  if (attributes.size())
  {
    printAttributes(attributes);
  }
}

void OStreamSpanExporter::printInstrumentationScope(
    const opentelemetry::sdk::instrumentationscope::InstrumentationScope &instrumentation_scope)
{
  sout_ << instrumentation_scope.GetName();
  auto version = instrumentation_scope.GetVersion();
  if (version.size())
  {
    sout_ << "-" << version;
  }
}

}  // namespace trace
}  // namespace exporter
OPENTELEMETRY_END_NAMESPACE
