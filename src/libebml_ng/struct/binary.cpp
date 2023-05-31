#ifndef EBML_NG_STRUCT_BINARY_CPP
#define EBML_NG_STRUCT_BINARY_CPP

#include "libebml_ng/struct/binary.h"
#include <string.h>

namespace ebml {
    size_t size(const std::string& s) {
        return s.size();
    }

    size_t pack(const std::string& s, char* dest) {
        memcpy(dest, s.data(), s.size());
        return s.size();
    }

    size_t pack(const std::string& s, size_t size, char* dest) {
        if (size < s.size()) {
            throw std::runtime_error("Size of string too large to fit.");
        } else if (size > s.size()) {
            memcpy(dest, s.data(), s.size());
            dest[s.size()] = 0;
            return size;
        }
        memcpy(dest, s.data(), s.size());
        return s.size();
    }

    template<>
    std::string unpack<std::string>(const char* s, size_t size) {
        return std::string(s, size);
    }
}
#endif
