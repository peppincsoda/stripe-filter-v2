#include "BaslerInput.h"
#include <QSettings>
#include <QDebug>

#include <opencv2/opencv.hpp>

namespace sfv2 {

    BaslerInput::BaslerInput()
    {

    }

    BaslerInput::~BaslerInput()
    {

    }

    bool BaslerInput::open(const QSettings& /*settings*/)
    {
        return false;
    }

    bool BaslerInput::read(FilterInputData& /*data*/)
    {
        return false;
    }

}
