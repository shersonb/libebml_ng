#ifndef EBML_NG_BINARY_H
#define EBML_NG_BINARY_H

#include <string>
#include "../struct.h"

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
}

#endif
