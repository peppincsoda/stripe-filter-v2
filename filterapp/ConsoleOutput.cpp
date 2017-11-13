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
        case FilterStatus::ProcessingInvalidParams:
            return "ProcessingInvalidParams";
        case FilterStatus::InputFailed:
            return "InputFailed";
        case FilterStatus::UnknownStatus:
            return "UnknownStatus";
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
        std::cout << " L:" << std::setw(5) << std::right << data.left_dist
                  << " R:" << std::setw(5) << std::right << data.right_dist
                  << " OUTPUT:" << std::setw(5) << std::right << data.measurement
                  << " ENTROPY:" << std::setw(8) << std::right << data.entropy
                  << " STATUS:" << getFilterStatusStr(data.status) << " | \r";
        return true;
    }
}
