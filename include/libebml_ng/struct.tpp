#ifndef EBML_NG_STRUCT_TPP
#define EBML_NG_STRUCT_TPP

#include <string>
#include <memory>
#include "struct.h"

namespace ebml {
    template<typename T>
    std::string pack(const T& value) {
        size_t packsize = size(value);
        auto data = std::make_unique<char[]>(packsize);
        pack(value, data.get());
        return std::string(data.get(), packsize);
    }

    template<typename T>
    T unpack(const std::string& str) {
        return unpack<T>(str.c_str(), str.size());
    }
}
#endif
