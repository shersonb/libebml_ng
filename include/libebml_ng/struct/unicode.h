#ifndef EBML_NG_STRUCT_UNICODE_H
#define EBML_NG_STRUCT_UNICODE_H

#include "libebml_ng/struct.h"
#include <string>

// Until Doxygen supports using macros found in #included headers...

#define DECL_EXTERN_STR_FUNCTIONS(T) \
    extern template std::string pack(const T&); \
    extern template T unpack<T>(const std::string&);

#define DECL_EXTERN_FUNCTIONS(T) \
    size_t size(const T&); \
    size_t pack(const T&, size_t, char*); \
    template<> \
    T unpack<T>(const char*, size_t); \
    extern template size_t pack(const T&, char*); \
    DECL_EXTERN_STR_FUNCTIONS(T)

namespace ebml {
    DECL_EXTERN_FUNCTIONS(std::wstring)
}
#endif
