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
    bool ebmlElement::parent_is_const() const {
        return this->_parent.is_const();
    }

    ebml::ptr<ebmlMasterElement> ebmlElement::parent() const {
        return this->_parent.get();
    }

    ebml::ptr<const ebmlMasterElement> ebmlElement::c_parent() const {
        return this->_parent.cget();
    }

    ebmlElement_sp ebmlElement::root() const {
        ebmlElement_sp current = this->parent();
        ebmlElement_sp previous = nullptr;

        while (current) {
            previous = current;
            current = current->parent();
        }

        return previous;
    }

    c_ebmlElement_sp ebmlElement::c_root() const {
        c_ebmlElement_sp current = this->c_parent();
        c_ebmlElement_sp previous = sp();

        while (current) {
            previous = current;
            current = current->c_parent();
        }

        return previous;
    }

    ebmlDocument_sp ebmlElement::document() const {
        auto r = this->c_root();

        if (r != nullptr) {
            return r->_document.lock();
        }

        return nullptr;
    }

    bool ebmlElement::hasParent() const {
        return this->_parent.notnull();
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

    // void ebmlElement::_attachChild(const ebmlElement_sp& child, bool weak) {
    //     // auto this_sp = this->shared_from_this();
    //     auto this_sp = sp();
    //     child->_setParent(std::move(this_sp), weak);
    // }
    //
    // void ebmlElement::_attachChild(const ebmlElement_sp& child, bool weak) const {
    //     // auto this_sp = this->shared_from_this();
    //     auto this_sp = sp();
    //     child->_setParent(std::move(this_sp), weak);
    // }

    // void ebmlElement::_detachChild(const ebmlElement_sp& child) const {
    //     auto p = child->c_parent();
    //
    //     if (p.get() != this) {
    //         throw ebmlException("_detachChild(): Element is not child of this.");
    //     }
    //
    //     child->_detach();
    // }

    void ebmlElement::_detach() {
        this->_parent = nullptr;
        // unsigned char flags = this->_parent_flags & 0x03;
        //
        // switch (flags) {
        //     case 0x00: {
        //         this->_parent.reset();
        //         return;
        //     }
        //     case 0x01: {
        //         this->_parent_weak.reset();
        //         return;
        //     }
        //     case 0x02: {
        //         this->_const_parent.reset();
        //         return;
        //     }
        //     case 0x03: {
        //         this->_const_parent_weak.reset();
        //         return;
        //     }
        // }
        //
        // this->_parent_flags = 0x00;
    }

    off_t ebmlElement::offsetInParent() const {
        return this->_offsetInParent;
    }

    off_t ebmlElement::offsetInFile() const {
        // std::cout << "ebmlElement::offsetInFile: " << std::endl;
        if (this->_offsetInParent < 0) {
            return -1;
        }

        auto parent = this->c_parent();

        if (parent == nullptr) {
            return this->_offsetInParent;
        }

        // auto x = parent->dataOffsetInFile();
        // std::cout << "parent->dataOffsetInFile(): " << x << std::endl;
        // std::cout << "this->_offsetInParent: " << this->_offsetInParent << std::endl;
        return parent->dataOffsetInFile() + this->_offsetInParent;
    }

    off_t ebmlElement::dataOffsetInParent() const {
        if (this->_offsetInParent >= 0) {
            return this->_offsetInParent + this->headSize();
        }

        return -1;
    }

    off_t ebmlElement::dataOffsetInFile() const {
        off_t offset = this->offsetInFile();

        if (offset >= 0) {
            return offset + this->headSize();
        }

        return -1;
    }

    // ebmlElement::ebmlElement(const ebmlElementClass* cls) : _cls(cls) {}
    ebmlElement::ebmlElement(const ebmlElementClass* cls) : _cls(cls), _self(new control_block(), this) {}

    void ebmlElement::_validate() {
        auto lock = std::unique_lock(_self.mutex);

        if (_self.ctl == nullptr) {
            _self.ctl = new control_block;
            _self.ctl->weakcount = 1;
            _self.ptr = this;
        }

        _self.ctl->valid = true;
    }

    // void ebmlElement::_clonedata(const ebmlElement*) {
    //     throw ebmlException("void _clonedata(const ebmlElement_sp&) not implemented.");
    // }

    // void ebmlElement::_decode(const parseFile& parsed) {
    //     auto data = std::make_unique<char[]>(parsed.dataSize); // std::unique_ptr<char[]> data(new char[parsed.dataSize]);
    //     auto s_parsed = parseString(parsed, data.get());
    //     this->_decode(s_parsed);
    // }

    // void ebmlElement::_cdecode(const parseString& parsed) {
    //     this->_decode(parsed);
    // }

    // void ebmlElement::_cdecode(const parseFile& parsed) {
    //     auto data = std::make_unique<char[]>(parsed.dataSize);
    //     auto s_parsed = parseString(parsed, data.get());
    //     this->_cdecode(s_parsed);
    // }

    size_t ebmlElement::encode(char* dest) const {
        size_t dataSize = this->dataSize();
        return ebmlElement::encode(dest, dataSize);
    }

    std::string ebmlElement::encode() const {
        std::string result;
        size_t dataSize = this->dataSize();
        result.resize(dataSize+16);
        size_t outerSize = this->encode(&result[0], dataSize);
        result.resize(outerSize);
        return result;
    }

    // size_t ebmlElement::encode(const ioBase_sp& dest) const {
    //     return encode(*dest);
    // }

    // size_t ebmlElement::encode(const ioBase_sp& dest, off_t offset) const {
    //     return encode(*dest, offset);
    // }

    size_t ebmlElement::encode(ioBase& dest) const {
        size_t _dataSize = dataSize();
        auto buffer = std::make_unique<char[]>(_dataSize + 16);
        auto outerSize = encode(buffer.get(), _dataSize);
        return dest.write(buffer.get(), outerSize);
    }

    size_t ebmlElement::encode(ioBase& dest, off_t offset) const {
        size_t _dataSize = dataSize();
        auto buffer = std::make_unique<char[]>(_dataSize + 16);
        auto outerSize = encode(buffer.get(), _dataSize);
        return dest.write(buffer.get(), offset, outerSize);
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
