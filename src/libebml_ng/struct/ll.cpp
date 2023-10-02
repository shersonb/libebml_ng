#ifndef EBML_NG_STRUCT_LL_CPP
#define EBML_NG_STRUCT_LL_CPP

#include "libebml_ng/struct/ll.h"
#include "libebml_ng/struct.tpp"

namespace ebml {
    DEF_SIZE(long long) {
        unsigned long long k = 1;
        long long c = 128;

        while (k < 8) {
            if ((-c <= value) && (value < c)) {
                return k;
            }
            k++;
            c <<= 8;
        }

        return 8;
    }

    DEF_PACK(long long) {
        char* as_chars;

        if (size == 0 or size > 8) {
            throw std::invalid_argument("pack: invalid value for size");
        };

        if (size != 8) {
            if (value > 0 && (value >> (8*size)) != 0) {
                throw std::overflow_error("pack: int too big to convert");
            };
            if (value < 0 && (value >> (8*size)) != -1) {
                throw std::overflow_error("pack: int too big to convert");
            };
        };

        as_chars = (char*)&value;

        if (_is_littleendian) {
            memcpy(dest, as_chars, size);
            _reverse(dest, 0, size - 1);
        } else {
            memcpy(dest, as_chars + (8 - size), size);
        }

        return size;
    }

    DEF_UNPACK(long long) {
        long long ret = 0;
        char* as_chars;

        if (size == 0) {
            return 0;
        };

        if (size > 8) {
            throw std::invalid_argument("unpack<long long>: invalid value for size");
        };

        as_chars = (char*)&ret;

        if (src[0] & -128) {
            ret = -1ll;
        } else {
            ret = 0;
        }

        if (_is_littleendian) {
            memcpy(as_chars, src, size);
            _reverse(as_chars, 0, size - 1);
        } else {
            memcpy(as_chars + (8 - size), src, size);
        }

        return ret;
    }

    INST_TEMPLATES(long long)
}
#endif
