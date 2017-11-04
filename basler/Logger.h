#ifndef LOGGER_H
#define LOGGER_H

#include "LoggerApi.h"

#ifndef __MINGW32__

#define LOG_DEBUG(...)    basler::writeLog(basler::LogLevel::Debug   , __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__) 
#define LOG_INFO(...)     basler::writeLog(basler::LogLevel::Info    , __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_WARNING(...)  basler::writeLog(basler::LogLevel::Warning , __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_CRITICAL(...) basler::writeLog(basler::LogLevel::Critical, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_FATAL(...)    basler::writeLog(basler::LogLevel::Fatal   , __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

namespace basler {

    void writeLog(LogLevel level, const char* file, int line, const char* func, const char* format, ...);

}

#endif // !__MINGW32__

#endif // LOGGER_H
