#ifndef LOGGER_API_H
#define LOGGER_API_H

namespace basler {

    enum class LogLevel
    {
        Debug,
        Info,
        Warning,
        Critical,
        Fatal,
    };

    typedef void (_stdcall *LoggerFn)(LogLevel level,
                                      const char* file,
                                      int line,
                                      const char* func,
                                      const char* message);

    typedef void (_stdcall *SetLoggerFn)(LoggerFn);
}

#ifndef __MINGW32__
extern "C" void _stdcall BaslerSetLogger(basler::LoggerFn fn);
#endif // !__MINGW32__

#endif // LOGGER_API_H
