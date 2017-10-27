#ifndef FILTEROUTPUT_H
#define FILTEROUTPUT_H

class QSettings;

namespace sfv2 {

    enum class FilterStatus
    {
        OK,
        ProcessingFailed,
        InputFailed,
    };

    struct FilterOutputData
    {
        int measurement;
        FilterStatus status;
    };

    class FilterOutput
    {
    public:
        FilterOutput() {}
        virtual ~FilterOutput() {}
        virtual bool open(const QSettings& settings) = 0;
        virtual bool write(const FilterOutputData& data) = 0;
    };

}


#endif // FILTEROUTPUT_H
