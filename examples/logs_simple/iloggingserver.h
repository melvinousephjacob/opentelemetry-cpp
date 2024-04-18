#pragma once
#include "models.h";
#include <string>
#include <exception>

class ICentralLogServer
{
    public:
        void Log(std::string message, Severity severity, CoreLogData coreLogData);
        void Log(std::string message, Severity severity, std::exception exception, CoreLogData coreLogData);
        void Log(std::string message, Severity severity, InfoCategory infoCategory, CoreLogData coreLogData);

        void LogWithSeverity(nostd::shared_ptr<logs::Logger> logger, Severity severity, std::unordered_map<std::string, std::string> mymap, std::string message);
};
