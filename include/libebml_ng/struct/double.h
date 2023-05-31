#ifndef EBML_NG_DOUBLE_H
#define EBML_NG_DOUBLE_H

#include "libebml_ng/struct.h"

namespace ebml {
    size_t size(const float&);
    size_t size(const double&);

    size_t pack(const float&, char*);
    size_t pack(const double&, char*);

    template<>
    float unpack<float>(const char*, size_t);

    template<>
    double unpack<double>(const char*, size_t);

}

#endif
