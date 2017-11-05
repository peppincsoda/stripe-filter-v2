#ifndef LOGGER_API_H
#define LOGGER_API_H

#include "Definitions.h"

namespace basler {

    enum class LogLevel
    {
        Debug,
        Info,
        Warning,
        Critical,
        Fatal,
    };

    typedef void (STDCALL *LoggerFn)(LogLevel level,
                                      const char* file,
                                      int line,
                                      const char* func,
                                      const char* message);

    typedef void (STDCALL *SetLoggerFn)(LoggerFn);
}

#ifndef __MINGW32__
extern "C" void STDCALL BaslerSetLogger(basler::LoggerFn fn);
#endif // !__MINGW32__

#endif // LOGGER_API_H
