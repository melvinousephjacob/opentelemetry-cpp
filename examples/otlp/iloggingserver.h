#pragma once
#include "opentelemetry/models.h";
#include <string>
#include <exception>

class ICentralLogServer
{
    public:
        void Log(std::string message, Severity severity, CoreLogData coreLogData);
        void Log(std::string message, Severity severity, std::exception exception, CoreLogData coreLogData);
        void Log(std::string message, Severity severity, InfoCategory infoCategory, CoreLogData coreLogData);
};