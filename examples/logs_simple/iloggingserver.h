#pragma once
#include "models.h";
#include <string>
#include <exception>

class ICentralLogServer
{
    public:
        void Log(std::string message, Severity severity);
        void Log(std::string message, Severity severity, std::exception exception);
        void Log(std::string message, Severity severity, InfoCategory infoCategory);
};
