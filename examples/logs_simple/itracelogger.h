#pragma once
#include "tracedata.h";
#include <string>
#include <exception>

class ITraceLogger
{
    public:
        void TraceInfo(std::string message, std::exception exception);
        void TraceInfo(std::string message, TraceData traceData);
        void TraceInfo(std::string message);
        void TraceVerbose(std::string message);
        void TraceVerbose(std::string message, TraceData traceData);
        void TraceVerbose(std::string message, std::exception exception);
};
