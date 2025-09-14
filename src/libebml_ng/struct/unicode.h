#ifndef EBML_NG_STRUCT_UNICODE_H
#define EBML_NG_STRUCT_UNICODE_H

#include "../struct.h"
#include <string>

namespace ebml {
    size_t _size_utf8(const std::wstring&);
    size_t _pack_utf8(const std::wstring&, size_t, char*);
    std::wstring _unpack_utf8(const char*, size_t);

    DEF_SIZE(std::wstring) {
        return _size_utf8(value);
    }

    DEF_PACK(std::wstring) {
        return _pack_utf8(value, size, dest);
    }

    DEF_UNPACK(std::wstring) {
        return _unpack_utf8(src,  size);
    }
}
#endif
