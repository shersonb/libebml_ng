#ifndef EBML_NG_REPR_H
#define EBML_NG_REPR_H

#include <string>
#include <memory>

namespace ebml {
    template <typename T>
    std::wstring repr(const std::shared_ptr<T>& obj);

    template <typename T>
    std::wstring repr(const T& obj);

    template <typename T>
    std::wstring repr(T* const obj);

    std::wstring repr(const std::string&);
    std::wstring repr(const std::wstring&);
    std::wstring repr(unsigned long long);
    std::wstring repr(unsigned long);
    std::wstring repr(long long);
    std::wstring repr(long);
    std::wstring repr(double);
}
#endif
