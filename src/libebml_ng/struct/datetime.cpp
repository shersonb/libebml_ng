#ifndef EBML_NG_STRUCT_DATETIME_CPP
#define EBML_NG_STRUCT_DATETIME_CPP

#include "libebml_ng/struct/ll.h"
#include "libebml_ng/struct/datetime.h"
#include "libebml_ng/struct.tpp"
#include <iostream>

namespace ebml {
    const timepoint_t epoch = timepoint_t(std::chrono::duration<long long, std::nano>(978307200000000000LL));

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

    INST_TEMPLATES(timepoint_t)
}
#endif
