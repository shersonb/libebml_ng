#ifndef EBML_NG_TEMPLATE_TPP
#define EBML_NG_TEMPLATE_TPP

#include "base.h"
#include "template.h"
#include "struct.tpp"
#include "repr.h"
#include "vint.h"
#include "exceptions.h"
#include <memory>

namespace ebml {
    template<typename T>
    ebmlDataElementClass<T>::ebmlDataElementClass() : ebmlElementClass() {
    }

    template<typename T>
    ebmlDataElementClass<T>::ebmlDataElementClass(const char* ebmlID, const std::wstring& name)
    : ebmlDataElementClass(unpackVint(ebmlID), name) {}

    template<typename T>
    ebmlDataElementClass<T>::ebmlDataElementClass(const char* ebmlID, const std::wstring& name, const T& defaultval)
    : ebmlDataElementClass(unpackVint(ebmlID), name, defaultval) {}

    template<typename T>
    ebmlDataElementClass<T>::ebmlDataElementClass(const char* ebmlID, const std::wstring& name, T&& defaultval)
    : ebmlDataElementClass(unpackVint(ebmlID), name, std::move(defaultval)) {}

    template<typename T>
    ebmlDataElementClass<T>::ebmlDataElementClass(ebmlID_t ebmlID, const std::wstring& name) : ebmlElementClass(ebmlID, name) {
        this->defaultval = defaultdefault;
    }

    template<typename T>
    ebmlDataElementClass<T>::ebmlDataElementClass(ebmlID_t ebmlID, const std::wstring& name, const T& defaultval) : ebmlElementClass(ebmlID, name), defaultval(defaultval) {}

    template<typename T>
    ebmlDataElementClass<T>::ebmlDataElementClass(ebmlID_t ebmlID, const std::wstring& name, T&& defaultval) : ebmlElementClass(ebmlID, name), defaultval(std::move(defaultval)) {}

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
        auto _data = std::make_unique<char[]>(parsed.dataSize);
        parsed.read(_data.get(), 0, parsed.dataSize);
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
        return elem->ref<ebmlDataElement<T>>().data;
    }

    template<typename T>
    const T& data(const c_ebmlElement_sp& elem) {
        return elem->ref<const ebmlDataElement<T>>().data;
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


    template<typename T>
    ebmlDataElementClass<const T>::ebmlDataElementClass() : ebmlElementClass(), defaultval(defaultdefault) {
    }

    template<typename T>
    ebmlDataElementClass<const T>::ebmlDataElementClass(const char* ebmlID, const std::wstring& name)
    : ebmlDataElementClass(unpackVint(ebmlID), name, defaultdefault) {}

    template<typename T>
    ebmlDataElementClass<const T>::ebmlDataElementClass(const char* ebmlID, const std::wstring& name, const T& defaultval)
    : ebmlDataElementClass(unpackVint(ebmlID), name, defaultval) {}

    template<typename T>
    ebmlDataElementClass<const T>::ebmlDataElementClass(const char* ebmlID, const std::wstring& name, T&& defaultval)
    : ebmlDataElementClass(unpackVint(ebmlID), name, std::move(defaultval)) {}

    template<typename T>
    ebmlDataElementClass<const T>::ebmlDataElementClass(ebmlID_t ebmlID, const std::wstring& name)
    : ebmlDataElementClass(ebmlID, name, defaultdefault) {}

    template<typename T>
    ebmlDataElementClass<const T>::ebmlDataElementClass(ebmlID_t ebmlID, const std::wstring& name, const T& defaultval) : ebmlElementClass(ebmlID, name), defaultval(defaultval) {}

    template<typename T>
    ebmlDataElementClass<const T>::ebmlDataElementClass(ebmlID_t ebmlID, const std::wstring& name, T&& defaultval) : ebmlElementClass(ebmlID, name), defaultval(std::move(defaultval)) {}

    template<typename T>
    ebmlElement* ebmlDataElementClass<const T>::_new() const {
        return new ebmlDataElement<const T>(this, this->defaultval);
    }

    template<typename T>
    ebmlElement_sp ebmlDataElementClass<const T>::operator()(const T& value) const {
        auto elem = new ebmlDataElement<const T>(this, value);
        return ebmlElement_sp(elem);
    }

    template<typename T>
    ebmlElement_sp ebmlDataElementClass<const T>::operator()(T&& value) const {
        auto elem = new ebmlDataElement<const T>(this, std::move(value));
        return ebmlElement_sp(elem);
    }

    template<typename T>
    ebmlElement_sp ebmlDataElementClass<const T>::decode(const parseString& parsed) const {
        ebmlElement_sp elem;

        if (parsed.ebmlID != this->ebmlID) {
            throw ebmlNoMatch("EBML ID does not match.", this, 0, 0, 0);
        }

        T data = unpack<T>(parsed.data, parsed.dataSize);
        elem = (*this)(std::move(data));

        // try {
        //     elem->_decode(parsed);
        // } catch (ebmlDecodeError& e) {
        //     if (e.cls == nullptr) {
        //         e.cls = this;
        //         e.headSize = parsed.ebmlIDWidth + parsed.sizeWidth;
        //     }
        //     throw;
        // }

        return elem;
    }

    template<typename T>
    ebmlElement_sp ebmlDataElementClass<const T>::decode(const parseFile& parsed) const {
        ebmlElement_sp elem;

        if (parsed.ebmlID != this->ebmlID) {
            throw ebmlNoMatch("EBML ID does not match.", this, parsed.offset);
        }

        auto chardata = std::make_unique<char[]>(parsed.dataSize);
        auto s_parsed = parseString(parsed, chardata.get());
        T data = unpack<T>(s_parsed.data, s_parsed.dataSize);
        elem = (*this)(std::move(data));
        // elem->_decode(parsed);
        return elem;
    }

    template<typename T>
    ebmlDataElement<const T>::ebmlDataElement(const ebmlDataElementClass<const T>* __cls, const T& _data) : ebmlElement(__cls), data(_data) {}

    template<typename T>
    ebmlDataElement<const T>::ebmlDataElement(const ebmlDataElementClass<const T>* __cls, T&& _data) : ebmlElement(__cls), data(std::move(_data)) {}

    template<typename T>
    const ebmlDataElementClass<const T>* ebmlDataElement<const T>::cls() const {
        return dynamic_cast<const ebmlDataElementClass<const T>*>(this->_cls);
    }

    template<typename T>
    ebmlElement_sp ebmlDataElement<const T>::clone() const {
        ebmlElement_sp elem = (*this->cls())(this->data);
        return elem;
    }

    template<typename T>
    size_t ebmlDataElement<const T>::dataSize() const {
        return size(this->data);
    }

    template<typename T>
    size_t ebmlDataElement<const T>::_encode(char* dest) const {
        size_t _dataSize = this->dataSize();
        return pack(this->data, _dataSize, dest);
    }

    // The next three member functions have empty implementation because they cannot be deleted

    template<typename T>
    void ebmlDataElement<const T>::_decode(const parseString&) {
    }

    template<typename T>
    void ebmlDataElement<const T>::_decode(const parseFile&) {
    }

    template<typename T>
    void ebmlDataElement<const T>::_clonedata(const ebmlElement*) {
    }

    template <typename T>
    std::wstring ebmlDataElement<const T>::minirepr() const {
        return ebml::repr(this->data);
    }

    template <typename T>
    ebmlElement_sp ebmlDataElementClass<const T>::_cdecode(const parseString& parsed) const {
        if (parsed.ebmlID != this->ebmlID) {
            throw ebmlNoMatch("EBML ID does not match.", this, 0, 0, 0);
        }

        T data = unpack<T>(parsed.data, parsed.dataSize);
        auto elem = (*this)(std::move(data));
        return ebmlElement_sp(elem);
    }

    template <typename T>
    ebmlElement_sp ebmlDataElementClass<const T>::_cdecode(const parseFile& parsed) const {
        auto chars = std::make_unique<char[]>(parsed.dataSize);
        auto s_parsed = parseString(parsed, chars.get());
        return this->_cdecode(s_parsed);
    }

}
#endif
