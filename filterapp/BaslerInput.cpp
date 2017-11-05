#include "BaslerInput.h"
#include "FilterSettings.h"

#include <QDebug>

#include "basler/Camera.h"

#include <opencv2/opencv.hpp>

namespace sfv2 {

    static void STDCALL qtLogger_(basler::LogLevel level,
                                    const char* file,
                                    int line,
                                    const char* func,
                                    const char* message)
    {
        QMessageLogger msg_logger(file, line, func);
        switch (level) {
        case basler::LogLevel::Debug:
            msg_logger.debug(message);
            break;
        case basler::LogLevel::Info:
            msg_logger.info(message);
            break;
        case basler::LogLevel::Warning:
            msg_logger.warning(message);
            break;
        case basler::LogLevel::Critical:
            msg_logger.critical(message);
            break;
        case basler::LogLevel::Fatal:
            msg_logger.fatal(message);
            break;
        }
    }

    class BaslerInput::Impl
    {
    public:
        Impl();
        ~Impl();

        bool open(const FilterSettings& settings);
        bool read(FilterInputData& data);

    private:
        std::unique_ptr<basler::Api> api_;
        std::unique_ptr<basler::Camera> cam_;
        cv::Mat current_frame_;
    };

    BaslerInput::BaslerInput()
        : pimpl_(std::make_unique<Impl>())
    {

    }

    BaslerInput::~BaslerInput()
    {

    }

    bool BaslerInput::open(const FilterSettings& settings)
    {
        return pimpl_->open(settings);
    }

    bool BaslerInput::read(FilterInputData& data)
    {
        return pimpl_->read(data);
    }

    BaslerInput::Impl::Impl()
        : api_()
        , cam_()
    {
    }

    BaslerInput::Impl::~Impl()
    {
    }

    bool BaslerInput::Impl::open(const FilterSettings& /*settings*/)
    {
        if (api_ == nullptr) {
            auto api = std::make_unique<basler::Api>(qtLogger_);
            if (!api->init())
                return false;

            api_ = std::move(api);
            cam_ = std::make_unique<basler::Camera>(api_.get());
        }

        if (!cam_->open())
            return false;

        return true;
    }

    bool BaslerInput::Impl::read(FilterInputData& data)
    {
        basler::GrabFrameResult result;
        if (!cam_->grabFrame(result))
            return false;

        int cv_type = 0;
        switch (result.format) {
        case basler::PixelFormat::Grayscale8bit:
            cv_type = CV_8UC1;
            break;
        default:
            assert(0 && "Pixel format returned by BaslerLib is unknown");
        }

        // Wrap the returned image into a cv::Mat object
        current_frame_ = cv::Mat(result.height,
                                 result.width,
                                 cv_type,
                                 result.data,
                                 result.stride);

        data.frame = &current_frame_;
        return true;
    }

}
