#ifndef FILTERINPUT_H
#define FILTERINPUT_H

class QSettings;

namespace cv {
    class Mat;
}

namespace sfv2 {

    struct FilterInputData
    {
        cv::Mat* frame;
    };

    class FilterInput
    {
    public:
        FilterInput() {}
        virtual ~FilterInput() {}
        virtual bool open(const QSettings& settings) = 0;
        virtual bool read(FilterInputData& data) = 0;
    };

}

#endif // FILTERINPUT_H
