#ifndef BASLERINPUT_H
#define BASLERINPUT_H

#include "FilterInput.h"

#include <memory>

namespace sfv2 {

    class BaslerInput : public FilterInput
    {
    public:
        BaslerInput();
        ~BaslerInput();

        BaslerInput(const BaslerInput&) = delete;
        BaslerInput& operator=(const BaslerInput&) = delete;

        BaslerInput(BaslerInput&&) = default;
        BaslerInput& operator=(BaslerInput&&) = default;

        bool open(const FilterSettings& settings) override;
        bool read(FilterInputData& data) override;

    private:
        class Impl;
        std::unique_ptr<Impl> pimpl_;
    };

}

#endif // BASLERINPUT_H
