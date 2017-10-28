#ifndef CONSOLEOUTPUT_H
#define CONSOLEOUTPUT_H

#include "FilterOutput.h"

namespace sfv2 {

    class ConsoleOutput : public FilterOutput
    {
    public:
        ConsoleOutput();
        ~ConsoleOutput();

        bool open(const FilterSettings& settings) override;
        bool write(const FilterOutputData& data) override;
    };

}

#endif // CONSOLEOUTPUT_H
