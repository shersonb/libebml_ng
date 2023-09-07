#ifndef EBML_NG_TEMPLATE_TPP
#define EBML_NG_TEMPLATE_TPP

#include "libebml_ng/base.h"
#include "libebml_ng/template.h"

#include "libebml_ng/struct.h"
#include "libebml_ng/repr.h"
#include <memory>

namespace ebml {
    template<typename T>
    ebmlDataElementClass<T>::ebmlDataElementClass() : ebmlElementClass() {
    }

    template<typename T>
    ebmlDataElementClass<T>::ebmlDataElementClass(ebmlID_t ebmlID, const std::wstring& name) : ebmlElementClass(ebmlID, name) {
        this->defaultval = defaultdefault;
    }

    template<typename T>
    ebmlDataElementClass<T>::ebmlDataElementClass(ebmlID_t ebmlID, const std::wstring& name, const T& defaultval) : ebmlElementClass(ebmlID, name), defaultval(defaultval) {}

    template<typename T>
    ebmlDataElementClass<T>::ebmlDataElementClass(ebmlID_t ebmlID, const std::wstring& name, T&& defaultval) : ebmlElementClass(ebmlID, name), defaultval(std::move(defaultval)) {}

    // template<typename T>
    // ebmlElement_sp ebmlDataElementClass<T>::operator()() const {
    //     auto elem = new ebmlDataElement<T>(this, this->defaultval);
    //     return ebmlElement_sp(elem);
    // }

    template<typename T>
    ebmlElement* ebmlDataElementClass<T>::_new() const {
        return new ebmlDataElement<T>(this, this->defaultval);
    }

    template<typename T>
    ebmlElement_sp ebmlDataElementClass<T>::operator()(const T& value) const {
        auto elem = new ebmlDataElement<T>(this, value);
        return ebmlElement_sp(elem);
    }

    template<typename T>
    ebmlElement_sp ebmlDataElementClass<T>::operator()(T&& value) const {
        auto elem = new ebmlDataElement<T>(this, std::move(value));
        return ebmlElement_sp(elem);
    }

    template<typename T>
    ebmlDataElement<T>::ebmlDataElement(const ebmlDataElementClass<T>* __cls, const T& _data) : ebmlElement(__cls), data(_data) {}

    template<typename T>
    ebmlDataElement<T>::ebmlDataElement(const ebmlDataElementClass<T>* __cls, T&& _data) : ebmlElement(__cls), data(std::move(_data)) {}

    template<typename T>
    size_t ebmlDataElement<T>::dataSize() const {
        return size(this->data);
    }

    template<typename T>
    size_t ebmlDataElement<T>::_encode(char* dest) const {
        size_t _dataSize = this->dataSize();
        return pack(this->data, _dataSize, dest);
    }

    template<typename T>
    void ebmlDataElement<T>::_decode(const parseString& parsed) {
        this->data = unpack<T>(parsed.data, parsed.dataSize);
    }

    template<typename T>
    void ebmlDataElement<T>::_decode(const parseFile& parsed) {
        auto _data = std::unique_ptr<char[]>(new char[parsed.dataSize]);
        parsed.read(_data.get());
        this->data = unpack<T>(_data.get(), parsed.dataSize);
    }

    template<typename T>
    void ebmlDataElement<T>::_clonedata(const ebmlElement* orig) {
        const ebmlDataElement<T>* orig_typed = dynamic_cast<const ebmlDataElement<T>*>(orig);
        if (orig_typed) {
            this->data = orig_typed->data;
        }
    }

    template <typename T>
    ebmlDataElement_sp<T> recast(const ebmlElement_sp& ptr) {
        return std::dynamic_pointer_cast<ebmlDataElement<T>>(ptr);
    }

    template <typename T>
    std::wstring ebmlDataElement<T>::minirepr() const {
        return ebml::repr(this->data);
    }

    template<typename T>
    T& data(const ebmlElement_sp& elem) {
        if (auto recast = std::dynamic_pointer_cast<ebmlDataElement<T>>(elem)) {
            return recast->data;
        }
        throw std::invalid_argument("Incompatible ebmlElement type");
    }

    template<typename T>
    T data(const c_ebmlElement_sp& elem) {
        if (auto recast = std::dynamic_pointer_cast<const ebmlDataElement<T>>(elem)) {
            return recast->data;
        }
        throw std::invalid_argument("Incompatible ebmlElement type");
    }

    template<typename T>
    T& data(ebmlElement* elem) {
        if (auto recast = dynamic_cast<ebmlDataElement<T>*>(elem)) {
            return recast->data;
        }
        throw std::invalid_argument("Incompatible ebmlElement type");
    }

    template<typename T>
    T data(const ebmlElement* elem) {
        if (auto recast = dynamic_cast<const ebmlDataElement<T>*>(elem)) {
            return recast->data;
        }
        throw std::invalid_argument("Incompatible ebmlElement type");
    }
}
#endif
