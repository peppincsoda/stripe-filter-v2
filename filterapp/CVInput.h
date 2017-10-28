#ifndef CVINPUT_H
#define CVINPUT_H

#include "FilterInput.h"

#include <memory>

namespace cv {
    class VideoCapture;
}

namespace sfv2 {

    class CVInput : public FilterInput
    {
    public:
        CVInput();
        ~CVInput();

        CVInput(const CVInput&) = delete;
        CVInput& operator=(const CVInput&) = delete;

        CVInput(CVInput&&) = default;
        CVInput& operator=(CVInput&&) = default;

        bool open(const FilterSettings& settings) override;
        bool read(FilterInputData& data) override;

    private:
        std::unique_ptr<cv::VideoCapture> vid_cap_;
    };

}

#endif // CVINPUT_H
