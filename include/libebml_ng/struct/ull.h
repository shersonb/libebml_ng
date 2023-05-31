#ifndef EBML_NG_ULL_H
#define EBML_NG_ULL_H

#include "libebml_ng/struct.h"

namespace ebml {
    size_t size(const unsigned long long&);

    size_t pack(const unsigned long long&, size_t, char*);

    template<>
    unsigned long long unpack<unsigned long long>(const char*, size_t);
}
#endif
