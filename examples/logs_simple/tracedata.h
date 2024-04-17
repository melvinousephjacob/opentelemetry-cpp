#pragma once
#include <string>
#include <list>
#include <utility>
#include <ctime>
#include <map>

struct TraceData
{
    std::string AdditionalInfo;
    std::string ContextInfo;
    std::string EventType;
    std::string StackTrace;
    std::string ExceptionInfo;
    std::time_t DateTime; 
};
 
