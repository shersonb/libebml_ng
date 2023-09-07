#ifndef EBML_NG_STRUCT_CPP
#define EBML_NG_STRUCT_CPP
#include "libebml_ng/struct.h"
#include "libebml_ng/exceptions.h"
#include <stdio.h>

namespace ebml {
    unsigned long long __one = 1;
    bool _is_littleendian = (((char*)&__one)[0] == 1);

    void _reverse(char* s, unsigned int j, unsigned int k) {
        while (j < k) {
            s[j] ^= s[k];
            s[k] ^= s[j];
            s[j] ^= s[k];

            j++;
            k--;
        };
    }

    template<typename T>
    size_t pack(const T& value, char* dest) {
        size_t packedSize = size(value);
        return pack(value, packedSize, dest);
    }

    template<typename T>
    size_t pack(const T& value, size_t size, char* dest) {
        size_t packedSize = pack(value, dest);
        if (packedSize > size) {
            // The packed data size exceeds the provided size
            throw std::runtime_error("Packed data size exceeds buffer size");
        }
        return packedSize;
    }
}
#endif
