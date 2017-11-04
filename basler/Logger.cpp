#include "Logger.h"

#include <cstdio>
#include <cstdarg>

namespace basler {

    static LoggerFn logger_ = nullptr;

    void writeLog(LogLevel level, const char* file, int line, const char* func, const char* format, ...)
    {
        va_list arg_list;
        va_start(arg_list, format);

        char message[1024];
        vsprintf_s(message, format, arg_list);

        logger_(level, file, line, func, message);
        va_end(arg_list);
    }

}

extern "C" void _stdcall BaslerSetLogger(basler::LoggerFn fn)
{
    basler::logger_ = fn;
}
