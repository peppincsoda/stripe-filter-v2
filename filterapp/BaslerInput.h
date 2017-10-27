#ifndef BASLERINPUT_H
#define BASLERINPUT_H

#include "FilterInput.h"

namespace sfv2 {

    class BaslerInput : public FilterInput
    {
    public:
        BaslerInput();
        ~BaslerInput();

        bool open(const QSettings& settings) override;
        bool read(FilterInputData& data) override;
    };

}

#endif // BASLERINPUT_H
