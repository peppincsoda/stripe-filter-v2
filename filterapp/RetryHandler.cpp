#include "RetryHandler.h"
#include <QElapsedTimer>
#include <QDebug>

namespace sfv2 {

    class RetryHandler::Impl
    {
    public:
        Impl(int max_init_attempts,
             int init_delay_ms,
             int delay_ms,
             std::function<bool ()> fn);
        ~Impl();

        bool succeeded();
        void reset();

    private:
        void retry();

        int max_init_attempts_;
        int init_delay_ms_;
        int delay_ms_;
        std::function<bool ()> fn_;

        bool fn_succeeded_;
        QElapsedTimer retry_timer_;
        int num_attempts_;
    };

    RetryHandler::RetryHandler(int max_init_attempts,
                               int init_delay_ms,
                               int delay_ms,
                               std::function<bool ()> fn)
    {
        pimpl_ = std::make_unique<Impl>(max_init_attempts,
                                        init_delay_ms,
                                        delay_ms,
                                        fn);
    }

    RetryHandler::~RetryHandler()
    {

    }

    bool RetryHandler::succeeded()
    {
        return pimpl_->succeeded();
    }

    void RetryHandler::reset()
    {
        pimpl_->reset();
    }

    RetryHandler::Impl::Impl(int max_init_attempts,
                             int init_delay_ms,
                             int delay_ms,
                             std::function<bool ()> fn)
        : max_init_attempts_(max_init_attempts)
        , init_delay_ms_(init_delay_ms)
        , delay_ms_(delay_ms)
        , fn_(fn)

        , fn_succeeded_(false)
        , retry_timer_()
        , num_attempts_(0)
    {

    }

    RetryHandler::Impl::~Impl()
    {

    }

    bool RetryHandler::Impl::succeeded()
    {
        if (!retry_timer_.isValid()) {
            retry();
        } else {
            const auto elapsed_ms = retry_timer_.elapsed();
            if (!fn_succeeded_ &&
                ((num_attempts_ < max_init_attempts_ && elapsed_ms >= init_delay_ms_) ||
                elapsed_ms >= delay_ms_)) {
                retry();
            }
        }

        return fn_succeeded_;
    }

    void RetryHandler::Impl::retry()
    {
        fn_succeeded_ = fn_();
        retry_timer_.start();
        num_attempts_++;

        if (!fn_succeeded_) {
            const auto delay_ms = static_cast<float>(num_attempts_ < max_init_attempts_ ?
                init_delay_ms_ : delay_ms_);

            qInfo() << "Retrying in " << (delay_ms / 1000) << " second(s)...";
        }
    }

    void RetryHandler::Impl::reset()
    {
        fn_succeeded_ = false;
        retry_timer_.start();
        num_attempts_ = 0;
    }
}
