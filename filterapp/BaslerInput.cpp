#include "BaslerInput.h"
#include <QSettings>
#include <QDebug>

#include <opencv2/opencv.hpp>

namespace sfv2 {

    class BaslerInput::Impl
    {
    public:
        bool open(const QSettings& settings);
        bool read(FilterInputData& data);
    };

    BaslerInput::BaslerInput()
        : pimpl_(std::make_unique<Impl>())
    {

    }

    BaslerInput::~BaslerInput()
    {

    }

    bool BaslerInput::open(const QSettings& settings)
    {
        return pimpl_->open(settings);
    }

    bool BaslerInput::read(FilterInputData& data)
    {
        return pimpl_->read(data);
    }

    bool BaslerInput::Impl::open(const QSettings& /*settings*/)
    {
        return false;
    }

    bool BaslerInput::Impl::read(FilterInputData& /*data*/)
    {
        return false;
    }

}
