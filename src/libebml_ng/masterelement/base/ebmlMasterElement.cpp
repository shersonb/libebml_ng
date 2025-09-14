#ifndef EBML_NG_MASTERELEMENT_CHILDCLASSSPEC_CPP
#define EBML_NG_MASTERELEMENT_CHILDCLASSSPEC_CPP

#include "ebmlMasterElement.h"
#include "../../exceptions.h"
#include "../../ebmlElementType.h"
#include "../../ebmlElement.tpp"
#include "../../ptrs.h"
#include "../../ebmlVoid.h"
#include "../../typeof.h"
#include "../../struct/unicode.h"
#include "../../struct.tpp"

// #include "base/childmgmt.cpp"
// #include "base/decoding.cpp"
// #include "base/encoding.cpp"
// #include "base/initialization.cpp"
// #include "base/iteration.cpp"
// #include "base/size.cpp"

namespace ebml {
    template class ebmlElementCRTP<ebmlMasterElementType, ebmlMasterElement, ebmlElement>;
    template class ebml_shared_ptr<ebmlMasterElement>;
    template class ebml_weak_ptr<ebmlMasterElement>;

    ebmlMasterElement::ebmlMasterElement(const ebmlMasterElementType * cls)
      : ebmlElementCRTP<ebmlMasterElementType, ebmlMasterElement>(cls) {}

    void ebmlMasterElement::_init(const parseString& parsed) {
        try {
            if (parsed.dataSize > 0) {
                parseString::iterator iter = parsed.begin();
                _decodeChildren(iter);
            }
        } catch (ebmlDecodeError& e) {
            if (e.cls != nullptr) {
                e.offset += parsed.ebmlIDWidth + parsed.sizeWidth;
            }
            throw;
        }
    }

    void ebmlMasterElement::_init(const parseFile& parsed) {
        std::cout << typeof(this) << "::_init(const parseFile&)" << std::endl;
        try {
            if (parsed.dataSize > 0) {
                parseFile::iterator iter = parsed.begin();
                _scanChildren(iter);
            }
        } catch (ebmlDecodeError& e) {
            if (e.cls != nullptr) {
                e.offset += parsed.ebmlIDWidth + parsed.sizeWidth;
            }
            throw;
        }
    }

    void ebmlMasterElement::_cinit(const parseString& parsed) {
        try {
            if (parsed.dataSize > 0) {
                parseString::iterator iter = parsed.begin();
                _cdecodeChildren(iter);
            }
        } catch (ebmlDecodeError& e) {
            if (e.cls != nullptr) {
                e.offset += parsed.ebmlIDWidth + parsed.sizeWidth;
            }
            throw;
        }
    }

    void ebmlMasterElement::_cinit(const parseFile& parsed) {
        try {
            if (parsed.dataSize > 0) {
                parseFile::iterator iter = parsed.begin();
                _cscanChildren(iter);
            }
        } catch (ebmlDecodeError& e) {
            if (e.cls != nullptr) {
                e.offset += parsed.ebmlIDWidth + parsed.sizeWidth;
            }
            throw;
        }
    }

    ebmlElement* ebmlMasterElement::_decodeChild(const parseString& parsed) const {
        return cls()._decodeChild(parsed);
    }

    ebmlElement* ebmlMasterElement::_cdecodeChild(const parseString& parsed) const {
        return cls()._cdecodeChild(parsed);
    }

    ebmlElement* ebmlMasterElement::_decodeChild(const parseFile& parsed) const {
        return cls()._decodeChild(parsed);
    }

    ebmlElement* ebmlMasterElement::_cdecodeChild(const parseFile& parsed) const {
        return cls()._cdecodeChild(parsed);
    }

    size_t ebmlMasterElement::dataSize() const {
        return sizetree().dataSize;
    }

    sizetree_t ebmlMasterElement::sizetree() const {
        sizetree_t result;

        auto iter = cbegin();
        auto end = cend();

        result.ebmlIDWidth = widthAsVint(cls().ebmlID);
        result.dataSize = 0;

        while (iter != end) {
            auto& elem = *iter;
            ++iter;

            auto me_elem = dynamic_cast<const ebmlMasterElement*>(&elem);

            if (me_elem != nullptr) {
                sizetree_t child_sizetree = me_elem->sizetree();
                result.append(std::move(child_sizetree));
            } else {
                auto dataSize = elem.dataSize();
                sizetree_t child_sizetree = sizetree_t(
                    widthAsVint(elem.ebmlID()),
                    widthAsVint(dataSize),
                    dataSize
                );
                result.append(std::move(child_sizetree));
            }
        }

        result.sizeWidth = widthAsVint(result.dataSize);
        return result;
    }

    void ebmlMasterElement::_decodeChildren(parseString::iterator& iter) {
        parseString parsed;
        ebmlElement_sp elem_sp;
        ebmlElement* elem;
        _clear();
        auto& c = cls();

        while (!iter.atEnd()) {
            parsed = *iter;
            ++iter;

            if (parsed.ebmlIDWidth == 0) {
                break;
            }

            if (parsed.ebmlID == Void.ebmlID) {
                continue;
            }

            try {
                elem = c._decodeChild(parsed);
                elem_sp = elem->sp();
            } catch (ebmlDecodeError& e) {
                if (e.cls != nullptr) {
                    e.offset += parsed.offset;
                }
                throw;
            }

            this->_attachChild(*elem);
            this->_addChild(std::move(elem_sp));
        }
    }

    void ebmlMasterElement::_cdecodeChildren(parseString::iterator& iter) {
        parseString parsed;
        ebmlElement* elem;
        this->_clear();
        auto& c = cls();

        while (!iter.atEnd()) {
            parsed = *iter;
            ++iter;

            if (parsed.ebmlIDWidth == 0) {
                break;
            }

            if (parsed.ebmlID == Void.ebmlID) {
                continue;
            }

            try {
                elem = c._cdecodeChild(parsed);
            } catch (ebmlDecodeError& e) {
                if (e.cls != nullptr) {
                    e.offset += parsed.offset;
                }
                throw;
            }

            auto elem_sp = elem->sp();
            this->_attachChild(*elem);
            this->_addChild(std::move(elem_sp));
        }
    }

    void ebmlMasterElement::_scanChildren(parseFile::iterator& iter) {
        std::cout << typeof(this) << "::_scanChildren(parseFile::iterator&)" << std::endl;
        while (!iter.atEnd()) {
            parseFile parsed = *iter;
            // std::cout << "D " << parsed.offset << " " << parsed.dataOffset() << " " << parsed.dataSize << std::endl;
            this->_handleParseFile(parsed);
            ++iter;
        }
    }

    void ebmlMasterElement::_cscanChildren(parseFile::iterator& iter) {
        while (!iter.atEnd()) {
            parseFile parsed = *iter;
            this->_chandleParseFile(parsed);
            ++iter;
        }
    }

    void ebmlMasterElement::_handleParseFile(const parseFile& parsed) {
        std::cout << typeof(this) << "::_handleParseFile(const parseFile&)" << std::endl;
        auto elem = cls()._decodeChild(parsed);
        std::cout << pack(elem->repr()) << std::endl;
        _addChild(elem->sp());
    }

    void ebmlMasterElement::_chandleParseFile(const parseFile& parsed) {
        auto elem = cls()._cdecodeChild(parsed);
        _addChild(elem->sp());
    }

    std::string ebmlMasterElement::encode() const {
        std::string result;
        auto _sizetree = sizetree();
        result.resize(_sizetree.outerSize());
        encode(&result[0], _sizetree);
        return result;
    }

    size_t ebmlMasterElement::encode(char* dest) const {
        auto _sizetree = sizetree();
        return encode(dest, _sizetree);
    }

    size_t ebmlMasterElement::encode(char* dest, const sizetree_t& _sizetree) const {
        size_t headWidth = _encode_head(dest, _sizetree.dataSize);

        if (_encode(dest + headWidth, _sizetree) != _sizetree.dataSize) {
            throw ebmlEncodeError("Encoded size does not match expected size.", sp());
        };

        return headWidth + _sizetree.dataSize;
    }


    size_t ebmlMasterElement::_encode(char* dest) const {
        auto _sizetree = sizetree();
        return _encode(dest, _sizetree);
    }

    size_t ebmlMasterElement::_encode(char* dest, const sizetree_t& _sizetree) const {
        size_t offset = 0;

        auto elem_iter = cbegin();
        auto elem_end = cend();

        if (!_sizetree.children.has_value()) {
            throw std::runtime_error("sizetree_t::children has no value");
        }

        auto tree_iter = _sizetree.children->cbegin();
        auto tree_end = _sizetree.children->cend();

        while (elem_iter != elem_end) {
            if (tree_iter == tree_end) {
                throw std::runtime_error("Unexpected end of branch.");
            }
            auto& elem = *elem_iter;
            auto me_elem = dynamic_cast<const ebmlMasterElement*>(&elem);

            if (me_elem != nullptr) {
                offset += me_elem->encode(dest + offset, *tree_iter);
            } else {
                offset += elem.encode(dest + offset, (*tree_iter).dataSize);
            }

            ++elem_iter;
            ++tree_iter;
        }

        if (tree_iter != tree_end) {
            throw std::runtime_error("Branch continues past expected end.");
        }

        return offset;
    }

    size_t ebmlMasterElement::encode(ioBase& dest) const {
        auto _sizetree = sizetree();
        auto buffer = std::make_unique<char[]>(_sizetree.outerSize());
        auto outerSize = encode(buffer.get(), _sizetree);
        return dest.write(buffer.get(), outerSize);
    }

    size_t ebmlMasterElement::encode(ioBase& dest, off_t offset) const {
        auto _sizetree = sizetree();
        auto buffer = std::make_unique<char[]>(_sizetree.outerSize());
        auto outerSize = encode(buffer.get(), _sizetree);
        return dest.write(buffer.get(), offset, outerSize);
    }

    void ebmlMasterElement::_clear() {
        throw ebmlNotImplementedError("_clear() not implemented.");
    }

    void ebmlMasterElement::_attachChild(const ebmlElement_sp& child, bool weak) {
        if (child.get() == this) {
            throw ebmlException("cannot attach element to itself");
        } else if (cls().childTypes().count(child->ebmlID()) == 0) {
            std::string errmsg = "cannot add '";
            errmsg += pack(child->cls().name);
            errmsg += "' object to parent";
            // throw ebmlException(pack<std::wstring>(errmsg));
            throw ebmlException(errmsg);
        }

        child->_setParent(sp(), weak);
    }

    void ebmlMasterElement::_attachChild(const ebmlElement_sp& child, bool weak) const {
        if (child.get() == this) {
            throw ebmlException("cannot attach element to itself");
        } else if (cls().childTypes().count(child->ebmlID()) == 0) {
            std::string errmsg = "cannot add '";
            errmsg += pack(child->cls().name);
            errmsg += "' object to parent";
            // throw ebmlException(pack<std::wstring>(errmsg));
            throw ebmlException(errmsg);
        }

        child->_setParent(sp(), weak);
    }

    void ebmlMasterElement::_attachChild(ebmlElement& child, bool weak) {
        if (&child == this) {
            throw ebmlException("cannot attach element to itself");
        } else if (cls().childTypes().count(child.ebmlID()) == 0) {
            std::string errmsg = "cannot add '";
            errmsg += pack(child.cls().name);
            errmsg += "' object to parent";
            // throw ebmlException(pack<std::wstring>(errmsg));
            throw ebmlException(errmsg);
        }

        child._setParent(*this, weak);
    }

    void ebmlMasterElement::_attachChild(ebmlElement& child, bool weak) const {
        if (&child == this) {
            throw ebmlException("cannot attach element to itself");
        } else if (cls().childTypes().count(child.ebmlID()) == 0) {
            std::string errmsg = "cannot add '";
            errmsg += pack(child.cls().name);
            errmsg += "' object to parent";
            // throw ebmlException(pack<std::wstring>(errmsg));
            throw ebmlException(errmsg);
        }

        child._setParent(*this, weak);
    }

    void ebmlMasterElement::_detachChild(const ebmlElement_sp& child) const {
        auto p = child->c_parent();

        if (p.get() != this) {
            throw ebmlException("_detachChild(): Element is not child of this.");
        }

        child->_detach();
    }

    void ebmlMasterElement::_attachChildren(const ebmlElement_l& elems, bool weak) {
        auto start = elems.cbegin();
        auto end = elems.cend();
        auto iter = start;

        while (iter != end) {
            try {
                const auto& elem = *iter;

                if (elem.get() == this) {
                    throw ebmlException("cannot attach element to itself");
                }

                elem->_setParent(sp(), weak);
                // ebmlElement::_attachChild(elem, weak);
                ++iter;
            } catch (...) {
                while (iter != start) {
                    --iter;
                    this->_detachChild(*iter);
                }
                throw;
            }
        }
    }

    void ebmlMasterElement::_attachChildren(const ebmlElement_l& elems, bool weak) const {
        auto start = elems.cbegin();
        auto end = elems.cend();
        auto iter = start;

        while (iter != end) {
            try {
                const auto& elem = *iter;

                if (elem.get() == this) {
                    throw ebmlException("cannot attach element to itself");
                }

                elem->_setParent(sp(), weak);
                // ebmlElement::_attachChild(elem, weak);
                ++iter;
            } catch (...) {
                while (iter != start) {
                    --iter;
                    this->_detachChild(*iter);
                }
                throw;
            }
        }
    }

    void ebmlMasterElement::_detachChildren(const ebmlElement_l& elems) const {
        auto start = elems.cbegin();
        auto end = elems.cend();
        auto iter = start;

        while (iter != end) {
            this->_detachChild(*iter);
        }
    }

    void ebmlMasterElement::_clonedata(const ebmlElement* elem) {
        auto melem = dynamic_cast<const ebmlMasterElement*>(elem);

        if (melem == nullptr) {
            throw std::invalid_argument("Unable to cast to const ebmlMasterElement*");
        }

        this->_clear();

        auto iter = melem->cbegin();
        auto end = melem->cend();

        while (iter != end) {
            auto& childelem = *iter;
            auto copy = childelem.clone();
            this->_addChild(copy);
            ++iter;
        }
    }

    ebmlMasterElement::iterator ebmlMasterElement::begin() {
        auto iter = this->_begin();
        return ebmlMasterElement::iterator(iter);
    }

    ebmlMasterElement::iterator ebmlMasterElement::end() {
        auto iter = this->_end();
        return ebmlMasterElement::iterator(iter);
    }

    ebmlMasterElement::iterator::iterator(ebmlMasterElement::_iterator* iter) {
        this->_iter = iter;
    }

    ebmlMasterElement::iterator::iterator(const ebmlMasterElement::iterator& other) {
        this->_iter = other._iter->copy();
    }

    ebmlMasterElement::iterator::iterator(ebmlMasterElement::iterator&& other) {
        this->_iter = std::exchange(other._iter, nullptr);
    }

    ebmlElement& ebmlMasterElement::iterator::operator*() const {
        return **this->_iter;
    }

    ebmlMasterElement::iterator& ebmlMasterElement::iterator::operator++() {
        ++(*this->_iter);
        return *this;
    }

    ebmlMasterElement::iterator& ebmlMasterElement::iterator::operator=(const ebmlMasterElement::iterator& other) {
        if (this != &other) {
            this->_iter = other._iter->copy();
        }

        return *this;
    }

    ebmlMasterElement::iterator& ebmlMasterElement::iterator::operator=(ebmlMasterElement::iterator&& other) {
        if (this != &other) {
            this->_iter = std::exchange(other._iter, nullptr);
        }

        return *this;
    }

    bool ebmlMasterElement::iterator::operator==(const iterator& other) const {
        return (*this->_iter) == (*other._iter);
    }

    bool ebmlMasterElement::iterator::operator!=(const iterator& other) const {
        return (*this->_iter) != (*other._iter);
    }

    ebmlMasterElement::iterator::~iterator() {
        if (this->_iter != nullptr) {
            delete this->_iter;
        }
    }

    // CONST ITERATORS

    ebmlMasterElement::const_iterator ebmlMasterElement::cbegin() const {
        auto iter = _cbegin();
        return ebmlMasterElement::const_iterator(iter);
    }

    ebmlMasterElement::const_iterator ebmlMasterElement::cend() const {
        auto iter = this->_cend();
        return ebmlMasterElement::const_iterator(iter);
    }

    ebmlMasterElement::iterator ebmlMasterElement::make_iter(_iterator* _iter) {
        return ebmlMasterElement::iterator(_iter);
    }

    ebmlMasterElement::const_iterator ebmlMasterElement::make_iter(_const_iterator* _iter) {
        return ebmlMasterElement::const_iterator(_iter);
    }

    ebmlMasterElement::const_iterator::const_iterator(ebmlMasterElement::_const_iterator* iter) {
        this->_iter = iter;
    }

    ebmlMasterElement::const_iterator::const_iterator(const ebmlMasterElement::const_iterator& other) {
        this->_iter = other._iter->copy();
    }

    ebmlMasterElement::const_iterator::const_iterator(ebmlMasterElement::const_iterator&& other) {
        this->_iter = std::exchange(other._iter, nullptr);
    }

    const ebmlElement& ebmlMasterElement::const_iterator::operator*() const {
        return **this->_iter;
    }

    ebmlMasterElement::const_iterator& ebmlMasterElement::const_iterator::operator++() {
        ++(*this->_iter);
        return *this;
    }

    ebmlMasterElement::const_iterator& ebmlMasterElement::const_iterator::operator=(const ebmlMasterElement::const_iterator& other) {
        if (this != &other) {
            this->_iter = other._iter->copy();
        }

        return *this;
    }

    ebmlMasterElement::const_iterator& ebmlMasterElement::const_iterator::operator=(ebmlMasterElement::const_iterator&& other) {
        if (this != &other) {
            this->_iter = std::exchange(other._iter, nullptr);
        }

        return *this;
    }

    bool ebmlMasterElement::const_iterator::operator==(const const_iterator& other) const {
        return (*this->_iter) == (*other._iter);
    }

    bool ebmlMasterElement::const_iterator::operator!=(const const_iterator& other) const {
        return (*this->_iter) != (*other._iter);
    }

    ebmlMasterElement::const_iterator::~const_iterator() {
        if (this->_iter != nullptr) {
            delete this->_iter;
        }
    }
}
#endif
