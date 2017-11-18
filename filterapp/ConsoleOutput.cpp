#include "ConsoleOutput.h"

#include <QMetaEnum>

#include <iomanip>
#include <iostream>

namespace sfv2 {

    static const char* getFilterStatusStr(FilterOutputData::Status status)
    {
        return QMetaEnum::fromType<FilterOutputData::Status>()
                .valueToKey(static_cast<int>(status));
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
        std::cout << " L:" << std::setw(5) << std::right << data.leftDist()
                  << " R:" << std::setw(5) << std::right << data.rightDist()
                  << " OUTPUT:" << std::setw(5) << std::right << data.measurement()
                  << " ENTROPY:" << std::setw(8) << std::right << data.entropy()
                  << " STATUS:" << getFilterStatusStr(data.status()) << " | \r";
        return true;
    }
}
