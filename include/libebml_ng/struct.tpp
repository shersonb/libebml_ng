#ifndef EBML_NG_STRUCT_TPP
#define EBML_NG_STRUCT_TPP

#include <string>
#include <memory>
#include "struct.h"

namespace ebml {
    template<typename T>
    std::string pack(const T& value) {
        size_t packsize = size(value);
        // auto data = std::make_unique<char[]>(packsize);
        // pack(value, data.get());
        // return std::string(data.get(), packsize);
        std::string s;
        s.resize(packsize);
        pack(value, &s[0]);
        return s;
    }

    template<typename T>
    T unpack(const std::string& str) {
        return unpack<T>(str.c_str(), str.size());
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
