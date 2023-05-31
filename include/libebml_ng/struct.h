#ifndef EBML_NG_STRUCT_H
#define EBML_NG_STRUCT_H

#include <string.h>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <string>
#include <stdexcept>

#ifndef __STDC_IEC_559__
#error "IEEE 754 is required for floating-point types"
#endif

namespace ebml {
    extern bool _is_littleendian;
    void _reverse(char* s, unsigned int j, unsigned int k);

    // Deduction guide for size function
    template<typename T>
    size_t size(const T& value);

    // Deduction guide for pack function
    template<typename T>
    size_t pack(const T& value, char* dest);

    // These functions accept a size_t parameter, but are not required to abide by it
    // if is impossible to encode the data using the given size.
    // All pack functions must return size encoded.

//     size_t pack(const unsigned long long&, size_t, char*);
//     size_t pack(const long long&, size_t, char*);
//     size_t pack(const std::string&, size_t, char*); // For null-terminated strings within larger containers
//         // For this overload, the returned size should just echo back the provided size_t to reflect the
//         // amount of space *allocated* for the string, and not simply the size of the string.

    // These functions do not a size_t parameter, still must return encoded (on-disk) size.
//     size_t pack(const float&, char*);
//     size_t pack(const double&, char*);
//     size_t pack(const std::string&, char*);

    template<typename T>
    size_t pack(const T& value, size_t size, char* dest);

//     size_t pack(const double&, char*);


    template<typename T>
    T unpack(const char*, size_t);

    template<>
    unsigned long long unpack<unsigned long long>(const char*, size_t);

//     template<>
//     long long unpack<long long>(const char*, size_t);

//     template<>
//     float unpack<float>(const char*, size_t);
//
//     template<>
//     double unpack<double>(const char*, size_t);
}

#endif
