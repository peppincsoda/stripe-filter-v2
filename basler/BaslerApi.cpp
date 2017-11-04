#include "BaslerApi.h"

namespace basler {

    Api::Api(LoggerFn logger)
    {
        BaslerSetLogger(logger);
    }

    Api::~Api()
    {

    }

    bool Api::init()
    {
        return true;
    }
}
