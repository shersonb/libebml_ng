#ifndef EBML_NG_TEMPLATE_TPP
#define EBML_NG_TEMPLATE_TPP

#include "libebml_ng/template.h"
#pragma message("template.tpp included")

#include "libebml_ng/struct.h"
#include <memory>

namespace ebml {
    template<typename T>
    ebmlElementClassTemplate<T>::ebmlElementClassTemplate(ebmlID_t ebmlID, const std::u32string& name) : ebmlElementClassBase(ebmlID, name) {}

    template<typename T>
    ebmlElementClassTemplate<T>::ebmlElementClassTemplate(ebmlID_t ebmlID, const std::u32string& name, const T& defaultval) : ebmlElementClassBase(ebmlID, name), defaultval(defaultval) {}

    template<typename T>
    ebmlElement_sp ebmlElementClassTemplate<T>::operator()() const {
        auto elem = new ebmlElementTemplate<T>(this, this->defaultval);
        return ebmlElement_sp(elem);
    }

    template<typename T>
    ebmlElement_sp ebmlElementClassTemplate<T>::operator()(const T& value) const {
        auto elem = new ebmlElementTemplate<T>(this, value);
        return ebmlElement_sp(elem);
    }

    template<typename T>
    ebmlElementTemplate<T>::ebmlElementTemplate(const ebmlElementClassTemplate<T>* __cls, const T& _data) : ebmlElementBase(__cls), data(_data) {}

    template<typename T>
    size_t ebmlElementTemplate<T>::dataSize() const {
        return size(this->data);
    }

    template<typename T>
    size_t ebmlElementTemplate<T>::_encode(char* dest) const {
        size_t _dataSize = this->dataSize();
        return pack(this->data, _dataSize, dest);
    }

    template<typename T>
    void ebmlElementTemplate<T>::_decode(const parseString& parsed) {
        this->data = unpack<T>(parsed.data, parsed.dataSize);
    }

    template<typename T>
    void ebmlElementTemplate<T>::_decode(const parseFile_sp& parsed) {
//         std::unique_ptr<char[]> _data = std::make_unique<char[]>(parsed->dataSize);
        std::unique_ptr<char[]> _data(new char[parsed->dataSize]);
        parsed->read(_data.get());
        this->data = unpack<T>(_data.get(), parsed->dataSize);
    }

    template<typename T>
    void ebmlElementTemplate<T>::_clonedata(const ebmlElementBase* orig) {
        const ebmlElementTemplate<T>* orig_typed = dynamic_cast<const ebmlElementTemplate<T>*>(orig);
        if (orig_typed) {
            this->data = orig_typed->data;
        }
    }
}
#endif
