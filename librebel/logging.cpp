#include "stdincludes.h"
#include "logging.h"
#include <cstdarg>

typedef std::list<rebel::LogOutputHandlerPtr> HandlerList;

HandlerList rebel::Logging::_outputHandlers;
rebel::LogLevel rebel::Logging::_minLevel = rebel::LogLevel_Error;


void rebel::Logging::addOutputHandler(rebel::LogOutputHandlerPtr handler) {
    _outputHandlers.push_back(handler);
}

void rebel::Logging::setMinLevel(rebel::LogLevel level) {
    _minLevel = level;
}

static rebel::tstring   LogLevelPrefixes[] = {
    _T("DEBUG: "),
    _T("INFO: "),
    _T("ERROR: "),
    _T("FATAL: "),
};

void rebel::Logging::outputMessage(rebel::LogLevel level, rebel::tstring const& msg) {
    if(level >= _minLevel) {
        rebel::tstring _msg = LogLevelPrefixes[level] + msg;

        HandlerList::iterator iter = _outputHandlers.begin();
        for(; iter != _outputHandlers.end(); ++iter) {
            (*iter)->writeString(_msg);
        }
    }
}

void rebel::Logging::outputMessage(rebel::LogLevel level, rebel::tformat const& fmt) {
    if(level >= _minLevel) {
        outputMessage(level, boost::str(fmt));
    }
}

bool rebel::Logging::_SUCCEEDED(HRESULT hr, tstring const& filename, int lineno) {
    if(hr < 0) return false;
    LOG_INFO(tformat(_T("%s(%d): %s - %s\n")) % filename % lineno % DXGetErrorString9(hr) % DXGetErrorDescription9(hr));
    return true;
}

bool rebel::Logging::_FAILED(HRESULT hr, rebel::tstring const& filename, int lineno) {
    if(hr >= 0) return false;
    LOG_INFO(tformat(_T("%s(%d): %s - %s\n")) % filename % lineno % DXGetErrorString9(hr) % DXGetErrorDescription9(hr));
    return true;
}

void rebel::LogOutputHandler_Debugger::writeString(rebel::tstring const& msg) {
    OutputDebugString(msg.c_str());
}



