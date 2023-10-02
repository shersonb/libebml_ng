#ifndef EBML_NG_STRUCT_BINARY_CPP
#define EBML_NG_STRUCT_BINARY_CPP

#include <string.h>

#include "libebml_ng/struct/binary.h"
#include "libebml_ng/struct.tpp"

namespace ebml {
    DEF_SIZE(std::string) {
        return value.size();
    }

    DEF_PACK(std::string) {
        if (size < value.size()) {
            throw std::runtime_error("Size of string too large to fit.");
        } else if (size > value.size()) {
            memcpy(dest, value.data(), value.size());
            dest[value.size()] = 0;
            return size;
        }
        memcpy(dest, value.data(), value.size());
        return value.size();
    }

    DEF_PACK_ALT(std::string) {
        memcpy(dest, value.data(), value.size());
        return value.size();
    }

    DEF_UNPACK(std::string) {
        return std::string(src, size);
    }

    INST_STRING_TEMPLATES(std::string)
}
#endif
