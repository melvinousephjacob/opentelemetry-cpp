
#include <string>

class IMetricsLogger
{
    public:
        static void Log(const std::string& textStr);
};
