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
    /**
     * @brief Reverses the contents of a character array between indices [j, k].
     *
     * This helper function is used internally to swap the bytes of a raw data representation.
     *
     * @param s Pointer to the character array.
     * @param j The starting index.
     * @param k The ending index.
     */
    inline void _reverse(char* s, unsigned int j, unsigned int k) {
        while (j < k) {
            s[j] ^= s[k];
            s[k] ^= s[j];
            s[j] ^= s[k];
            j++;
            k--;
        }
    }

    /**
     * @brief Computes the size in bytes required to encode the given value.
     *
     * This function should return the number of bytes that will be used
     * to encode the provided value into a raw data format.
     *
     * @tparam T The type of the value to encode.
     * @param value The value to be encoded.
     * @return The number of bytes required for the encoded representation.
     *
     * @throws ebml::ebmlEncodeError If the value cannot be encoded.
     *         (Note: Only exceptions of type ebml::ebmlEncodeError should be thrown.)
     */
    template<typename T>
    inline size_t size(const T& value);

    /**
     * @brief Packs the given value into a raw data buffer.
     *
     * This function writes the encoded representation of the given value into
     * the destination buffer pointed by \a dest, and returns the number of bytes written.
     *
     * @tparam T The type of the value to encode.
     * @param value The value to be encoded.
     * @param dest Pointer to the destination buffer to hold the encoded bytes.
     * @return The number of bytes that were written into the buffer.
     *
     * @throws ebml::ebmlEncodeError If the encoding fails.
     *         (Only exceptions of type ebml::ebmlEncodeError should be thrown.)
     */
    template<typename T>
    inline size_t pack(const T& value, char* dest);

    /**
     * @brief Packs the given value into a raw data buffer with a size constraint.
     *
     * This function writes the encoded representation of \a value into the buffer \a dest.
     * If the number of bytes required to encode the value exceeds the provided \a size,
     * then an ebml::ebmlEncodeError is thrown.
     *
     * @tparam T The type of the value to encode.
     * @param value The value to be encoded.
     * @param size The maximum number of bytes available in the destination buffer.
     * @param dest Pointer to the destination buffer.
     * @return The number of bytes written into the buffer.
     *
     * @throws ebml::ebmlEncodeError If the packed data size exceeds the provided buffer size.
     *         (Only exceptions of type ebml::ebmlEncodeError should be thrown.)
     */
    template<typename T>
    inline size_t pack(const T& value, size_t size, char* dest);

    /**
     * @brief Packs the given value into a std::string.
     *
     * This function creates a std::string whose contents represent the encoded form
     * of the provided \a value.
     *
     * @tparam T The type of the value to encode.
     * @param value The value to be encoded.
     * @return A std::string containing the encoded data.
     *
     * @throws ebml::ebmlEncodeError If the encoding fails.
     *         (Only exceptions of type ebml::ebmlEncodeError should be thrown.)
     */
    template<typename T>
    inline std::string pack(const T& value);

    /**
     * @brief Unpacks a value of type T from a raw data buffer.
     *
     * This function converts the raw bytes (provided by pointer \a src with
     * length \a size) into a value of type T.
     *
     * @tparam T The type to decode.
     * @param src Pointer to the raw data buffer.
     * @param size The size of the data in bytes.
     * @return The decoded value of type T.
     *
     * @throws ebml::ebmlDecodeError If decoding fails.
     *         (Only exceptions of type ebml::ebmlDecodeError should be thrown.)
     */
    template<typename T>
    inline T unpack(const char* src, size_t size);

    /**
     * @brief Unpacks a value of type T from a std::string.
     *
     * This function decodes the contents of the provided \a std::string into a value of type T.
     *
     * @tparam T The type to decode.
     * @param str The std::string containing the encoded data.
     * @return The decoded value of type T.
     *
     * @throws ebml::ebmlDecodeError If decoding fails.
     *         (Only exceptions of type ebml::ebmlDecodeError should be thrown.)
     */
    template<typename T>
    inline T unpack(const std::string& str);
}

#define DEF_SIZE(T) inline size_t size(const T& value)

#define FIXED_SIZE(T, s) \
    inline size_t size(const T&) { \
        return s; \
    }


#define DEF_PACK(T) inline size_t pack(const T& value, size_t size, char* dest)
#define DEF_PACK_ALT(T) inline size_t pack(const T& value, char* dest)
#define DEF_UNPACK(T) \
    template<> \
    inline T unpack<T>(const char* src, size_t size)

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
