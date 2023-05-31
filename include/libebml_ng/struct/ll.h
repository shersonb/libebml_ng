#ifndef EBML_NG_LL_H
#define EBML_NG_LL_H

#include "libebml_ng/struct.h"

namespace ebml {
    size_t size(const long long&);

    size_t pack(const long long&, size_t, char*);

    template<>
    long long unpack<long long>(const char*, size_t);
}
#endif
