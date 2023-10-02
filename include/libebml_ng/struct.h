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
    std::string pack(const T& value);

    template<typename T>
    size_t pack(const T& value, char* dest);

    template<typename T>
    T unpack(const char*, size_t);

    template<typename T>
    T unpack(const std::string&);

}

#define DEF_SIZE(T) size_t size(const T& value)

#define FIXED_SIZE(T, s) \
    size_t size(const T&) { \
        return s; \
    }


#define DEF_PACK(T) size_t pack(const T& value, size_t size, char* dest)
#define DEF_PACK_ALT(T) size_t pack(const T& value, char* dest)
#define DEF_UNPACK(T) \
    template<> \
    T unpack<T>(const char* src, size_t size)

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

#define DECL_EXTERN_FUNCTIONS_ALT(T) \
    size_t size(const T&); \
    size_t pack(const T&, char*); \
    template<> \
    T unpack<T>(const char*, size_t); \
    extern template size_t pack(const T&, size_t, char*); \
    DECL_EXTERN_STR_FUNCTIONS(T)

#define DECL_EXTERN_FUNCTIONS_ALT2(T) \
    size_t size(const T&); \
    size_t pack(const T&, char*); \
    size_t pack(const T&, size_t, char*); \
    template<> \
    T unpack<T>(const char*, size_t); \
    DECL_EXTERN_STR_FUNCTIONS(T)

#define INST_STRING_TEMPLATES(T) \
    template std::string pack(const T&); \
    template T unpack<T>(const std::string&);

#define INST_TEMPLATES(T) \
    template size_t pack(const T&, char*); \
    INST_STRING_TEMPLATES(T)

#define INST_TEMPLATES_ALT(T) \
    template size_t pack(const T&, size_t, char*); \
    INST_STRING_TEMPLATES(T)

#endif
