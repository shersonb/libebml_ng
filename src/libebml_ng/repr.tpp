#ifndef EBML_NG_REPR_TPP
#define EBML_NG_REPR_TPP
#include "cxxabi.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

namespace ebml {
    template <typename T>
    std::string repr(T* const obj) {
        int status;
        std::stringstream ss;
        std::string mangled = typeid(T).name();
        char* demangled = abi::__cxa_demangle(mangled.c_str(), 0, 0, &status);
        ss << "<pointer to " << demangled << " object at 0x" << reinterpret_cast<uintptr_t>(obj) << ">";
        return ss.str();
    }

    template <typename T>
    std::string repr(const std::shared_ptr<T>& obj) {
        int status;
        std::stringstream ss;
        std::string mangled = typeid(T).name();
        char* demangled = abi::__cxa_demangle(mangled.c_str(), 0, 0, &status);
        ss << "<shared pointer to " << demangled << " object at 0x" << reinterpret_cast<uintptr_t>(obj.get()) << ">";
        return ss.str();
    }

    template <typename T>
    std::string repr(const T& obj) {
        int status;
        std::stringstream ss;
        std::string mangled = typeid(T).name();
        char* demangled = abi::__cxa_demangle(mangled.c_str(), 0, 0, &status);
        ss << "<" << demangled << " object at 0x" << reinterpret_cast<uintptr_t>(&obj) << ">";
        return ss.str();
    }
}
#endif
