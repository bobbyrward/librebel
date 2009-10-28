#ifndef LIBREBEL_LOGGING_H_
#define LIBREGEL_LOGGING_H_

namespace rebel {

    class LogOutputHandler {
    public:
        virtual void writeString(tstring const& msg) = 0;
    };

    typedef boost::shared_ptr<LogOutputHandler> LogOutputHandlerPtr;

    typedef enum {
        LogLevel_Debug = 0,
        LogLevel_Info,
        LogLevel_Error,
        LogLevel_Fatal,
        LogLevel_Silent = 0xFFFF,
    } LogLevel;


    class Logging {
    public:

    public:
        static void addOutputHandler(LogOutputHandlerPtr handler);
        static void outputMessage(LogLevel level, tstring const& msg);
        static void outputMessage(LogLevel level, tformat const& fmt);
        static void setMinLevel(LogLevel level);
        static bool _FAILED(HRESULT hr, tstring const& filename, int lineno); 
        static bool _SUCCEEDED(HRESULT hr, tstring const& filename, int lineno); 
    private:
        static std::list<LogOutputHandlerPtr>   _outputHandlers;
        static LogLevel                         _minLevel; 
    };


    class LogOutputHandler_Debugger 
        : public LogOutputHandler
    {
    public:
        virtual void writeString(tstring const& msg);
    };

}


#if !defined(LIBREBEL_NO_LOGGING) || defined(DEBUG)
    #define LOG_DEBUG(msg)  rebel::Logging::outputMessage(rebel::LogLevel_Debug, msg);
    #define LOG_INFO(msg)   rebel::Logging::outputMessage(rebel::LogLevel_Info,  msg);
    #define LOG_ERROR(msg)  rebel::Logging::outputMessage(rebel::LogLevel_Error, msg);
    #define LOG_FATAL(msg)  rebel::Logging::outputMessage(rebel::LogLevel_Fatal, msg);

    #undef FAILED
    #undef SUCCEEDED
    #define FAILED(hr)      rebel::Logging::_FAILED(hr, __TFILE__, __LINE__)
    #define SUCCEEDED(hr)   rebel::Logging::_SUCCEEDED(hr, __TFILE__, __LINE__)
#else 
    #define LOG_DEBUG(msg)
    #define LOG_INFO(msg)
    #define LOG_ERROR(msg)
    #define LOG_FATAL(msg)
#endif


#endif // LIBREGEL_LOGGING_H_

