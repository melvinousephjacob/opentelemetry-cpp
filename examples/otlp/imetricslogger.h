#include <string>

class IMetricsLogger
{
    public:
        void Log(const std::string& textStr);
};
