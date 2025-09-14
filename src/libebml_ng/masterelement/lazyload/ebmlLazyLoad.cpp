#ifndef EBML_NG_LAZYLOAD_BASE_CPP
#define EBML_NG_LAZYLOAD_BASE_CPP

#include "ebmlLazyLoad.h"
#include "../../ebmlVoid.h"
#include "../../ebmlElementType.tpp"
#include "../../ebmlElement.tpp"
#include "../../ebmlDocument.h"

namespace ebml {
    template class ebmlTypeCRTP<ebmlLazyLoadType, ebmlLazyLoad, ebmlMasterElementType>;
    template class ebmlElementCRTP<ebmlLazyLoadType, ebmlLazyLoad, ebmlMasterElement>;

    // ebmlLazyLoadType::ebmlLazyLoadType(const char* ebmlID, const std::wstring& name, const occurSpec_t& recursive) : ebmlLazyLoadType(unpackVint(ebmlID), name, recursive) {}

    ebmlLazyLoadType::ebmlLazyLoadType(ebmlID_t ebmlID, const std::wstring& name, const childTypeSpecArg_l & childClasses)
      : ebmlTypeCRTP<ebmlLazyLoadType, ebmlLazyLoad, ebmlMasterElementType>(ebmlID, name, childClasses) {}

    ebmlLazyLoadType::ebmlLazyLoadType(ebmlID_t ebmlID, const std::wstring& name, const childTypeSpecArg_l & childClasses, const seekHelper_t* seekHelper)
      : ebmlTypeCRTP<ebmlLazyLoadType, ebmlLazyLoad, ebmlMasterElementType>(ebmlID, name, childClasses, seekHelper) {}

    ebmlLazyLoadType::ebmlLazyLoadType(const char* ebmlID, const std::wstring& name, const childTypeSpecArg_l & childClasses)
      : ebmlTypeCRTP<ebmlLazyLoadType, ebmlLazyLoad, ebmlMasterElementType>(ebmlID, name, childClasses) {}

    ebmlLazyLoadType::ebmlLazyLoadType(const char* ebmlID, const std::wstring& name, const childTypeSpecArg_l & childClasses, const seekHelper_t* seekHelper)
      : ebmlTypeCRTP<ebmlLazyLoadType, ebmlLazyLoad, ebmlMasterElementType>(ebmlID, name, childClasses, seekHelper) {}

    ebmlLazyLoadType::ebmlLazyLoadType(ebmlID_t ebmlID, const std::wstring& name, const childTypeSpecArg_l & childClasses, const childElemHelper_t* childElemHelper)
      : ebmlTypeCRTP<ebmlLazyLoadType, ebmlLazyLoad, ebmlMasterElementType>(ebmlID, name, childClasses), _childHelper(childElemHelper) {}

    ebmlLazyLoadType::ebmlLazyLoadType(ebmlID_t ebmlID, const std::wstring& name, const childTypeSpecArg_l & childClasses, const seekHelper_t* seekHelper, const childElemHelper_t* childElemHelper)
      : ebmlTypeCRTP<ebmlLazyLoadType, ebmlLazyLoad, ebmlMasterElementType>(ebmlID, name, childClasses, seekHelper), _childHelper(childElemHelper) {}

    ebmlLazyLoadType::ebmlLazyLoadType(const char* ebmlID, const std::wstring& name, const childTypeSpecArg_l & childClasses, const childElemHelper_t* childElemHelper)
      : ebmlTypeCRTP<ebmlLazyLoadType, ebmlLazyLoad, ebmlMasterElementType>(ebmlID, name, childClasses), _childHelper(childElemHelper) {}

    ebmlLazyLoadType::ebmlLazyLoadType(const char* ebmlID, const std::wstring& name, const childTypeSpecArg_l & childClasses, const seekHelper_t* seekHelper, const childElemHelper_t* childElemHelper)
      : ebmlTypeCRTP<ebmlLazyLoadType, ebmlLazyLoad, ebmlMasterElementType>(ebmlID, name, childClasses, seekHelper), _childHelper(childElemHelper) {}

    // ebmlLazyLoadType::ebmlLazyLoadType(ebmlID_t ebmlID, const std::wstring& name, childClassSpecArg_init_l childClasses, const seekHelper_t* seekHelper)
    // : ebmlMasterElementClass(ebmlID, name, seekHelper), _childClasses(childClasses) {}
    //
    // ebmlLazyLoadType::ebmlLazyLoadType(const char* ebmlID, const std::wstring& name, childClassSpecArg_init_l childClasses)
    // : ebmlLazyLoadType(unpackVint(ebmlID), name, childClasses) {}


    ebmlLazyLoad::ebmlLazyLoad(const ebmlLazyLoadType * cls)
      : ebmlElementCRTP<ebmlLazyLoadType, ebmlLazyLoad, ebmlMasterElement>(cls) {
        for (auto& pair : cls->childTypes()) {
            if (auto keymap = pair.second->initSeekMap()) {
                _children_by_key.insert({pair.first, std::unique_ptr<seekMapBase>(keymap)});
            }
        }
    }

    ebmlLazyLoad::ebmlLazyLoad(const ebmlLazyLoadType * cls, ioBase& file, off_t offset, vintWidth_t sizeWidth, size_t dataSize) : ebmlLazyLoad(cls) {
        size_t trunc_offset = _initNew(file, offset, sizeWidth, dataSize);
        _offsetInParent = offset;
        file.truncate(offset + trunc_offset);
    }

    ebmlLazyLoad::ebmlLazyLoad(const ebmlLazyLoadType * cls, ebmlLazyLoad& parent, off_t offset, vintWidth_t sizeWidth, size_t dataSize) : ebmlLazyLoad(cls) {
        _setParent(parent.sp<ebmlMasterElement>(), true);
        auto& f = parent.file();
        _initNew(f, parent.dataOffsetInFile() + offset, sizeWidth, dataSize);
        _offsetInParent = offset;
    }

    size_t ebmlLazyLoad::_initNew(ioBase& file, off_t offset, vintWidth_t sizeWidth, size_t dataSize) {
        if ((dataSize < UNKNOWN) and (dataSize >= ((1LL << (7*sizeWidth)) - 1))) {
            std::string errmsg = "size ";
            errmsg += std::to_string(dataSize);
            errmsg += " too large for sizeWidth ";
            errmsg += std::to_string(sizeWidth);
            throw std::invalid_argument(errmsg);
        }

        _sizeWidth = sizeWidth;
        _dataSize = dataSize;

        char buffer[25]; // Room enough for ebmlID, size, and VOID element
        vintWidth_t ebmlIDWidth = widthAsVint(cls().ebmlID);
        unsigned char headSize = ebmlIDWidth + _sizeWidth;

        packVint(_cls->ebmlID, ebmlIDWidth, buffer);
        packVint(dataSize, _sizeWidth, buffer + ebmlIDWidth);

        unsigned char voidWidth = 0;

        if (dataSize > 0) {
            voidWidth = Void.writeVoid(buffer + headSize, dataSize);
        } else {
            voidWidth = Void.writeVoid(buffer + headSize, UNKNOWN);
        }

        file.write(buffer, offset, headSize + voidWidth);

        if (dataSize > 0) {
            return headSize + dataSize;
        } else {
            return headSize + voidWidth;
        }
    }

    const children_m& ebmlLazyLoad::seeksByOffset() const {
        return _children_by_offset;
    }

    ioBase& ebmlLazyLoad::file() const {
        auto doc = document();

        if (doc != nullptr) {
            return doc->file();
        }

        throw std::runtime_error("element does not have a document");
    }

    size_t ebmlLazyLoad::dataSize() const {
        return _dataSize;
    }

    void ebmlLazyLoad::_handleParseFile(const parseFile& parsed) {
        if (parsed.ebmlID == Void.ebmlID) {
            return;
        }

        auto childClasses = cls().childTypes();
        const ebmlElementType * childClass;
        seekData_t* seekData;
        auto _wp = wp<ebmlMasterElement>();

        if (childClasses.count(parsed.ebmlID)) {
            childClass = childClasses[parsed.ebmlID];
            seekData = childClass->makeSeekData(parsed, std::move(_wp));
        } else {
            seekData = new seekData_t(parsed);
            seekData->parent = std::move(_wp);
        }

        auto seekData_up = std::unique_ptr<seekData_t>(seekData);
        _handleSeekData(std::move(seekData_up));
    }

    void ebmlLazyLoad::_handleSeekData(std::unique_ptr<seekData_t>&& ptr) {
        _handleSeekData(std::move(ptr), _children_by_offset.end());
    }

    void ebmlLazyLoad::_handleSeekData(std::unique_ptr<seekData_t>&& ptr, const children_m::iterator& hint) {
        auto& seekData = *ptr;
        _children_by_offset.insert(hint, {seekData.offsetInParent, std::move(ptr)});

        auto& children_by_ebmlID = _children_by_ebmlID[seekData.ebmlID];
        children_by_ebmlID.insert(children_by_ebmlID.end(), {seekData.offsetInParent, &seekData});

        if (_children_by_key.count(seekData.ebmlID)) {
            _children_by_key.at(seekData.ebmlID)->add(&seekData);
        }
    }

    readLock_t ebmlLazyLoad::getRLock() const {
        std::vector<std::shared_lock<std::shared_mutex>> locks;
        locks.push_back(std::shared_lock<std::shared_mutex>(_mutex));

        const ebmlLazyLoad* child = this;
        auto parent_p = c_parent();

        while (auto recast = dynamic_cast<const ebmlLazyLoad*>(parent_p.get())) {
            auto& parent = *recast;
            {
                auto lock = std::shared_lock<std::shared_mutex>(parent._mutex);

                if (child->c_parent().get() != recast) {
                    throw std::runtime_error("Child orphaned while attempting to acquire lock");
                }

                locks.push_back(std::move(lock));
            }
            child = recast;
            parent_p = parent.c_parent();

            if (parent_p == nullptr) {
                break;
            }
        }

        return {std::move(locks), this};
    }

    readLock_t ebmlLazyLoad::getRLock(const readLock_t& rlock) const {
        std::vector<std::shared_lock<std::shared_mutex>> locks;

        if (rlock.elem == this) {
            return {std::move(locks), this};
        }

        locks.push_back(std::shared_lock<std::shared_mutex>(_mutex));

        const ebmlLazyLoad* child = this;
        auto parent_p = c_parent();

        while (auto recast = dynamic_cast<const ebmlLazyLoad*>(parent_p.get())) {
            if (recast == rlock.elem) {
                break;
            }

            auto& parent = *recast;
            {
                auto lock = std::shared_lock<std::shared_mutex>(parent._mutex);

                if (child->c_parent().get() != recast) {
                    throw std::runtime_error("Child orphaned while attempting to acquire lock");
                }

                locks.push_back(std::move(lock));
            }
            child = recast;
            parent_p = parent.c_parent();

            if (parent_p == nullptr) {
                break;
            }
        }

        return {std::move(locks), this};
    }

    writeLock_t ebmlLazyLoad::getWLock() const {
        std::vector<std::shared_lock<std::shared_mutex>> rlocks;

        const ebmlLazyLoad* child = this;
        auto parent_p = c_parent();

        while (auto recast = dynamic_cast<const ebmlLazyLoad*>(parent_p.get())) {
            auto& parent = *recast;
            {
                auto lock = std::shared_lock<std::shared_mutex>(parent._mutex);

                if (child->c_parent().get() != recast) {
                    throw std::runtime_error("Child orphaned while attempting to acquire lock");
                }

                rlocks.push_back(std::move(lock));
            }

            child = recast;
            parent_p = parent.c_parent();

            if (parent_p == nullptr) {
                break;
            }
        }

        auto wlock = std::unique_lock<std::shared_mutex>(_mutex);
        return {std::move(rlocks), std::move(wlock), this};
    }

    writeLock_t ebmlLazyLoad::getWLock(const readLock_t& rlock) const {
        std::vector<std::shared_lock<std::shared_mutex>> rlocks;

        const ebmlLazyLoad* child = this;
        auto parent_p = c_parent();

        while (auto recast = dynamic_cast<const ebmlLazyLoad*>(parent_p.get())) {
            if (recast == rlock.elem) {
                break;
            }

            auto& parent = *recast;
            {
                auto lock = std::shared_lock<std::shared_mutex>(parent._mutex);

                if (child->c_parent().get() != recast) {
                    throw std::runtime_error("Child orphaned while attempting to acquire lock");
                }

                rlocks.push_back(std::move(lock));
            }

            child = recast;
            parent_p = parent.c_parent();

            if (parent_p == nullptr) {
                break;
            }
        }

        auto wlock = std::unique_lock<std::shared_mutex>(_mutex);
        return {std::move(rlocks), std::move(wlock), this};
    }

    writeLock_t ebmlLazyLoad::getWLock(const writeLock_t& wlock) const {
        _verifyLock(wlock);
        return {{}, {}, this};
    }

    writeLock_t ebmlLazyLoad::getGlobalLock() const {
        auto _root = c_root();

        if (auto recast = dynamic_cast<const ebmlLazyLoad*>(_root.get())) {
            auto wlock = std::unique_lock<std::shared_mutex>(recast->_mutex);
            return {std::move(wlock), recast};
        }

        throw std::bad_cast();
    }

    void ebmlLazyLoad::_verifyLock(const writeLock_t& lock) const {
        if (lock.elem != nullptr) {
            ebml::ptr<const ebmlMasterElement> elem = sp();

            while (auto ptr = elem.get()) {
                if (ptr == nullptr) {
                    break;
                }

                if (lock.elem == ptr) {
                    return;
                }
                elem = elem->c_parent();
            }
        }

        throw std::invalid_argument("invalid write lock");
    }

    ebml::ptr<ebmlElement> ebmlLazyLoad::_read_elem(seekData_t& seekData) {
        auto& f = file();
        auto parentOffset = dataOffsetInFile();
        off_t offset = parentOffset + seekData.offsetInParent;
        auto parsed = parseFile(f, offset);
        parsed.parentOffset = parentOffset;
        // auto elem = this->_read_elem(parsed);
        auto elem = _decodeChild(parsed)->sp();
        _attachChild(elem, false);
        seekData.ref = elem->wp();
        return elem;
    }

    ebml::ptr<const ebmlElement> ebmlLazyLoad::_read_elem(seekData_t& seekData) const {
        auto& f = file();
        auto parentOffset = dataOffsetInFile();
        off_t offset = parentOffset + seekData.offsetInParent;
        auto parsed = parseFile(f, offset);
        parsed.parentOffset = parentOffset;
        // auto elem = this->_read_elem(parsed);
        auto elem = _cdecodeChild(parsed)->sp();
        _attachChild(elem, false);
        seekData.ref = elem->wp();
        return elem;
    }

    ebml::ptr<ebmlElement> ebmlLazyLoad::_get(off_t offset) {
        if (_children_by_offset.count(offset) == 0) {
            std::string errmsg = "element not found at offset ";
            errmsg += std::to_string(offset);
            throw ebmlDecodeError(errmsg, DECODE_ERR_DEFAULT, offset);
        }

        seekData_t& seekData = *_children_by_offset[offset];

        if (auto elem = seekData.ref.get()) {
            return elem;
        }

        return _read_elem(seekData);
    }

    ebml::ptr<ebmlElement> ebmlLazyLoad::_get(ebmlID_t ebmlID, size_t index) {
        if (_children_by_ebmlID.count(ebmlID) == 0) {
            std::string errmsg = "no elements with ebmlID ";
            errmsg += std::to_string(ebmlID);
            errmsg += " found";
            throw ebmlDecodeError(errmsg, DECODE_ERR_DEFAULT);
        }

        auto& items = _children_by_ebmlID.at(ebmlID);

        if (index > items.size()) {
            throw std::out_of_range("list index out of range");
        }

        auto iter = items.begin();

        for (unsigned long k = 0; k < index; ++k) {
            ++iter;
        }

        seekData_t& seekData = *iter->second;

        if (auto elem = seekData.ref.get()) {
            return elem;
        }

        return _read_elem(seekData);
    }

    ebml::ptr<const ebmlElement> ebmlLazyLoad::_cget(off_t offset) const {
        if (_children_by_offset.count(offset) == 0) {
            std::string errmsg = "element not found at offset ";
            errmsg += std::to_string(offset);
            throw ebmlDecodeError(errmsg, DECODE_ERR_DEFAULT, offset);
        }

        seekData_t& seekData = *_children_by_offset.at(offset);

        if (auto elem = seekData.ref.cget()) {
            return elem;
        }

        return _read_elem(seekData);
    }

    ebml::ptr<const ebmlElement> ebmlLazyLoad::_cget(ebmlID_t ebmlID, size_t index) const {
        if (_children_by_ebmlID.count(ebmlID) == 0) {
            std::string errmsg = "no elements with ebmlID ";
            errmsg += std::to_string(ebmlID);
            errmsg += " found";
            throw ebmlDecodeError(errmsg, DECODE_ERR_DEFAULT);
        }

        auto& items = _children_by_ebmlID.at(ebmlID);

        if (index > items.size()) {
            throw std::out_of_range("list index out of range");
        }

        auto iter = items.begin();

        for (unsigned long k = 0; k < index; ++k) {
            ++iter;
        }

        seekData_t& seekData = *iter->second;

        // seekData_t& seekData = *items.at(index);

        if (auto elem = seekData.ref.get()) {
            return elem;
        }

        return _read_elem(seekData);
    }

    std::pair<children_m::iterator, children_m::iterator> ebmlLazyLoad::_find(off_t offset, bool left) {
        if (this->_children_by_offset.size() == 0) {
            return {this->_children_by_offset.begin(), this->_children_by_offset.end()};
        }

        children_m::iterator next;

        if (left) {
            next = this->_children_by_offset.lower_bound(offset);
        } else {
            next = this->_children_by_offset.upper_bound(offset);
        }

        auto prev = next;
        --prev;
        return {std::move(prev), std::move(next)};
    }

    /* TODO: REFACTOR */

    children_m::iterator ebmlLazyLoad::_findNext(off_t offset, bool strict) {
        if (strict) {
            return this->_children_by_offset.upper_bound(offset);
        } else {
            return this->_children_by_offset.lower_bound(offset);
        }
    }

    children_m::iterator ebmlLazyLoad::_findPrev(off_t offset, bool strict) {
        if (strict) {
            return --this->_children_by_offset.lower_bound(offset);
        } else {
            return --this->_children_by_offset.upper_bound(offset);
        }
    }

    // Insert status from sizetree.

    status_t<prepared_insert_t> ebmlLazyLoad::canInsert(off_t offset, sizetree_t&& sizetree, const writeLock_t& lock) {
        _verifyLock(lock);
        return _canInsert(offset, std::move(sizetree));
    }

    status_t<prepared_insert_t> ebmlLazyLoad::_canInsert(off_t offset, sizetree_t&& sizetree) {
        auto iters = _find(offset);
        auto begin = _children_by_offset.cbegin();
        auto end = _children_by_offset.cend();

        prepared_insert_t result = {{}, {offset, std::move(sizetree)}, static_cast<off_t>(_dataSize)};

        if (iters.second != begin) {
            auto& seekData = *iters.first->second;
            result.prev = {seekData.offsetInParent, {seekData.ebmlIDWidth, seekData.sizeWidth, seekData.dataSize}};
        }

        if (iters.second != end) {
            result.next = iters.second->second->offsetInParent;
        }

        if (offset < 0) {
            return {false, "offset cannot be negative", std::move(result)};
        }

        if ((result.next >= 0) and (result.extent.endOffset > result.next)) {
            return {false, "insertion extends past end of element", std::move(result)};
        }

        if (iters.second != begin) {
            if (result.prev.endOffset == -1) {
                return {false, "cannot write child after sibling with UNKNOWN size", std::move(result)};
            } else if (offset < result.prev.endOffset) {
                return {false, "cannot write child before end of previous sibling", std::move(result)};
            } else if (offset == result.prev.endOffset + 1) {
                return {false, "cannot write child exactly one byte after end of previous sibling", std::move(result)};
            }
        } else if (offset == 1) {
            return {false, "cannot write child at offset 1", std::move(result)};
        } else if (iters.second != end) {
            if (result.extent.endOffset == -1) {
                return {false, "cannot write child with UNKNOWN size before next sibling", std::move(result)};
            } else if (result.extent.endOffset > result.next) {
                return {false, "end of child extends past beginning of next sibling", std::move(result)};
            } else if (result.extent.endOffset == result.next - 1) {
                return {false, "child cannot end exactly one byte before beginning of next sibling", std::move(result)};
            }
        } else if (offset == 1) {
            return {false, "cannot write child at offset 1", std::move(result)};
        } else if (result.next >= 0) {
            if (result.extent.endOffset == -1) {
                return {false, "cannot write child with UNKNOWN size in element with known size", std::move(result)};
            } else if (result.extent.endOffset > result.next) {
                return {false, "child will extend past end of parent", std::move(result)};
            } else if (result.extent.endOffset == result.next - 1) {
                return {false, "child cannot end exactly one byte before end of parent", std::move(result)};
            }
        }

        result.hint = std::move(iters.second);
        return {true, std::move(result)};
    }

    // Insert from ebml::ptr<ebmlElement>

    status_t<prepared_insert_t> ebmlLazyLoad::_canInsert(off_t offset, const ebml::ptr<ebmlElement>& elem_p) {
        if (elem_p == nullptr) {
            throw std::invalid_argument("cannot attach nullptr");
        }

        auto& elem = *elem_p;
        auto sizetree = sizetree_t(widthAsVint(elem.ebmlID()), elem.sizeWidth(), elem.dataSize());
        return _canInsert(offset, std::move(sizetree));
    }

    seekData_t* ebmlLazyLoad::_insert(off_t offset, const ebmlElement_sp& elem_p, const status_t<prepared_insert_t>& prepared) {
        if (!prepared) {
            auto elem = ebml_static_pointer_cast<const ebmlElement>(sp());
            prepared.throw_exc(std::move(elem));
        }

        auto& elem = *elem_p;
        auto& prepared_insert = prepared.data();

        auto& helper = *cls()._childHelper;
        helper._onBeforeInsert(*this, offset, elem_p);

        auto& f = file();
        auto poffset = dataOffsetInFile();
        _attachChild(elem_p, false);

        try {
            if (prepared_insert.prev.endOffset < offset) {
                Void.writeVoid(f, poffset + prepared_insert.prev.endOffset, offset - prepared_insert.prev.endOffset);
            }

            elem.encode(f, poffset + offset);
            elem._offsetInParent = offset;

            if (prepared_insert.extent.endOffset < prepared_insert.next) {
                Void.writeVoid(f, poffset + prepared_insert.extent.endOffset, prepared_insert.next - prepared_insert.extent.endOffset);
            }

            if ((prepared_insert.next == UNKNOWN) and (prepared_insert.extent.endOffset >= 0)) {
                auto vsize = Void.writeVoid(f, poffset + prepared_insert.extent.endOffset, UNKNOWN);
                f.truncate(poffset + prepared_insert.extent.endOffset + vsize);
            }
        } catch (const std::exception& e) {
            if (prepared_insert.next == UNKNOWN) {
                auto vsize = Void.writeVoid(f, poffset + prepared_insert.prev.endOffset, UNKNOWN);
                f.truncate(poffset + prepared_insert.prev.endOffset + vsize);
            } else {
                Void.writeVoid(f, poffset + prepared_insert.prev.endOffset, prepared_insert.next - prepared_insert.prev.endOffset);
            }

            _detachChild(elem_p);
            throw;
        }

        seekData_t* seekData = elem_p->makeSeekData();
        auto seekData_up = std::unique_ptr<seekData_t>(seekData);
        _handleSeekData(std::move(seekData_up), prepared_insert.hint);
        helper._onAfterInsert(*this, offset, elem_p, seekData);
        return seekData;
    }

    // Insert from std::string

    status_t<prepared_insert_t> ebmlLazyLoad::_canInsert(off_t offset, const std::string& data) {
        auto parsed = parseString(data.data(), data.size());

        if (parsed.outerSize() < data.size()) {
            throw ebmlDataContinues("_canInsert(): Data continues past expected end.", 0);
        }

        auto sizetree = sizetree_t(parsed.ebmlIDWidth, parsed.sizeWidth, parsed.dataSize);
        return _canInsert(offset, std::move(sizetree));
    }

    // Insert from const char*

    seekData_t* ebmlLazyLoad::_insert(off_t offset, const char* data, const status_t<prepared_insert_t>& status) {
        if (!status) {
            auto elem = ebml_static_pointer_cast<const ebmlElement>(sp());
            status.throw_exc(std::move(elem));
        }

        auto& prepared_insert = status.data();
        size_t size = prepared_insert.extent.endOffset - prepared_insert.extent.offset;
        auto parsed = parseString(data, size, offset);
        auto& c = cls();
        const childTypeSpec_t& subclss = c.childTypes();
        const ebmlElementType * subcls = subclss[parsed.ebmlID];

        // TODO
        // cls._childHelper->_onBeforeInsert(*this, parsed);

        auto& f = file();
        auto poffset = dataOffsetInFile();

        try {
            if (prepared_insert.prev.endOffset < offset) {
                Void.writeVoid(f, poffset + prepared_insert.prev.endOffset, offset - prepared_insert.prev.endOffset);
            }

            f.write(data, poffset + offset, size);

            if (prepared_insert.extent.endOffset < prepared_insert.next) {
                Void.writeVoid(f, poffset + prepared_insert.extent.endOffset, prepared_insert.next - prepared_insert.extent.endOffset);
            }

            if ((prepared_insert.next == UNKNOWN) and (prepared_insert.extent.endOffset >= 0)) {
                auto vsize = Void.writeVoid(f, poffset + prepared_insert.extent.endOffset, UNKNOWN);
                f.truncate(poffset + prepared_insert.extent.endOffset + vsize);
            }
        } catch (const std::exception& e) {
            if (prepared_insert.next == UNKNOWN) {
                auto vsize = Void.writeVoid(f, poffset + prepared_insert.prev.endOffset, UNKNOWN);
                f.truncate(poffset + prepared_insert.prev.endOffset + vsize);
            } else {
                Void.writeVoid(f, poffset + prepared_insert.prev.endOffset, prepared_insert.next - prepared_insert.prev.endOffset);
            }

            throw;
        }

        seekData_t* seekData = subcls->makeSeekData(parsed);
        // seekData_t* seekData = subcls->_seekHelper->makeSeekData(parsed);
        auto seekData_up = std::unique_ptr<seekData_t>(seekData);
        _handleSeekData(std::move(seekData_up), prepared_insert.hint);

        // TODO
        // cls._childHelper->_onAfterInsert(*this, parsed, seekData);
        return seekData;
    }


    // Insert multiple elements

    status_t<prepared_multiinsert_t> ebmlLazyLoad::canInsert(off_t offset, const ebmlElement_l& elems, const writeLock_t& wlock) {
        _verifyLock(wlock);
        return _canInsert(offset, elems);
    }

    status_t<prepared_multiinsert_t> ebmlLazyLoad::_canInsert(off_t offset, const ebmlElement_l& elems) {
        std::vector<sizetree_t> sizetrees;

        for (const auto& elem : elems) {
            auto& elem_ref = *elem;

            if (auto recast = dynamic_cast<ebmlMasterElement*>(&elem_ref)) {
                sizetrees.push_back(recast->sizetree());
            } else {
                size_t dataSize = elem_ref.dataSize();
                vintWidth_t ebmlIDWidth = widthAsVint(elem_ref.ebmlID());
                vintWidth_t sizeWidth = elem_ref.sizeWidth(dataSize);
                sizetrees.push_back({ebmlIDWidth, sizeWidth, dataSize});
            }
        }

        return _canInsert(offset, std::move(sizetrees));
    }

    status_t<prepared_multiinsert_t> ebmlLazyLoad::canInsert(off_t offset, std::vector<sizetree_t>&& sizetrees, const writeLock_t& wlock) {
        _verifyLock(wlock);
        return _canInsert(offset, std::move(sizetrees));
    }

    status_t<prepared_multiinsert_t> ebmlLazyLoad::_canInsert(off_t offset, std::vector<sizetree_t>&& sizetrees) {
        auto iters = _find(offset);
        auto begin = _children_by_offset.cbegin();
        auto end = _children_by_offset.cend();

        prepared_multiinsert_t result = {{}, {}, static_cast<off_t>(_dataSize)};

        for (auto& sizetree : sizetrees) {
            result.extents.push_back({offset, std::move(sizetree)});
            offset += result.extents.back().endOffset;
        }

        sizetrees.clear();

        if (iters.second != begin) {
            auto& seekData = *iters.first->second;
            result.prev = {seekData.offsetInParent, {seekData.ebmlIDWidth, seekData.sizeWidth, seekData.dataSize}};
        }

        if (iters.second != end) {
            result.next = iters.second->second->offsetInParent;
        }

        if (offset < 0) {
            return {false, "offset cannot be negative", std::move(result)};
        }

        if ((result.next >= 0) and (result.extents.back().endOffset > result.next)) {
            return {false, "insertion extends past end of element", std::move(result)};
        }

        if (iters.second != begin) {
            if (result.prev.endOffset == -1) {
                return {false, "cannot write child after sibling with UNKNOWN size", std::move(result)};
            } else if (offset < result.prev.endOffset) {
                return {false, "cannot write child before end of previous sibling", std::move(result)};
            } else if (offset == result.prev.endOffset + 1) {
                return {false, "cannot write child exactly one byte after end of previous sibling", std::move(result)};
            }
        } else if (offset == 1) {
            return {false, "cannot write child at offset 1", std::move(result)};
        } else if (iters.second != end) {
            if (result.extents.back().endOffset == -1) {
                return {false, "cannot write child with UNKNOWN size before next sibling", std::move(result)};
            } else if (result.extents.back().endOffset > result.next) {
                return {false, "end of child extends past beginning of next sibling", std::move(result)};
            } else if (result.extents.back().endOffset == result.next - 1) {
                return {false, "child cannot end exactly one byte before beginning of next sibling", std::move(result)};
            }
        } else if (offset == 1) {
            return {false, "cannot write child at offset 1", std::move(result)};
        } else if (result.next >= 0) {
            if (result.extents.back().endOffset == -1) {
                return {false, "cannot write child with UNKNOWN size in element with known size", std::move(result)};
            } else if (result.extents.back().endOffset > result.next) {
                return {false, "child will extend past end of parent", std::move(result)};
            } else if (result.extents.back().endOffset == result.next - 1) {
                return {false, "child cannot end exactly one byte before end of parent", std::move(result)};
            }
        }

        result.hint = std::move(iters.second);
        return {true, std::move(result)};
    }

    status_t<prepared_remove_t> ebmlLazyLoad::_canRemove(off_t offset) {
        auto iter = _children_by_offset.find(offset);
        prepared_remove_t result = {0, offset, static_cast<off_t>(_dataSize), nullptr, iter};

        if (iter == _children_by_offset.end()) {
            return {false, "no element at offset", std::move(result)};
        }

        auto previter = iter; --previter;

        if (previter != _children_by_offset.begin()) {
            auto& seekData = *previter->second;
            result.prev = {seekData.offsetInParent, {seekData.ebmlIDWidth, seekData.sizeWidth, seekData.dataSize}};
        }

        auto nextiter = iter; ++nextiter;

        if (nextiter != _children_by_offset.end()) {
            result.next = nextiter->second->offsetInParent;
        }

        result.elem = iter->second->ref.get();
        return {true, std::move(result)};
    }

    void ebmlLazyLoad::_remove(off_t offset, const status_t<prepared_remove_t>& status) {
        if (!status) {
            auto elem = ebml_static_pointer_cast<const ebmlElement>(sp());
            status.throw_exc(std::move(elem));
        }

        auto& prepared_remove = status.data();
        auto& f = this->file();
        auto dataOffsetInFile = this->dataOffsetInFile();

        if (prepared_remove.next == -1) {
            auto vsize = Void.writeVoid(f, dataOffsetInFile + prepared_remove.prev.endOffset, UNKNOWN);
            f.truncate(dataOffsetInFile + prepared_remove.prev.endOffset + vsize);
        } else {
            Void.writeVoid(f, dataOffsetInFile + prepared_remove.prev.endOffset, prepared_remove.next - prepared_remove.prev.endOffset);
        }

        auto& pair = *prepared_remove.hint;
        _children_by_ebmlID.at(pair.second->ebmlID).erase(offset);

        if (prepared_remove.elem != nullptr) {
            _detachChild(prepared_remove.elem);
        }

        auto seekData_up = std::move(pair.second);
        auto& seekData = *seekData_up;

        if (_children_by_key.count(seekData.ebmlID)) {
            _children_by_key.at(seekData.ebmlID)->rm(&seekData);
        }

        _children_by_offset.erase(offset);
        // this->_postremove_hook(seekData.get()); // TODO
    }

    void ebmlLazyLoad::_resize(size_t size, const status_t<prepared_resize_t>& status) {
        if (!status) {
            status.throw_exc(sp());
        }

        auto& prepared_resize = status.data();
        vintWidth_t vheadsize = 0;

        char sizeVint[8];
        packVint(size, _sizeWidth, sizeVint);

        auto& f = file();
        auto dataOffsetInFile = this->dataOffsetInFile();
        f.write(sizeVint, dataOffsetInFile - _sizeWidth, _sizeWidth);
        _dataSize = size;

        if (size == UNKNOWN) {
            vheadsize = Void.writeVoid(f, dataOffsetInFile + prepared_resize.lastChild.endOffset, UNKNOWN);
        } else if (size > prepared_resize.lastChild.endOffset) {
            vheadsize = Void.writeVoid(f, dataOffsetInFile + prepared_resize.lastChild.endOffset, size - prepared_resize.lastChild.endOffset);
        }

        if (size < UNKNOWN) {
            if ( prepared_resize.seekData != nullptr) {
                off_t endOffsetInParent = dataOffsetInParent() + size;
                prepared_resize.seekData->dataSize = size;

                if (prepared_resize.nextSibling > endOffsetInParent) {
                    Void.writeVoid(
                        f, dataOffsetInFile + _dataSize,
                        prepared_resize.nextSibling - endOffsetInParent);
                }
            } else {
                f.truncate(dataOffsetInFile + _dataSize);
            }
        } else {
            f.truncate(dataOffsetInFile + prepared_resize.lastChild.endOffset + vheadsize);
        }
    }

    status_t<prepared_resize_t> ebmlLazyLoad::_canResize(size_t size) {
        // auto parent = this->parent()->ptr<ebmlLazyLoad>();
        auto parent_sp = ebml_dynamic_pointer_cast<ebmlLazyLoad>(parent());
        prepared_resize_t result;

        off_t offset = offsetInParent();
        bool hasNextSibling = false;

        if (parent_sp != nullptr) {
            auto& parentref = *parent_sp;
            auto& children_by_offset = parentref._children_by_offset;
            auto iter = children_by_offset.find(offset);
            result.hint = iter;
            result.seekData = &*iter->second;

            ++iter;

            if (iter == children_by_offset.end()) {
                result.nextSibling = parentref.dataSize();
            } else {
                hasNextSibling = true;
                result.nextSibling = iter->second->offsetInParent;
            }
        }

        if (_children_by_offset.size() > 0) {
            auto iter = this->_children_by_offset.end(); --iter;
            auto& lastChild = *iter->second;
            result.lastChild = {lastChild.offsetInParent, {lastChild.ebmlIDWidth, lastChild.sizeWidth, lastChild.dataSize}};
            // lastChildOffset = lastChild.offsetInParent;
            // lastChildEnd = lastChild.endOffset();
        }

        if (!_sizeSupported(size)) {
            return {false, "size not supported", std::move(result)};
        }

        if ((size < UNKNOWN) and (result.lastChild.endOffset == -1)) {
            return {false, "child with unknown size not supported in parent with known size", std::move(result)};
        }

        if (size == result.lastChild.endOffset + 1) {
            return {false, "element cannot end one byte after end of last child", std::move(result)};
        }

        if (size < result.lastChild.endOffset) {
            return {false, "element cannot end before end of last child", std::move(result)};
        }

        if (result.nextSibling >= 0) {
            if (result.endOffset == result.endOffset - 1) {
                if (hasNextSibling) {
                    return {false, "element cannot end one byte before beginning of next sibling", std::move(result)};
                }
                return {false, "element cannot end one byte before end of parent", std::move(result)};
            }

            if (result.endOffset > result.nextSibling) {
                if (hasNextSibling) {
                    return {false, "element cannot end after beginning of next sibling", std::move(result)};
                }
                return {false, "element cannot end after end of parent", std::move(result)};
            }

            if (size == 1) {
                return {false, "element size cannot be 1", std::move(result)};
            }
        }

        return {true, std::move(result)};
    }
// if (readLock.elem == this) {
//     // If elem == this, convert the last shared_lock to unique_lock
//     std::shared_lock<std::shared_mutex>& lastSharedLock = readLocks.back();
//     auto lock = std::unique_lock<std::shared_mutex>(lastSharedLock.mutex(), std::adopt_lock);
//     return writeLock_t(std::move(writeLock), this);
// }
    // TODO
    std::wstring ebmlLazyLoad::minirepr() const {
        return L"";
    }

    // Do nothing
    void ebmlLazyLoad::_addChild(const ebmlElement_sp&) {}
    void ebmlLazyLoad::_addChild(ebmlElement_sp&&) {}

    // TODO
    ebmlMasterElement::_iterator* ebmlLazyLoad::_begin() {return nullptr;}
    ebmlMasterElement::_iterator* ebmlLazyLoad::_end() {return nullptr;}
    ebmlMasterElement::_const_iterator* ebmlLazyLoad::_cbegin() const {return nullptr;}
    ebmlMasterElement::_const_iterator* ebmlLazyLoad::_cend() const {return nullptr;}
}
#endif
