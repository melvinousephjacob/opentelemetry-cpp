public IDevicePropertyLogger
// DESCRIPTION      : Interface of the 'Device Property Logging'
// NOTES            : None.
//<<===========================================================================

{
    public:
        virtual ~IDevicePropertyLogger() {}

        virtual bool Log(const std::string& textStr) = 0;
        virtual bool getPropertiesFromLastLog( std::string& data_info,
                                                OSTIME_ABS& timestamp_info) = 0;
};
