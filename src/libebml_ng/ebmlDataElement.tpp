#ifndef EBML_NG_DATAELEMENT_TPP
#define EBML_NG_DATAELEMENT_TPP

// #include "base.h"
// #include "template.h"
// #include "struct.tpp"
#include "repr.h"
#include "vint.h"
// #include "exceptions.h"
// #include <memory>
#include "ebmlElementType.tpp"
#include "ebmlElement.tpp"

namespace ebml {
    // template<typename T>
    // ebmlDataType<T>::ebmlDataType() : ebmlElementClass() {
    // }

    template<typename T>
    ebmlDataType<T>::ebmlDataType(const char* ebmlID, const std::wstring& name)
    : ebmlDataType(unpackVint(ebmlID), name) {}

    template<typename T>
    ebmlDataType<T>::ebmlDataType(const char* ebmlID, const std::wstring& name, const T& defaultval)
    : ebmlDataType(unpackVint(ebmlID), name, defaultval) {}

    template<typename T>
    ebmlDataType<T>::ebmlDataType(const char* ebmlID, const std::wstring& name, T&& defaultval)
    : ebmlDataType(unpackVint(ebmlID), name, std::move(defaultval)) {}

    template<typename T>
    ebmlDataType<T>::ebmlDataType(ebmlID_t ebmlID, const std::wstring& name)
    : ebmlTypeCRTP<ebmlDataType<T>, ebmlDataElement<T>>(ebmlID, name) {
        this->defaultval = defaultdefault;
    }

    template<typename T>
    ebmlDataType<T>::ebmlDataType(ebmlID_t ebmlID, const std::wstring& name, const T& defaultval)
    : ebmlTypeCRTP<ebmlDataType<T>, ebmlDataElement<T>>(ebmlID, name), defaultval(defaultval) {}

    template<typename T>
    ebmlDataType<T>::ebmlDataType(ebmlID_t ebmlID, const std::wstring& name, T&& defaultval)
    : ebmlTypeCRTP<ebmlDataType<T>, ebmlDataElement<T>>(ebmlID, name), defaultval(std::move(defaultval)) {}

    template<typename T>
    ebmlDataType<T>::ebmlDataType(const char* ebmlID, const std::wstring& name, const seekHelper_t* seekHelper)
    : ebmlDataType(unpackVint(ebmlID), name, seekHelper) {}

    template<typename T>
    ebmlDataType<T>::ebmlDataType(const char* ebmlID, const std::wstring& name, const T& defaultval, const seekHelper_t* seekHelper)
    : ebmlDataType(unpackVint(ebmlID), name, defaultval, seekHelper) {}

    template<typename T>
    ebmlDataType<T>::ebmlDataType(const char* ebmlID, const std::wstring& name, T&& defaultval, const seekHelper_t* seekHelper)
    : ebmlDataType(unpackVint(ebmlID), name, std::move(defaultval), seekHelper) {}

    template<typename T>
    ebmlDataType<T>::ebmlDataType(ebmlID_t ebmlID, const std::wstring& name, const seekHelper_t* seekHelper)
    : ebmlTypeCRTP<ebmlDataType<T>, ebmlDataElement<T>>(ebmlID, name, seekHelper) {
        this->defaultval = defaultdefault;
    }

    template<typename T>
    ebmlDataType<T>::ebmlDataType(ebmlID_t ebmlID, const std::wstring& name, const T& defaultval, const seekHelper_t* seekHelper)
    : ebmlTypeCRTP<ebmlDataType<T>, ebmlDataElement<T>>(ebmlID, name, seekHelper), defaultval(defaultval) {}

    template<typename T>
    ebmlDataType<T>::ebmlDataType(ebmlID_t ebmlID, const std::wstring& name, T&& defaultval, const seekHelper_t* seekHelper)
    : ebmlTypeCRTP<ebmlDataType<T>, ebmlDataElement<T>>(ebmlID, name, seekHelper), defaultval(std::move(defaultval)) {}

    // template<typename T>
    // ebmlElement* ebmlDataType<T>::_new() const {
    //     return new ebmlDataElement<T>(this, this->defaultval);
    // }

    // template<typename T>
    // ebmlDataElement_sp<T> ebmlDataType<T>::operator()() const {
    //     auto elem = new ebmlDataElement<T>(this, this->defaultval);
    //     return ebmlDataElement_sp<T>(elem);
    // }
    //
    // template<typename T>
    // ebmlDataElement_sp<T> ebmlDataType<T>::operator()(const T& value) const {
    //     auto elem = new ebmlDataElement<T>(this, value);
    //     return ebmlDataElement_sp<T>(elem);
    // }
    //
    // template<typename T>
    // ebmlDataElement_sp<T> ebmlDataType<T>::operator()(T&& value) const {
    //     auto elem = new ebmlDataElement<T>(this, std::move(value));
    //     return ebmlDataElement_sp<T>(elem);
    // }

    // template<typename T>
    // ebmlElement_sp ebmlDataType<T>::_decode(const parseString& parsed) const {
    //     if (parsed.dataSize == 0) {
    //         return ebmlElement_sp(new ebmlDataElement<T>(this, this->defaultval));
    //     } else {
    //         T data = unpack<T>(parsed.data, parsed.dataSize);
    //         return ebmlElement_sp(new ebmlDataElement<T>(this, std::move(data)));
    //     }
    // }
    //
    // template<typename T>
    // ebmlElement_sp ebmlDataType<T>::_decode(const parseFile& parsed) const {
    //     if (parsed.dataSize == 0) {
    //         return ebmlElement_sp(new ebmlDataElement<T>(this, this->defaultval));
    //     } else {
    //         auto buffer_sp = std::make_unique<char[]>(parsed.dataSize);
    //         auto buffer = buffer_sp.get();
    //         parsed.read(buffer, 0, parsed.dataSize);
    //         T data = unpack<T>(buffer, parsed.dataSize);
    //         return ebmlElement_sp(new ebmlDataElement<T>(this, std::move(data)));
    //     }
    // }

    template<typename T>
    ebmlDataElement<T>::ebmlDataElement(const ebmlDataType<T>* cls)
      : ebmlElementCRTP<ebmlDataType<T>, ebmlDataElement<T>>(cls), data(cls->defaultval) {}

    template<typename T>
    ebmlDataElement<T>::ebmlDataElement(const ebmlDataType<T>* __cls, const T& _data)
      : ebmlElementCRTP<ebmlDataType<T>, ebmlDataElement<T>>(__cls), data(_data) {}

    template<typename T>
    ebmlDataElement<T>::ebmlDataElement(const ebmlDataType<T>* __cls, T&& _data) : ebmlElementCRTP<ebmlDataType<T>, ebmlDataElement<T>>(__cls), data(std::move(_data)) {}

    template<typename T>
    ebmlDataElement<T>::ebmlDataElement(const ebmlDataType<T>* __cls, const parseString& parsed)
    : ebmlElementCRTP<ebmlDataType<T>, ebmlDataElement<T>>(__cls),
    data((parsed.dataSize > 0) ? parsed.unpack<T>() : __cls->defaultval) {}

    template<typename T>
    ebmlDataElement<T>::ebmlDataElement(const ebmlDataElement<T>& orig)
    : ebmlElementCRTP<ebmlDataType<T>, ebmlDataElement<T>>(&orig.cls()), data(orig.data) {}

    template<typename T>
    size_t ebmlDataElement<T>::dataSize() const {
        return size(this->data);
    }

    template<typename T>
    size_t ebmlDataElement<T>::_encode(char* dest) const {
        size_t _dataSize = this->dataSize();
        return pack(this->data, _dataSize, dest);
    }

    // template<typename T>
    // void ebmlDataElement<T>::_decode(const parseString& parsed) {
    //     this->data = unpack<T>(parsed.data, parsed.dataSize);
    // }
    //
    // template<typename T>
    // void ebmlDataElement<T>::_decode(const parseFile& parsed) {
    //     auto _data = std::make_unique<char[]>(parsed.dataSize);
    //     parsed.read(_data.get(), 0, parsed.dataSize);
    //     this->data = unpack<T>(_data.get(), parsed.dataSize);
    // }

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
        return elem->as<ebmlDataElement<T>>().data;
    }

    template<typename T>
    const T& data(const c_ebmlElement_sp& elem) {
        return elem->as<const ebmlDataElement<T>>().data;
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


    // template<typename T>
    // ebmlDataType<const T>::ebmlDataType() : ebmlElementClass(), defaultval(defaultdefault) {
    // }

    template<typename T>
    ebmlDataType<const T>::ebmlDataType(const char* ebmlID, const std::wstring& name)
    : ebmlDataType(unpackVint(ebmlID), name, defaultdefault) {}

    template<typename T>
    ebmlDataType<const T>::ebmlDataType(const char* ebmlID, const std::wstring& name, const T& defaultval)
    : ebmlDataType(unpackVint(ebmlID), name, defaultval) {}

    template<typename T>
    ebmlDataType<const T>::ebmlDataType(const char* ebmlID, const std::wstring& name, T&& defaultval)
    : ebmlDataType(unpackVint(ebmlID), name, std::move(defaultval)) {}

    template<typename T>
    ebmlDataType<const T>::ebmlDataType(ebmlID_t ebmlID, const std::wstring& name)
    : ebmlDataType(ebmlID, name, defaultdefault) {}

    template<typename T>
    ebmlDataType<const T>::ebmlDataType(ebmlID_t ebmlID, const std::wstring& name, const T& defaultval)
    : ebmlTypeCRTP<ebmlDataType<const T>, ebmlDataElement<const T>>(ebmlID, name), defaultval(defaultval) {}

    template<typename T>
    ebmlDataType<const T>::ebmlDataType(ebmlID_t ebmlID, const std::wstring& name, T&& defaultval)
    : ebmlTypeCRTP<ebmlDataType<const T>, ebmlDataElement<const T>>(ebmlID, name), defaultval(std::move(defaultval)) {}

    // template<typename T>
    // ebmlElement* ebmlDataType<const T>::_new() const {
    //     return new ebmlDataElement<const T>(this, this->defaultval);
    // }

    // template<typename T>
    // ebmlDataElement_sp<const T> ebmlDataType<const T>::operator()(const T& value) const {
    //     auto elem = new ebmlDataElement<const T>(this, value);
    //     return ebmlDataElement_sp<const T>(elem);
    // }
    //
    // template<typename T>
    // ebmlDataElement_sp<const T> ebmlDataType<const T>::operator()(T&& value) const {
    //     auto elem = new ebmlDataElement<const T>(this, std::move(value));
    //     return ebmlDataElement_sp<const T>(elem);
    // }

    // template<typename T>
    // ebmlElement_sp ebmlDataType<const T>::decode(const parseString& parsed) const {
    //     ebmlElement_sp elem;
    //
    //     if (parsed.ebmlID != this->ebmlID) {
    //         throw ebmlNoMatch("EBML ID does not match.", this, 0, 0, 0);
    //     }
    //
    //     T data = unpack<T>(parsed.data, parsed.dataSize);
    //     elem = (*this)(std::move(data));
    //
    //     // try {
    //     //     elem->_decode(parsed);
    //     // } catch (ebmlDecodeError& e) {
    //     //     if (e.cls == nullptr) {
    //     //         e.cls = this;
    //     //         e.headSize = parsed.ebmlIDWidth + parsed.sizeWidth;
    //     //     }
    //     //     throw;
    //     // }
    //
    //     return elem;
    // }

    // template<typename T>
    // ebmlElement_sp ebmlDataType<const T>::decode(const parseFile& parsed) const {
    //     ebmlElement_sp elem;
    //
    //     if (parsed.ebmlID != this->ebmlID) {
    //         throw ebmlNoMatch("EBML ID does not match.", this, parsed.offset);
    //     }
    //
    //     auto chardata = std::make_unique<char[]>(parsed.dataSize);
    //     auto s_parsed = parseString(parsed, chardata.get());
    //     T data = unpack<T>(s_parsed.data, s_parsed.dataSize);
    //     elem = (*this)(std::move(data));
    //     elem->_offsetInParent = parsed.offset - parsed.parentOffset;
    //     // elem->_decode(parsed);
    //     return elem;
    // }

    // template<typename T>
    // ebmlElement_sp ebmlDataType<const T>::_decode(const parseString& parsed) const {
    //     if (parsed.dataSize == 0) {
    //         return ebmlElement_sp(new ebmlDataElement<const T>(this, this->defaultval));
    //     } else {
    //         T data = unpack<T>(parsed.data, parsed.dataSize);
    //         return ebmlElement_sp(new ebmlDataElement<const T>(this, std::move(data)));
    //     }
    // }
    //
    // template<typename T>
    // ebmlElement_sp ebmlDataType<const T>::_decode(const parseFile& parsed) const {
    //     if (parsed.dataSize == 0) {
    //         return ebmlElement_sp(new ebmlDataElement<const T>(this, this->defaultval));
    //     } else {
    //         auto buffer_sp = std::make_unique<char[]>(parsed.dataSize);
    //         auto buffer = buffer_sp.get();
    //         parsed.read(buffer, 0, parsed.dataSize);
    //         T data = unpack<T>(buffer, parsed.dataSize);
    //         return ebmlElement_sp(new ebmlDataElement<const T>(this, std::move(data)));
    //     }
    // }

    template<typename T>
    ebmlDataElement<const T>::ebmlDataElement(const ebmlDataType<const T>* __cls, const T& _data)
    : ebmlElementCRTP<ebmlDataType<const T>, ebmlDataElement<const T>>(__cls), data(_data) {}

    template<typename T>
    ebmlDataElement<const T>::ebmlDataElement(const ebmlDataType<const T>* __cls, T&& _data)
    : ebmlElementCRTP<ebmlDataType<const T>, ebmlDataElement<const T>>(__cls), data(std::move(_data)) {}

    template<typename T>
    ebmlDataElement<const T>::ebmlDataElement(const ebmlDataType<const T>* __cls, const parseString& parsed)
    : ebmlElementCRTP<ebmlDataType<const T>, ebmlDataElement<const T>>(__cls),
    data((parsed.dataSize > 0) ? parsed.unpack<T>() : __cls->defaultval) {}

    template<typename T>
    ebmlDataElement<const T>::ebmlDataElement(const ebmlDataElement<const T>& orig)
    : ebmlElementCRTP<ebmlDataType<const T>, ebmlDataElement<const T>>(&orig.cls()), data(orig.data) {}

    // template<typename T>
    // const ebmlDataType<const T>* ebmlDataElement<const T>::cls() const {
    //     return dynamic_cast<const ebmlDataType<const T>*>(this->_cls);
    // }

    template<typename T>
    ebmlElement_sp ebmlDataElement<const T>::clone() const {
        // ebmlElement_sp elem = (*this->cls())(this->data);
        auto elem = new ebmlDataElement<const T>(&this->cls(), this->data);
        return ebmlElement_sp(elem);
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

    // template<typename T>
    // void ebmlDataElement<const T>::_decode(const parseString&) {
    // }
    //
    // template<typename T>
    // void ebmlDataElement<const T>::_decode(const parseFile&) {
    // }

    template<typename T>
    void ebmlDataElement<const T>::_clonedata(const ebmlElement*) {
    }

    template <typename T>
    std::wstring ebmlDataElement<const T>::minirepr() const {
        return ebml::repr(this->data);
    }

    // template <typename T>
    // ebmlElement_sp ebmlDataType<const T>::_cdecode(const parseString& parsed) const {
    //     if (parsed.ebmlID != this->ebmlID) {
    //         throw ebmlNoMatch("EBML ID does not match.", this, 0, 0, 0);
    //     }
    //
    //     T data = unpack<T>(parsed.data, parsed.dataSize);
    //     auto elem = (*this)(std::move(data));
    //     return ebmlElement_sp(elem);
    // }
    //
    // template <typename T>
    // ebmlElement_sp ebmlDataType<const T>::_cdecode(const parseFile& parsed) const {
    //     auto chars = std::make_unique<char[]>(parsed.dataSize);
    //     auto s_parsed = parseString(parsed, chars.get());
    //     auto elem = this->_cdecode(s_parsed);
    //     elem->_offsetInParent = parsed.offset - parsed.parentOffset;
    //     return elem;
    // }

}
#endif
