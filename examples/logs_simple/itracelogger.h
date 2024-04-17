#pragma once
#include "tracedata.h";
#include <string>
#include <exception>

class ITraceLogger
{
    public:
        void TraceInfo(std::string message, std::exception exception);
        void TraceInfo(std::string message, TraceData data);
        void TraceInfo(std::string message);
        void TraceVerbose(std::string message);
        void TraceVerbose(std::string message, TraceData data);
        void TraceVerbose(std::string message, std::exception exception);
};
