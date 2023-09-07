#ifndef EBML_NG_STRUCT_UNICODE_H
#define EBML_NG_STRUCT_UNICODE_H

#include "libebml_ng/struct.h"

namespace ebml {
    size_t size(const std::wstring&);

    size_t pack(const std::wstring&, size_t, char*);

    template<>
    std::wstring unpack<std::wstring>(const char*, size_t);
}
#endif
