#ifndef EBML_NG_DOUBLE_H
#define EBML_NG_DOUBLE_H

#include "libebml_ng/struct.h"

// Until Doxygen supports using macros found in #included headers...

#define DECL_EXTERN_STR_FUNCTIONS(T) \
    extern template std::string pack(const T&); \
    extern template T unpack<T>(const std::string&);


#define DECL_EXTERN_FUNCTIONS_ALT(T) \
    size_t size(const T&); \
    size_t pack(const T&, char*); \
    template<> \
    T unpack<T>(const char*, size_t); \
    extern template size_t pack(const T&, size_t, char*); \
    DECL_EXTERN_STR_FUNCTIONS(T)

namespace ebml {
    DECL_EXTERN_FUNCTIONS_ALT(float)
    DECL_EXTERN_FUNCTIONS_ALT(double)
}

#endif
