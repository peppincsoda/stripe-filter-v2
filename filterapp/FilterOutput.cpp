#include "FilterOutput.h"

namespace sfv2 {

    void FilterOutputData::reset()
    {
        left_dist = 0;
        right_dist = 0;
        measurement = -1;
        entropy = 0;
        status = FilterStatus::UnknownStatus;
    }

}
