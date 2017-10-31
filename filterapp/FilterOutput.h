#ifndef FILTEROUTPUT_H
#define FILTEROUTPUT_H

namespace sfv2 {

    class FilterSettings;

    enum class FilterStatus
    {
        OK,
        ProcessingFailed,
        InputFailed,
    };

    struct FilterOutputData
    {
        int left_dist;
        int right_dist;
        int measurement;
        FilterStatus status;
    };

    class FilterOutput
    {
    public:
        FilterOutput() {}
        virtual ~FilterOutput() {}
        virtual bool open(const FilterSettings& settings) = 0;
        virtual bool write(const FilterOutputData& data) = 0;
    };

}


#endif // FILTEROUTPUT_H
