#ifndef EBML_NG_DATETIME_H
#define EBML_NG_DATETIME_H

#include <chrono>

#include "libebml_ng/struct.h"

namespace ebml {
    typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> timepoint_t;
    extern const timepoint_t epoch;
    DECL_EXTERN_FUNCTIONS(timepoint_t)
}
#endif
