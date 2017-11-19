#include "FilterOutput.h"

namespace sfv2 {

    FilterOutputData::FilterOutputData()
        : status_(UnknownStatus)
        , left_dist_(0)
        , right_dist_(0)
        , measurement_(0)
        , entropy_(0)
        , fps_(0)
    {

    }

    FilterOutputData::Status FilterOutputData::status() const
    {
        return status_;
    }

    void FilterOutputData::setStatus(Status status)
    {
        status_ = status;
    }

    int FilterOutputData::leftDist() const
    {
        return left_dist_;
    }

    int FilterOutputData::rightDist() const
    {
        return right_dist_;
    }

    int FilterOutputData::measurement() const
    {
        return measurement_;
    }

    void FilterOutputData::setMeasurement(int left_dist, int right_dist, int measurement)
    {
        left_dist_ = left_dist;
        right_dist_ = right_dist;
        measurement_ = measurement;
    }

    double FilterOutputData::entropy() const
    {
        return entropy_;
    }

    void FilterOutputData::setEntropy(double entropy)
    {
        entropy_ = entropy;
    }

    int FilterOutputData::fps() const
    {
        return fps_;
    }

    void FilterOutputData::setFps(int fps)
    {
        fps_ = fps;
    }

    void FilterOutputData::reset()
    {
        status_ = UnknownStatus;
        left_dist_ = 0;
        right_dist_ = 0;
        measurement_ = 0;
        entropy_ = 0;
        fps_ = 0;
    }

}
