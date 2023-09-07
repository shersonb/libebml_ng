#ifndef EBML_NG_STRUCT_LL_CPP
#define EBML_NG_STRUCT_LL_CPP
// #include "libebml_ng/struct.cpp"
#include "libebml_ng/struct/ll.h"

namespace ebml {
    size_t size(const long long& n) {
        unsigned long long k = 1;
        long long c = 128;

        while (k < 8) {
            if ((-c <= n) && (n < c)) {
                return k;
            }
            k++;
            c <<= 8;
        }

        return 8;
    }

    size_t pack(const long long& n, size_t size, char* dest) {
        char* as_chars;

        if (size == 0 or size > 8) {
            throw std::invalid_argument("pack: invalid value for size");
        };

        if (size != 8) {
            if (n > 0 && (n >> (8*size)) != 0) {
                throw std::overflow_error("pack: int too big to convert");
            };
            if (n < 0 && (n >> (8*size)) != -1) {
                throw std::overflow_error("pack: int too big to convert");
            };
        };

        as_chars = (char*)&n;

        if (_is_littleendian) {
            memcpy(dest, as_chars, size);
            _reverse(dest, 0, size - 1);
        } else {
            memcpy(dest, as_chars + (8 - size), size);
        }

        return size;
    }

    template<>
    long long unpack<long long>(const char* s, size_t size) {
        long long ret = 0;
        char* as_chars;

        if (size == 0) {
            return 0;
        };

        if (size > 8) {
            throw std::invalid_argument("unpack<long long>: invalid value for size");
        };

        as_chars = (char*)&ret;

        if (s[0] & -128) {
            ret = -1ll;
        } else {
            ret = 0;
        }

        if (_is_littleendian) {
            memcpy(as_chars, s, size);
            _reverse(as_chars, 0, size - 1);
        } else {
            memcpy(as_chars + (8 - size), s, size);
        }

        return ret;
    }
}
#endif
