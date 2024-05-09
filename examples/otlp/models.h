#pragma once
#include <string>
#include <list>
#include <utility>
#include <ctime>
#include <map>

enum InfoCategory
{
    Nones = 0,
    OSEvent = 1,
    Installation = 2,
    Performance = 3,
    Security = 4,
    Exception = 5,
    FunctionEntry = 6,
    FunctionExit = 7,
    Interface = 8,
    UIEvent = 9,
    ActivityStart = 10,
    ActivityStop = 11,
    UserAction = 12,
    UserMessage = 13,
    ApplicationEvent = 14
};

enum Severity
{
    None = 0,
    DebugInfo = 1,
    DebugVerbose = 2,
    Info = 3,
    Warning = 4,
    Error = 5,
    Fatal = 6,
    Hazard = 7
};

enum LogType
{
    Trace = 0,
    Standard = 1,
    Development = 2,
    Service = 3,
    Statistical = 4,
    Utilization = 5,
    ServiceUtilization = 6,
    Predictive = 7,
    ServiceAdvanced = 8,
};

struct CoreLogData
{
    std::list<std::pair<std::string, std::string>> AdditionalInfoParameters;
    std::list<std::string> ResolutionParameters;
    std::string Resolution;
    std::string ActivityId;
    std::string Namespace;
    std::string Description;
    std::string HumanReadableEventId;
    int EventId;
    std::string HumanReadableModuleId;
    int ModuleId;
    std::string MachineName;
    std::string ThreadName;
    int ThreadId;
    std::string ProcessName;
    int ProcessId;
    std::list<std::string> DescriptionParameters;
    std::string ContextInfo;
    std::string AdditionalInfo;
    Severity severity;
    std::string EventType;
    LogType logType;
    std::string StackTrace;
    std::string ExceptionInfo;
    std::time_t DateTime;
    std::string SoftwareModule;
    std::string SoftwarePackage;
};
