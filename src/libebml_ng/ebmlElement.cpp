#ifndef EBML_NG_ELEMENT_CPP
#define EBML_NG_ELEMENT_CPP

// #include <memory>
// #include <iostream>
// #include <string>
#include <sstream>
#include <iomanip>

#include "libebml_ng/ebmlElementClass.h"
#include "libebml_ng/ebmlElement.h"
#include "libebml_ng/ebmlDocument.h"
#include "libebml_ng/exceptions.h"
#include "libebml_ng/struct.h"
#include "libebml_ng/vint.h"
#include "libebml_ng/seekdata.h"
#include "libebml_ng/masterelement/base/ebmlMasterElement.h"
// #include "libebml_ng/masterelement/lazyload.h"
// #include "libebml_ng/seekdata.h"

namespace ebml {
    ebmlElement::ebmlElement(const ebmlElementClass* cls) : _cls(cls), _self(new control_block(), this) {}

    bool ebmlElement::parent_is_const() const {
        return _parent.is_const();
    }

    ebml::ptr<ebmlMasterElement> ebmlElement::parent() const {
        return _parent.get();
    }

    ebml::ptr<const ebmlMasterElement> ebmlElement::c_parent() const {
        return _parent.cget();
    }

    bool ebmlElement::hasParent() const {
        return _parent.notnull();
    }

    void ebmlElement::_detach() {
        _parent = nullptr;
    }

    ebmlElement_sp ebmlElement::root() const {
        ebmlElement_sp current = parent();
        ebmlElement_sp previous = nullptr;

        while (current != nullptr) {
            previous = std::move(current);
            current = previous->parent();
        }

        return previous;
    }

    c_ebmlElement_sp ebmlElement::c_root() const {
        c_ebmlElement_sp current = c_parent();
        c_ebmlElement_sp previous = sp();

        while (current != nullptr) {
            previous = std::move(current);
            current = previous->parent();
        }

        return previous;
    }

    void ebmlElement::_setParent(const ebml::ptr<ebmlMasterElement>& parent, bool weak) {
        if (this->hasParent()) {
            throw ebmlException("_setParent(): Element already has a parent");
        }

        if (weak) {
            this->_parent = wptr(parent);
        } else {
            this->_parent = parent;
        }
    }

    void ebmlElement::_setParent(ebml::ptr<ebmlMasterElement>&& parent, bool weak) {
        if (this->hasParent()) {
            throw ebmlException("_setParent(): Element already has a parent");
        }

        if (weak) {
            this->_parent = wptr(parent);
            parent = nullptr;
        } else {
            this->_parent = std::move(parent);
        }
    }

    void ebmlElement::_setParent(const ebml::ptr<const ebmlMasterElement>& parent, bool weak) {
        if (this->hasParent()) {
            throw ebmlException("_setParent(): Element already has a parent");
        }

        if (parent.get() == this) {
            throw ebmlException("cannot attach an element to itself");
        }

        if (weak) {
            this->_parent = wptr(parent);
        } else {
            this->_parent = parent;
        }
    }

    void ebmlElement::_setParent(ebml::ptr<const ebmlMasterElement>&& parent, bool weak) {
        if (parent.get() == this) {
            throw ebmlException("cannot attach an element to itself");
        }

        if (this->hasParent()) {
            throw ebmlException("_setParent(): Element already has a parent");
        }

        if (weak) {
            this->_parent = wptr(parent);
            parent = nullptr;
        } else {
            this->_parent = std::move(parent);
        }
    }

    void ebmlElement::_setParent(ebmlMasterElement& parent, bool weak) {
        if (this->hasParent()) {
            throw ebmlException("_setParent(): Element already has a parent");
        }

        if (weak) {
            this->_parent = parent.wp();
        } else {
            this->_parent = parent.sp();
        }
    }

    void ebmlElement::_setParent(const ebmlMasterElement& parent, bool weak) {
        if (this->hasParent()) {
            throw ebmlException("_setParent(): Element already has a parent");
        }

        if (weak) {
            this->_parent = parent.wp();
        } else {
            this->_parent = parent.sp();
        }
    }

    off_t ebmlElement::offsetInFile() const {
        if (_offsetInParent < 0) {
            return -1;
        }

        auto parent = c_parent();

        if (parent == nullptr) {
            return _offsetInParent;
        }

        return parent->dataOffsetInFile() + _offsetInParent;
    }

    ebmlDocument_sp ebmlElement::document() const {
        auto r = c_root();

        if (r != nullptr) {
            return r->_document.lock();
        }

        return nullptr;
    }

    off_t ebmlElement::dataOffsetInParent() const {
        if (_offsetInParent >= 0) {
            return _offsetInParent + headSize();
        }

        return -1;
    }

    off_t ebmlElement::dataOffsetInFile() const {
        off_t offset = offsetInFile();

        if (offset >= 0) {
            return offset + headSize();
        }

        return -1;
    }

    size_t ebmlElement::_encode_head(char* dest, size_t dataSize) const {
        unsigned char ebmlIDWidth;
        unsigned char sizeWidth;

        if (_cls == nullptr) {
            throw ebmlEncodeError("Class not defined.", sp());
        };

        ebmlIDWidth = widthAsVint(this->_cls->ebmlID);
        sizeWidth = widthAsVint(dataSize);

        if (this->_sizeWidth > 0) {
            if (sizeWidth > this->_sizeWidth) {
                throw ebmlEncodeError("Data size incompatible with specified size width.", sp());
            };
            sizeWidth = this->_sizeWidth;
        }

        packVint(_cls->ebmlID, ebmlIDWidth, dest);
        packVint(dataSize, sizeWidth, dest + ebmlIDWidth);

        return ebmlIDWidth + sizeWidth;
    }

    size_t ebmlElement::encode(char* dest, size_t dataSize) const {
        size_t headWidth = this->_encode_head(dest, dataSize);

        if (_encode(dest + headWidth) != dataSize) {
            throw ebmlEncodeError("Encoded size does not match expected size.", sp());
        };

        return headWidth + dataSize;
    }

    size_t ebmlElement::dataSize() const {
        throw ebmlNotImplementedError("size_t dataSize() not implemented.");
    }

    unsigned char ebmlElement::sizeWidth() const {
        return this->sizeWidth(this->dataSize());
    }

    unsigned char ebmlElement::sizeWidth(size_t dataSize) const {
        unsigned char sizeWidth = widthAsVint(dataSize);

        if (this->_sizeWidth > 0) {
            if ((dataSize < UNKNOWN) and (this->_sizeWidth < sizeWidth)) {
                throw ebmlEncodeError("Data size incompatible with specified size width.", sp());
            }
            sizeWidth = this->_sizeWidth;
        }

        return sizeWidth;
    }

    unsigned char ebmlElement::headSize(size_t dataSize) const {
        return widthAsVint(this->_cls->ebmlID) + this->sizeWidth(dataSize);
    }

    unsigned char ebmlElement::headSize() const {
        return headSize(this->dataSize());
    }

    size_t  ebmlElement::outerSize() const {
        return this->outerSize(this->dataSize());
    }

    size_t ebmlElement::outerSize(size_t dataSize) const {
        unsigned char headSize = this->headSize(dataSize);
        return headSize + dataSize;
    }

    const ebmlElementClass& ebmlElement::cls() const {
        return *_cls;
    }

    ebmlID_t ebmlElement::ebmlID() const {
        return this->_cls->ebmlID;
    }

    // ebmlElement_sp ebmlElement::clone() const {
    //     ebmlElement_sp elem = (*this->_cls)();
    //     elem->_clonedata(this);
    //     return elem;
    // }

    ebml::ptr<ebmlElement> ebmlElement::clone() const {
        ebmlElement* elem = _clone();
        return elem->sp();
    }

    seekData_t* ebmlElement::makeSeekData() {
        return this->_cls->_seekHelper->makeSeekData(*this);
    }

    seekData_t* ebmlElement::makeSeekData() const {
        return this->_cls->_seekHelper->makeSeekData(*this);
    }

    ebmlElement::~ebmlElement() {}

    std::wstring ebmlElement::repr() const {
        auto mrepr = this->minirepr();

        std::wstringstream ss;

        char ebmlID_chars[8];
        unsigned char ebmlIDWidth;

        ebmlIDWidth = packVint(this->ebmlID(), ebmlID_chars);

        ss << L"<";
        ss << this->cls().name;

        ss << L" object: ebmlID=0x";
        // ret += ebml::repr(this->ebmlID());

        for (unsigned char k = 0; k < ebmlIDWidth; k++) {
            ss << std::hex << std::setw(2) << std::setfill(L'0') << static_cast<unsigned char>(ebmlID_chars[k]);
        }

        if (mrepr.size() > 0) {
            ss << L", data=";
            ss << mrepr;
        }

        ss << L">";
        return ss.str();
    }

    template class flexible_ptr<ebmlElement>;
    template class flexible_ptr<ebmlMasterElement>;

    // TODO:

    // ebmlElementBase::ebmlElementBase(const ebmlElementTypeBase* cls) : _cls(cls) {}
    //
    // const ebmlElementTypeBase* ebmlElementBase::cls() const {
    //     return this->_cls;
    // }
    //
    // bool ebmlElementBase::hasParent() const {
    //     return this->_parent.notnull();
    // }
    //
    // bool ebmlElementBase::constParent() const {
    //     return this->_parent.is_const();
    // }
    //
    // ptr<ebmlMasterElement> ebmlElementBase::parent() const {
    //     return this->_parent.get();
    // }
    //
    // ptr<const ebmlMasterElement> ebmlElementBase::cparent() const {
    //     return this->_parent.cget();
    // }
    ebml::ptr<ebmlElement> ebmlElement::sp() {
        return _self.lock();
    }

    ebml::ptr<const ebmlElement> ebmlElement::sp() const {
        return _self.c_lock();
    }

    ebml::wptr<ebmlElement> ebmlElement::wp() {
        return _self;
    }

    ebml::wptr<const ebmlElement> ebmlElement::wp() const {
        return ebml_static_pointer_cast<const ebmlElement>(_self);
    }
}

#endif
