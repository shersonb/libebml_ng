#ifndef EBML_NG_STRUCT_ULL_CPP
#define EBML_NG_STRUCT_ULL_CPP

#include "libebml_ng/struct/ull.h"
#include "libebml_ng/struct.tpp"

#include "libebml_ng/struct/binary.h"
#include "libebml_ng/repr.h"
#include <string>
#include <iostream>

namespace ebml {
    DEF_SIZE(unsigned long long) {
        unsigned long long k = 1;
        unsigned long long c = 256;

        while (k < 8) {
            if (value < c) {
                return k;
            }
            k++;
            c <<= 8;
        }

        return 8;
    }

    DEF_PACK(unsigned long long) {
        char* as_chars;

        if (size == 0 or size > 8) {
            throw std::invalid_argument("pack: invalid value for size");
        };

        if (size != 8 and (value >> (8*size)) != 0) {
            throw std::overflow_error("pack: int too big to convert");
        };

        as_chars = (char*)&value;

        if (std::endian::native == std::endian::little) {
            memcpy(dest, as_chars, size);
            _reverse(dest, 0, size - 1);
        } else {
            memcpy(dest, as_chars + (8 - size), size);
        }

        return size;
    }

    DEF_UNPACK(unsigned long long) {
        unsigned long long ret = 0;
        char* as_chars = (char*)&ret;

        if (size == 0) {
            return 0;
        };

        if (size > 8) {
            throw std::invalid_argument("unpack<unsigned long long>: invalid value for size");
        };

        if (std::endian::native == std::endian::little) {
            memcpy(as_chars, src, size);
            _reverse(as_chars, 0, size - 1);
        } else {
            memcpy(as_chars + (8 - size), src, size);
        }

        return ret;
    }

    INST_TEMPLATES(unsigned long long)
}
#endif
