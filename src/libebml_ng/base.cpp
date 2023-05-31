#ifndef EBML_NG_BASE_CPP
#define EBML_NG_BASE_CPP

#include "libebml_ng/base.h"

#include "libebml_ng/struct.cpp"
#include "libebml_ng/parsing/string.cpp"
#include "libebml_ng/parsing/io.cpp"
#include "libebml_ng/exceptions.cpp"
#include "libebml_ng/vint.cpp"

#include <stdio.h>

namespace ebml {
    ebmlElementClassBase::ebmlElementClassBase(ebmlID_t _ebmlID, const std::u32string& _name) {
        this->ebmlID = _ebmlID;
        this->name = _name;
    }

//     ebmlElement_sp ebmlElementClassBase::operator()() const {
//         throw ebmlException("void _clonedata(const ebmlElement_sp&) not implemented.", __LINE__, __FILE__);
//         auto elem = new ebmlElementBase(this);
//         return ebmlElement_sp(elem);
//     }

    ebmlElement_sp ebmlElementBase::parent() const {
        if (this->_weakparent) {
            if (!this->_parent_weak.expired()) {
                return this->_parent_weak.lock();
            }
            return nullptr;
        }
        return nullptr;
    }

    ebmlElement_sp ebmlElementBase::root() const {
        ebmlElement_sp current = _parent;
        ebmlElement_sp previous = nullptr;

        while (current) {
            previous = current;
            current = current->parent();
        }

        return previous;
    }

    ebmlDocument_sp ebmlElementBase::document() const {
        return root()->_document.lock();
    }

    bool ebmlElementBase::_hasParent() const {
        if (this->_weakparent) {
            return !this->_parent_weak.expired();
        }
        return this->_parent != nullptr;
    }

    void ebmlElementBase::_setParent(ebmlElement_sp __parent, bool weak) {
        this->_detach();
        this->_weakparent = weak;

        if (weak) {
            this->_parent_weak = __parent;
        } else {
            this->_parent = __parent;
        }
    }

    void ebmlElementBase::_detach() {
        if (this->_weakparent) {
            this->_parent_weak.reset();
            this->_weakparent = false;
        }
        this->_parent = nullptr;
    }

    ebmlElement_sp ebmlElementClassBase::decode(const char* data, size_t size) const {
        auto parsed = parseString(data, size);

        if (size > parsed.outerSize()) {
            throw ebmlException("Data continues past expected end.", __LINE__, __FILE__);
        }

        return this->decode(parsed);
    }

    ebmlElement_sp ebmlElementClassBase::decode(ioBase* file) const {
        auto parsed = parseFile_sp(new parseFile(file));
        return this->decode(parsed);
    }

    ebmlElement_sp ebmlElementClassBase::decode(const parseString& parsed) const {
        ebmlElement_sp elem;

        if (parsed.ebmlID != this->ebmlID) {
            throw ebmlException("EBML ID does not match.", __LINE__, __FILE__);
        }

        elem = (*this)();
        elem->_decode(parsed);

        return elem;
    }

    ebmlElement_sp ebmlElementClassBase::decode(const parseFile_sp& parsed) const {
        ebmlElement_sp elem;

        if (parsed->ebmlID != this->ebmlID) {
            throw ebmlException("EBML ID does not match.", __LINE__, __FILE__);
        }

        elem = (*this)();
        elem->_decode(parsed);
        return elem;
    }

    ebmlElementBase::ebmlElementBase(const ebmlElementClassBase* cls) {
        this->_cls = cls;
    }

    void ebmlElementBase::_clonedata(const ebmlElementBase*) {
        throw ebmlException("void _clonedata(const ebmlElement_sp&) not implemented.", __LINE__, __FILE__);
    }

//     void ebmlElementBase::_decode(const parseString& parsed) {
//         throw ebmlException("void _decode(const parseString&) not implemented.", __LINE__, __FILE__);
//     }
//

    void ebmlElementBase::_decode(const parseFile_sp& parsed) {
        std::unique_ptr<char[]> data(new char[parsed->dataSize]);
        auto s_parsed = parseString(parsed, data.get());
        this->_decode(s_parsed);
    }

//     size_t ebmlElementBase::_encode(char* dest, size_t dataSize) const {
//         throw ebmlException("unsigned long long _encode(char*, unsigned long long) not implemented.", __LINE__, __FILE__);
//     }

    // unsigned long long ebmlElementBase::_size() const {
    //     throw ebmlException("unsigned long long _size() not implemented.", __LINE__, __FILE__);
    // };

    size_t ebmlElementBase::encode(char* dest) const {
        unsigned long long dataSize = this->dataSize();
        return ebmlElementBase::encode(dest, dataSize);
    }

    size_t ebmlElementBase::encode(char* dest, size_t dataSize) const {
        unsigned char ebmlIDWidth;
        unsigned char sizeWidth;

        if (this->_cls == NULL) {
            throw ebmlClassNotDefined("Class not defined.", __LINE__, __FILE__);
        };

        ebmlIDWidth = widthAsVint(this->_cls->ebmlID);
        sizeWidth = widthAsVint(dataSize);

        if (this->sizeWidth > 0) {
            if (sizeWidth > this->sizeWidth) {
                throw ebmlException("Data size incompatible with specified size width.", __LINE__, __FILE__);
            };
            sizeWidth = this->sizeWidth;
        }

        packVint(this->_cls->ebmlID, ebmlIDWidth, dest);
        packVint(dataSize, sizeWidth, dest + ebmlIDWidth);

        if (this->_encode(dest + ebmlIDWidth + sizeWidth) != dataSize) {
            throw ebmlException("Encoded size does not match expected size.", __LINE__, __FILE__);
        };
        return ebmlIDWidth + sizeWidth + dataSize;
    }

    size_t ebmlElementBase::dataSize() const {
        throw ebmlException("size_t dataSize() not implemented.", __LINE__, __FILE__);
    }

    unsigned char ebmlElementBase::headSize(size_t dataSize) const {
        unsigned char _sizeWidth = widthAsVint(dataSize);

        if (this->sizeWidth > 0 and this->sizeWidth < _sizeWidth) {
            throw ebmlException("Data size incompatible with specified size width.", __LINE__, __FILE__);
        }
        return widthAsVint(this->_cls->ebmlID) + widthAsVint(dataSize);
    }

    unsigned char ebmlElementBase::headSize() const {
        return headSize(this->dataSize());
    }

    size_t  ebmlElementBase::outerSize() const {
        return this->outerSize(this->dataSize());
    }

    size_t ebmlElementBase::outerSize(size_t dataSize) const {
        unsigned char headSize = this->headSize(dataSize);
        return headSize + dataSize;
    }

    // void ebmlElement::init(const void* initdata) {
    //     this->_cls->_init(this, initdata);
    // }

    const ebmlElementClassBase* ebmlElementBase::cls() const {
        return this->_cls;
    }

    unsigned long long ebmlElementBase::ebmlID() const {
        auto cls = this->_cls;
        printf("%llu %llu %llu\n", (unsigned long long)this, this->_cls->ebmlID, cls->ebmlID);
        return this->_cls->ebmlID;
    }

    ebmlElement_sp ebmlElementBase::clone() const {
        ebmlElement_sp elem = (*this->_cls)();
        elem->_clonedata(this);
        return elem;
    }

    ebmlElementBase::~ebmlElementBase() {}

    std::u32string ebmlElementBase::repr() const {
        return std::u32string();
    }

    std::u32string ebmlElementClassBase::repr() const {
        return std::u32string();
    }
}

#endif
