#ifndef EBML_NG_STRUCT_H
#define EBML_NG_STRUCT_H

#include <string.h>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <string>
#include <stdexcept>
#include <memory>

#ifndef __STDC_IEC_559__
#error "IEEE 754 is required for floating-point types"
#endif

namespace ebml {
    extern bool _is_littleendian;
    void _reverse(char* s, unsigned int j, unsigned int k);

    template<typename T>
    size_t size(const T& value);

    // These two pack template functions have default implemenetations that reference each other.
    // Therefore, it is necessary to specialize one of them for each type T.

    template<typename T>
    size_t pack(const T& value, char* dest);

    template<typename T>
    size_t pack(const T& value, size_t size, char* dest);

    template<typename T>
    T unpack(const char*, size_t);
}

#endif
