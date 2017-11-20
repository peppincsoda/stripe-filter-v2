#ifndef AVERAGECALCULATOR_H
#define AVERAGECALCULATOR_H

#include <deque>

namespace sfv2 {

    template<class T>
    class AverageCalculator
    {
    public:
        AverageCalculator()
            : items_()
            , sum_(0)
            , length_(0)
        {

        }

        void reset()
        {
            items_.clear();
            sum_ = 0;
            length_ = 0;
        }

        void setLength(std::size_t length)
        {
            length_ = length;

            while (length_ < items_.size()) {
                sum_ -= items_.front();
                items_.pop_front();
            }
        }

        void addValue(T value)
        {
            items_.push_back(value);
            sum_ += items_.back();

            if (length_ < items_.size()) {
                sum_ -= items_.front();
                items_.pop_front();
            }
        }

        T value() const
        {
            if (0 < items_.size()) {
                return sum_ / items_.size();
            }

            return 0;
        }

    private:
        std::deque<T> items_;
        T sum_;
        std::size_t length_;
    };

}

#endif // AVERAGECALCULATOR_H
