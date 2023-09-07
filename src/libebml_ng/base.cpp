#ifndef EBML_NG_BASE_CPP
#define EBML_NG_BASE_CPP

#include "libebml_ng/base.h"

#include "libebml_ng/struct.cpp"
#include "libebml_ng/parsing/string.cpp"
#include "libebml_ng/parsing/io.cpp"
#include "libebml_ng/exceptions.cpp"
#include "libebml_ng/vint.cpp"
#include <memory>

#include <stdio.h>

namespace ebml {
    ebmlElementClass::ebmlElementClass() {
        this->ebmlID = 0;
        this->name = std::wstring(L"");
    }


    ebmlElement_sp ebmlElementClass::operator()() const {
        return ebmlElement_sp(this->_new());
    }

    ebmlElementClass::ebmlElementClass(ebmlID_t _ebmlID, const std::wstring& _name) {
        this->ebmlID = _ebmlID;
        this->name = _name;
    }

    bool ebmlElement::parent_is_const() const {
        return this->_parent_flags & 0x02;
    }

    ebmlElement_sp ebmlElement::parent() const {
        unsigned char flags = this->_parent_flags & 0x03;

        if (flags & 0x02) {
            throw ebmlException("non-const access to const parent");
        }

        switch (flags & 0x01) {
            case 0x00: {
                return this->_parent;
            }
            case 0x01: {
                return this->_parent_weak.lock();
            }
        }
        return nullptr;
    }

    c_ebmlElement_sp ebmlElement::c_parent() const {
        unsigned char flags = this->_parent_flags & 0x03;

        switch (flags) {
            case 0x00: {
                return this->_parent;
            }
            case 0x01: {
                return this->_parent_weak.lock();
            }
            case 0x02: {
                return this->_const_parent;
            }
            case 0x03: {
                return this->_const_parent_weak.lock();
            }
        }
        return nullptr;
    }

    ebmlElement_sp ebmlElement::root() const {
        ebmlElement_sp current = _parent;
        ebmlElement_sp previous = nullptr;

        while (current) {
            previous = current;
            current = current->parent();
        }

        return previous;
    }

    c_ebmlElement_sp ebmlElement::c_root() const {
        c_ebmlElement_sp current = _parent;
        c_ebmlElement_sp previous = nullptr;

        while (current) {
            previous = current;
            current = current->c_parent();
        }

        return previous;
    }

    ebmlDocument_sp ebmlElement::document() const {
        return root()->_document.lock();
    }

    bool ebmlElement::hasParent() const {
        unsigned char flags = this->_parent_flags & 0x03;

        switch (flags) {
            case 0x00: {
                return this->_parent != nullptr;
            }
            case 0x01: {
                return !this->_parent_weak.expired();
            }
            case 0x02: {
                return this->_const_parent != nullptr;
            }
            case 0x03: {
                return !this->_const_parent_weak.expired();
            }
        }

        return false;
    }

    void ebmlElement::_setParent(const ebmlElement_sp& parent, bool weak) {
        // this->_detach();
        if (this->hasParent()) {
            throw ebmlException("_setParent(): Element already has a parent");
        }

        if (weak) {
            this->_parent_flags = 0x01;
            new (&this->_parent_weak) ebmlElement_wp(parent);
        } else {
            this->_parent_flags = 0x00;
            new (&this->_parent) ebmlElement_sp(parent);
            // this->_parent = parent;
        }
    }

    void ebmlElement::_setParent(ebmlElement_sp&& parent, bool weak) {
        // this->_detach();
        if (this->hasParent()) {
            throw ebmlException("_setParent(): Element already has a parent");
        }

        if (weak) {
            this->_parent_flags = 0x01;
            new (&this->_parent_weak) ebmlElement_wp(parent);
        } else {
            this->_parent_flags = 0x00;
            new (&this->_parent) ebmlElement_sp(std::move(parent));
            // this->_parent = parent;
        }
    }

    void ebmlElement::_setParent(const c_ebmlElement_sp& parent, bool weak) {
        if (parent.get() == this) {
            throw ebmlException("cannot attach an element to itself");
        }

        if (this->hasParent()) {
            throw ebmlException("_setParent(): Element already has a parent");
        }

        if (weak) {
            this->_parent_flags = 0x03;
            new (&this->_const_parent_weak) c_ebmlElement_wp(parent);
            // this->_const_parent_weak = parent;
        } else {
            this->_parent_flags = 0x02;
            new (&this->_const_parent) c_ebmlElement_sp(parent);
        }
    }

    void ebmlElement::_setParent(c_ebmlElement_sp&& parent, bool weak) {
        if (parent.get() == this) {
            throw ebmlException("cannot attach an element to itself");
        }

        if (this->hasParent()) {
            throw ebmlException("_setParent(): Element already has a parent");
        }

        if (weak) {
            this->_parent_flags = 0x03;
            new (&this->_const_parent_weak) c_ebmlElement_wp(parent);
            // this->_const_parent_weak = parent;
        } else {
            this->_parent_flags = 0x02;
            new (&this->_const_parent) c_ebmlElement_sp(std::move(parent));
        }
    }

    void ebmlElement::_attachChild(const ebmlElement_sp& child, bool weak) {
        auto this_sp = this->shared_from_this();
        child->_setParent(std::move(this_sp), weak);
    }

    void ebmlElement::_attachChild(const ebmlElement_sp& child, bool weak) const {
        auto this_sp = this->shared_from_this();
        child->_setParent(std::move(this_sp), weak);
    }

    void ebmlElement::_detachChild(const ebmlElement_sp& child) {
        auto p = child->c_parent();

        if (p.get() != this) {
            throw ebmlException("_detachChild(): Element is not child of this.");
        }

        child->_detach();
    }

    void ebmlElement::_detach() {
        unsigned char flags = this->_parent_flags & 0x03;

        switch (flags) {
            case 0x00: {
                this->_parent.reset();
                return;
            }
            case 0x01: {
                this->_parent_weak.reset();
                return;
            }
            case 0x02: {
                this->_const_parent.reset();
                return;
            }
            case 0x03: {
                this->_const_parent_weak.reset();
                return;
            }
        }

        this->_parent_flags = 0x00;
    }

    ebmlElement_sp ebmlElementClass::decode(const parseString& parsed) const {
        ebmlElement_sp elem;

        if (parsed.ebmlID != this->ebmlID) {
            throw ebmlNoMatch("EBML ID does not match.", this, 0, 0, 0);
        }

        elem = (*this)();

        try {
            elem->_decode(parsed);
        } catch (ebmlDecodeError& e) {
            if (e.cls == nullptr) {
                e.cls = this;
                e.headSize = parsed.ebmlIDWidth + parsed.sizeWidth;
            }
            throw;
        }

        return elem;
    }

    ebmlElement_sp ebmlElementClass::decode(const parseFile& parsed) const {
        ebmlElement_sp elem;

        if (parsed.ebmlID != this->ebmlID) {
            throw ebmlNoMatch("EBML ID does not match.", this, parsed.offset);
        }

        elem = (*this)();
        elem->_decode(parsed);
        return elem;
    }

    ebmlElement_sp ebmlElementClass::decode(const char* data, size_t size) const {
        auto parsed = parseString(data, size);

        if (size > parsed.outerSize()) {
            throw ebmlDataContinues("Data continues past expected end.", this, 0, parsed.ebmlIDWidth + parsed.sizeWidth, parsed.dataSize);
        }

        return this->decode(parsed);
    }

    ebmlElement_sp ebmlElementClass::decode(const std::string& data) const {
        return this->decode(data.data(), data.size());
    }

    ebmlElement_sp ebmlElementClass::decode(ioBase_sp& file) const {
        return this->decode(file.get());
    }

    ebmlElement_sp ebmlElementClass::decode(ioBase* file) const {
        auto parsed = parseFile(file);
        return this->decode(parsed);
    }

    ebmlElement_sp ebmlElementClass::_cdecode(const parseString& parsed) const {
        if (parsed.ebmlID != this->ebmlID) {
            throw ebmlNoMatch("EBML ID does not match.", this, 0, 0, 0);
        }

        auto elem = ebmlElement_sp(this->_new());

        try {
            elem->_cdecode(parsed);
        } catch (ebmlDecodeError& e) {
            if (e.cls == nullptr) {
                e.cls = this;
                e.headSize = parsed.ebmlIDWidth + parsed.sizeWidth;
            }
            throw;
        }

        return elem;
    }

    c_ebmlElement_sp ebmlElementClass::cdecode(const parseString& parsed) const {
        return std::const_pointer_cast<const ebmlElement>(this->_cdecode(parsed));
    }

    ebmlElement_sp ebmlElementClass::_cdecode(const parseFile& parsed) const {
        if (parsed.ebmlID != this->ebmlID) {
            throw ebmlNoMatch("EBML ID does not match.", this, parsed.offset);
        }

        auto elem = ebmlElement_sp(this->_new());
        elem->_cdecode(parsed);
        return elem;
    }

    c_ebmlElement_sp ebmlElementClass::cdecode(const parseFile& parsed) const {
        return std::const_pointer_cast<const ebmlElement>(this->_cdecode(parsed));
    }

    c_ebmlElement_sp ebmlElementClass::cdecode(const char* data, size_t size) const {
        auto parsed = parseString(data, size);

        if (size > parsed.outerSize()) {
            throw ebmlDataContinues("Data continues past expected end.", this, 0, parsed.ebmlIDWidth + parsed.sizeWidth, parsed.dataSize);
        }

        return this->cdecode(parsed);
    }

    c_ebmlElement_sp ebmlElementClass::cdecode(const std::string& data) const {
        return this->cdecode(data.data(), data.size());
    }

    c_ebmlElement_sp ebmlElementClass::cdecode(ioBase_sp& file) const {
        return this->cdecode(file.get());
    }

    c_ebmlElement_sp ebmlElementClass::cdecode(ioBase* file) const {
        auto parsed = parseFile(file);
        return this->cdecode(parsed);
    }

    ebmlElement::ebmlElement(const ebmlElementClass* cls) { // : _parent(nullptr) {
        this->_cls = cls;
        this->_parent_flags = 0x00;
        new (&this->_parent) ebmlElement_sp(nullptr);
    }

    void ebmlElement::_clonedata(const ebmlElement*) {
        throw ebmlException("void _clonedata(const ebmlElement_sp&) not implemented.");
    }

    void ebmlElement::_decode(const parseFile& parsed) {
        std::unique_ptr<char[]> data(new char[parsed.dataSize]);
        auto s_parsed = parseString(parsed, data.get());
        this->_decode(s_parsed);
    }

    void ebmlElement::_cdecode(const parseString& parsed) {
        this->_decode(parsed);
    }

    void ebmlElement::_cdecode(const parseFile& parsed) {
        std::unique_ptr<char[]> data(new char[parsed.dataSize]);
        auto s_parsed = parseString(parsed, data.get());
        this->_cdecode(s_parsed);
    }

    size_t ebmlElement::encode(char* dest) const {
        unsigned long long dataSize = this->dataSize();
        return ebmlElement::encode(dest, dataSize);
    }

    size_t ebmlElement::encode(ioBase* dest) const {
        size_t dataSize = this->dataSize();
        auto buffer = std::make_unique<char[]>(dataSize + 16);
        auto outerSize = this->encode(buffer.get(), dataSize);
        return dest->write(buffer.get(), outerSize);
    }

    size_t ebmlElement::_encode_head(char* dest, size_t dataSize) const {
        unsigned char ebmlIDWidth;
        unsigned char sizeWidth;

        if (this->_cls == NULL) {
            throw ebmlEncodeError("Class not defined.", this->shared_from_this());
        };

        ebmlIDWidth = widthAsVint(this->_cls->ebmlID);
        sizeWidth = widthAsVint(dataSize);

        if (this->sizeWidth > 0) {
            if (sizeWidth > this->sizeWidth) {
                throw ebmlEncodeError("Data size incompatible with specified size width.", this->shared_from_this());
            };
            sizeWidth = this->sizeWidth;
        }

        packVint(this->_cls->ebmlID, ebmlIDWidth, dest);
        packVint(dataSize, sizeWidth, dest + ebmlIDWidth);

        return ebmlIDWidth + sizeWidth;
    }

    size_t ebmlElement::encode(char* dest, size_t dataSize) const {
        size_t headWidth = this->_encode_head(dest, dataSize);

        if (this->_encode(dest + headWidth) != dataSize) {
            throw ebmlEncodeError("Encoded size does not match expected size.", this->shared_from_this());
        };

        return headWidth + dataSize;
    }

    size_t ebmlElement::dataSize() const {
        throw ebmlNotImplementedError("size_t dataSize() not implemented.");
    }

    unsigned char ebmlElement::headSize(size_t dataSize) const {
        unsigned char _sizeWidth = widthAsVint(dataSize);

        if (this->sizeWidth > 0 and this->sizeWidth < _sizeWidth) {
            throw ebmlEncodeError("Data size incompatible with specified size width.", this->shared_from_this());
        }

        return widthAsVint(this->_cls->ebmlID) + widthAsVint(dataSize);
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

    const ebmlElementClass* ebmlElement::cls() const {
        return this->_cls;
    }

    unsigned long long ebmlElement::ebmlID() const {
        return this->_cls->ebmlID;
    }

    ebmlElement_sp ebmlElement::clone() const {
        ebmlElement_sp elem = (*this->_cls)();
        elem->_clonedata(this);
        return elem;
    }

    ebmlElement::~ebmlElement() {
        this->_detach();
    }

    std::wstring ebmlElement::repr() const {
        auto mrepr = this->minirepr();

        std::wstringstream ss;

        char ebmlID_chars[8];
        unsigned char ebmlIDWidth;

        ebmlIDWidth = packVint(this->ebmlID(), ebmlID_chars);

        ss << L"<";
        ss << this->cls()->name;

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

    std::wstring ebmlElementClass::repr() const {
        std::wstringstream ss;

        char ebmlID_chars[8];
        unsigned char ebmlIDWidth;

        ebmlIDWidth = packVint(this->ebmlID, ebmlID_chars);

        ss << L"<EBML Element type: ";
        ss << this->name;

        ss << L", ebmlID=0x";

        for (unsigned char k = 0; k < ebmlIDWidth; k++) {
            ss << std::hex << std::setw(2) << std::setfill(L'0') << static_cast<unsigned char>(ebmlID_chars[k]);
        }

        ss << L">";
        return ss.str();
    }

    std::wstring ebmlElementClass::minirepr() const {
        return std::wstring();
    }

    ebmlVoidClass::ebmlVoidClass() {
        this->ebmlID = 108;
    }

    ebmlElement* ebmlVoidClass::_new() const {
        return new ebmlVoid(this);
    }

    ebmlElement_sp ebmlVoidClass::operator()(size_t size) const {
        auto elem = new ebmlVoid(this, size);
        return std::shared_ptr<ebmlVoid>(elem);
    }

    ebmlVoid::ebmlVoid(const ebmlVoidClass* cls) : ebmlElement(cls), voidsize(0) {}
    ebmlVoid::ebmlVoid(const ebmlVoidClass* cls, size_t size) : ebmlElement(cls), voidsize(size) {}

    size_t ebmlVoid::dataSize() const {
        return this->voidsize;
    }

    size_t ebmlVoid::_encode(char* dest) const {
        memset(dest, 0, this->voidsize);
        return this->voidsize;
    }

    size_t ebmlVoid::encode(ioBase* file) const {
        char buffer[16];
        auto headSize = this->_encode_head(buffer, this->voidsize);

        file->write(buffer, headSize);

        if (file->seekable()) {
            file->seek(this->voidsize, SEEK_CUR);
        } else {
            char zeros[4096];
            memset(zeros, 0, 4096);

            for (size_t k = 0; k < this->voidsize; k += 4096) {
                file->write(zeros, std::min(static_cast<size_t>(4096), static_cast<size_t>(this->voidsize - k)));
            }
        }

        return this->voidsize;
    }

    void ebmlVoid::_decode(const parseString& parsed) {
        this->voidsize = parsed.dataSize;
    }
    void ebmlVoid::_decode(const parseFile& parsed) {
        this->voidsize = parsed.dataSize;
        parsed.seek(parsed.dataSize);
    }

    void ebmlVoid::_clonedata(const ebmlElement* orig) {
        const ebmlVoid* orig_typed = dynamic_cast<const ebmlVoid*>(orig);
        if (orig_typed) {
            this->voidsize = orig_typed->voidsize;
        }
    }

    std::wstring ebmlVoid::minirepr() const {
        return std::to_wstring(this->voidsize);
    }
    std::wstring ebmlVoid::repr() const {
        auto mrepr = this->minirepr();

        std::wstringstream ss;

        char ebmlID_chars[8];
        unsigned char ebmlIDWidth;

        ebmlIDWidth = packVint(this->ebmlID(), ebmlID_chars);

        ss << L"<";
        ss << this->cls()->name;

        ss << L" object: ebmlID=0x";
        // ret += ebml::repr(this->ebmlID());

        for (unsigned char k = 0; k < ebmlIDWidth; k++) {
            ss << std::hex << std::setw(2) << std::setfill(L'0') << static_cast<unsigned char>(ebmlID_chars[k]);
        }

        if (mrepr.size() > 0) {
            ss << L", voidsize=";
            ss << mrepr;
        }

        ss << L">";
        return ss.str();
    }
}

#endif
