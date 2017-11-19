#ifndef FILTEROUTPUT_H
#define FILTEROUTPUT_H

#include <QObject>

namespace sfv2 {

    class FilterSettings;

    class FilterOutputData : public QObject
    {
        Q_OBJECT
    public:
        enum Status
        {
            OK,
            ProcessingFailed,
            ProcessingInvalidParams,
            InputFailed,
            UnknownStatus = -1,
        };
        Q_ENUM(Status)

        FilterOutputData();
        ~FilterOutputData() = default;

        Status status() const;
        void setStatus(Status status);

        int leftDist() const;
        int rightDist() const;
        int measurement() const;
        void setMeasurement(int left_dist, int right_dist, int measurement);

        double entropy() const;
        void setEntropy(double entropy);

        int fps() const;
        void setFps(int fps);

        void reset();

    private:
        Status status_;

        int left_dist_;
        int right_dist_;
        int measurement_;

        double entropy_;

        int fps_;
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
