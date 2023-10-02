#ifndef EBML_NG_STRUCT_CPP
#define EBML_NG_STRUCT_CPP

#include <stdio.h>

#include "libebml_ng/struct.h"
#include "libebml_ng/exceptions.h"

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
}
#endif
