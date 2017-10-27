#ifndef RETRYHANDLER_H
#define RETRYHANDLER_H

#include <memory>
#include <functional>

namespace sfv2 {

    class RetryHandler
    {
    public:
        RetryHandler(int max_init_attempts,
                     int init_delay_ms,
                     int delay_ms,
                     std::function<bool ()> fn);
        ~RetryHandler();

        bool succeeded();
        void reset();

    private:
        class Impl;
        std::unique_ptr<Impl> pimpl_;
    };
}

#endif // RETRYHANDLER_H
