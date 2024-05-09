#include <string>

class IDevicePropertyLogger
{
    public:
        virtual ~IDevicePropertyLogger() {}

        virtual bool Log(const std::string& textStr) = 0;
};
