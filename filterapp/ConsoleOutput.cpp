#include "ConsoleOutput.h"

#include <iomanip>
#include <iostream>

namespace sfv2 {

    static const char* getFilterStatusStr(FilterStatus status)
    {
        switch (status) {
        case FilterStatus::OK:
            return "OK";
        case FilterStatus::ProcessingFailed:
            return "ProcessingFailed";
        case FilterStatus::InputFailed:
            return "InputFailed";
        }
        return "?";
    }

    ConsoleOutput::ConsoleOutput()
    {

    }

    ConsoleOutput::~ConsoleOutput()
    {

    }

    bool ConsoleOutput::open(const FilterSettings& /*settings*/)
    {
        return true;
    }

    bool ConsoleOutput::write(const FilterOutputData& data)
    {
        std::cout << "                                        \r";
        std::cout << std::setw(5) << std::left << data.left_dist
                  << std::setw(5) << std::left << data.right_dist
                  << std::setw(5) << std::left << data.measurement
                  << " " << getFilterStatusStr(data.status) << '\r';
        return true;
    }
}
