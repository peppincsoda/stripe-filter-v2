#include "BaslerInput.h"
#include "FilterSettings.h"

#include <QDebug>

#include <opencv2/opencv.hpp>

namespace sfv2 {

    class BaslerInput::Impl
    {
    public:
        bool open(const FilterSettings& settings);
        bool read(FilterInputData& data);
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

    bool BaslerInput::Impl::open(const FilterSettings& /*settings*/)
    {
        return false;
    }

    bool BaslerInput::Impl::read(FilterInputData& /*data*/)
    {
        return false;
    }

}
