#ifndef FILTERINPUT_H
#define FILTERINPUT_H

namespace cv {
    class Mat;
}

namespace sfv2 {

    class FilterSettings;

    struct FilterInputData
    {
        cv::Mat* frame;
    };

    class FilterInput
    {
    public:
        FilterInput() {}
        virtual ~FilterInput() {}
        virtual bool open(const FilterSettings& settings) = 0;
        virtual bool read(FilterInputData& data) = 0;
    };

}

#endif // FILTERINPUT_H
