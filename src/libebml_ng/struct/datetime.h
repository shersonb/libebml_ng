#ifndef EBML_NG_DATETIME_H
#define EBML_NG_DATETIME_H

#include <chrono>

#include "../struct.h"
#include "ll.h"

namespace ebml {
    typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> timepoint_t;
    extern const timepoint_t epoch;

    DEF_SIZE(timepoint_t) {
        long long N = std::chrono::duration_cast<std::chrono::nanoseconds>(value - epoch).count();
        return size(N);
    }

    DEF_PACK(timepoint_t) {
        long long N = std::chrono::duration_cast<std::chrono::nanoseconds>(value - epoch).count();
        return pack(N, size, dest);
    }

    DEF_UNPACK(timepoint_t) {
        long long N = unpack<long long>(src, size);
        return timepoint_t(epoch.time_since_epoch() + std::chrono::duration<long long, std::nano>(N));
    }
}
#endif
