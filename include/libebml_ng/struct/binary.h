#ifndef EBML_NG_BINARY_H
#define EBML_NG_BINARY_H

#include "libebml_ng/struct.h"
#include <string>

namespace ebml {
    size_t size(const std::string&);

    size_t pack(const std::string&, char*);

    size_t pack(const std::string&, size_t, char*);
        // For null-terminated strings within larger containers
        // For this overload, the returned size should just echo back the provided size_t to reflect the
        // amount of space *allocated* for the string, and not simply the size of the string.

    template<>
    std::string unpack<std::string>(const char*, size_t);
}

#endif
