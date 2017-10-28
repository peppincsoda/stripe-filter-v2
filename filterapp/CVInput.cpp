#include "CVInput.h"
#include "FilterSettings.h"

#include <QDebug>

#include <opencv2/opencv.hpp>

namespace sfv2 {

    CVInput::CVInput()
    {

    }

    CVInput::~CVInput()
    {

    }

    bool CVInput::open(const FilterSettings& /*settings*/)
    {
        vid_cap_ = std::make_unique<cv::VideoCapture>(0);
        if (!vid_cap_->isOpened()) {
            qWarning() << "Failed to open the default camera";
            return false;
        }

        return true;
    }

    bool CVInput::read(FilterInputData& data)
    {
        assert(data.frame != nullptr);
        return vid_cap_->read(*data.frame);
    }

}
