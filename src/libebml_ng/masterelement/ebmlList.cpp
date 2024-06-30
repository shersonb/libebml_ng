#ifndef EBML_NG_MASTERELEMENT_LIST_CPP
#define EBML_NG_MASTERELEMENT_LIST_CPP
#include <mutex>

#include "libebml_ng/masterelement/ebmlList.h"
#include "libebml_ng/masterelement/base/childSlot_t.tpp"
#include "libebml_ng/vint.h"
#include "libebml_ng/ebmlElementClass.tpp"
#include "libebml_ng/ebmlElement.tpp"

namespace ebml {
    ebmlListClass::ebmlListClass(const char* ebmlID, const std::wstring& name, const occurSpec_t& recursive)
    : ebmlListClass(unpackVint(ebmlID), name, recursive) {}

    ebmlListClass::ebmlListClass(const char* ebmlID, const std::wstring& name, const childClassSpecArg_l& childSpec, const occurSpec_t& recursive)
    : ebmlListClass(unpackVint(ebmlID), name, childSpec, recursive) {}

    ebmlListClass::ebmlListClass(const char* ebmlID, const std::wstring& name, childClassSpecArg_init_l childSpec, const occurSpec_t& recursive)
    : ebmlListClass(unpackVint(ebmlID), name, childSpec, recursive) {}

    ebmlListClass::ebmlListClass(ebmlID_t ebmlID, const std::wstring& name, const occurSpec_t& recursive) : ClsMixin<ebmlListClass, ebmlList, ebmlMasterElementClass>(ebmlID, name) {
        if (recursive.max != 0) {
            this->_childClasses.add({this, recursive.min, recursive.max});
        }
    }

    ebmlListClass::ebmlListClass(ebmlID_t ebmlID, const std::wstring& name, const childClassSpecArg_l& childSpec, const occurSpec_t& recursive) : ebmlListClass(ebmlID, name) {
        this->_childClasses = childSpec;

        if (recursive.max != 0) {
            this->_childClasses.add({this, recursive.min, recursive.max});
        }
    }

    ebmlListClass::ebmlListClass(ebmlID_t ebmlID, const std::wstring& name, childClassSpecArg_init_l childSpec, const occurSpec_t& recursive) : ebmlListClass(ebmlID, name) {
        this->_childClasses = childSpec;

        if (recursive.max != 0) {
            this->_childClasses.add({this, recursive.min, recursive.max});
        }
    }

    // ebmlElement_sp ebmlListClass::operator()() const {
    //     auto elem = new ebmlList(this);
    //     return std::shared_ptr<ebmlElement>(elem);
    // }

    // ebmlElement* ebmlListClass::_new() const {
    //     return new ebmlList(this);
    // }
    //
    // ebmlList_sp ebmlListClass::operator()() const {
    //     auto elem = new ebmlList(this);
    //     auto elem_sp = ebmlList_sp(elem);
    //     return elem_sp;
    // }

    ebmlList::ebmlList(const ebmlListClass* cls, const ebmlElement_l& items) : ebmlList(cls) {
        setData(items);
    }

    ebmlList::ebmlList(const ebmlListClass* cls, ebmlElement_l&& items) : ebmlList(cls) {
        setData(std::move(items));
    }

    ebmlList::ebmlList(const ebmlList& orig) : ebmlList(&orig.cls()) {
        _data.reserve(orig._data.size());

        for (const auto& elem : orig._data) {
            auto newelem = elem->clone();
            _attachChild(newelem);
            _data.push_back(std::move(newelem));
        }
    }

    ebmlList::ebmlList(const ebmlListClass* cls) : InstMixin<ebmlListClass, ebmlList, ebmlMasterElement>(cls) {}

    void ebmlList::_validateData(const ebmlElement_l& items) {
        auto start = items.cbegin();
        auto iter = start;
        auto end = items.cend();

        while (iter != end) {
            try {
                _attachChild(*iter);
                ++iter;
            } catch (...) {
                while (iter != start) {
                    --iter;
                    _detachChild(*iter);
                }
                throw;
            }
        }
    }

    void ebmlList::_setData(const ebmlElement_l& items) {
        _data.reserve(items.size());

        for (const auto& item : items) {
            _data.push_back(item);
        }
    }

    void ebmlList::setData(const ebmlElement_l& items) {
        this->_validateData(items);
        this->_clear();
        this->_setData(items);
    }

    void ebmlList::_setData(ebmlElement_l&& items) {
        _data.reserve(items.size());

        for (auto& item : items) {
            _data.push_back(std::move(item));
        }

        items.clear();
    }

    void ebmlList::setData(ebmlElement_l&& items) {
        this->_validateData(items);
        this->_clear();
        this->_setData(std::move(items));
    }

    std::wstring ebmlList::minirepr() const {
        std::wstring ret = L"[";

        if (_data.size()) {
            auto iter = _data.cbegin();
            auto end = _data.cend();

            auto elem = *iter;
            ret += elem->repr();
            ++iter;

            while (iter != end) {
                ret += L", ";
                elem = *iter;
                ret += elem->repr();
                ++iter;
            }
        }

        ret += L"]";

        return ret;
    }

    size_t ebmlList::size() const {
        return _data.size();
    }

    void ebmlList::_clear() {
        for (const auto& elem : _data) {
            this->_detachChild(elem);
        }

        _data.resize(0);
    }

    childSlot_t<> ebmlList::operator[](off_t offset) {
        return {this, cls().childClasses(), _data[offset]};
    }

    template<typename T>
    childSlot_t<T> ebmlList::at(off_t offset) {
        if ((offset < 0) or (offset >= _data.size())) {
            throw std::out_of_range("list index out of range");
        }

        return {this, cls().childClasses(), _data.at(offset)};
    }

    const ebmlElement& ebmlList::operator[](off_t offset) const {
        return *_data.at(offset);
    }

    const ebmlElement& ebmlList::at(off_t offset) const {
        return *_data.at(offset);
    }

    const ebmlElement& ebmlList::front() const {
        return *_data.front();
    }

    const ebmlElement& ebmlList::back() const {
        return *_data.back();
    }

    template<typename T>
    childSlot_t<T> ebmlList::front() {
        return {this, cls().childClasses(), _data.front()};
    }

    template<typename T>
    childSlot_t<T> ebmlList::back() {
        return {this, cls().childClasses(), _data.back()};
    }

    void ebmlList::insert(off_t offset, const ebmlElement_sp& elem) {
        if ((offset < 0) or (offset > _data.size())) {
            throw std::out_of_range("list insertion index out of range");
        }
        this->_attachChild(elem);
        _data.insert(_data.begin() + offset, elem);
    }

    void ebmlList::erase(off_t offset) {
        if ((0 <= offset) and (offset < _data.size())) {
            this->_detachChild(_data.at(offset));
        } else {
            throw std::out_of_range("list assignment index out of range");
        }
        _data.erase(_data.begin() + offset);
    }

    void ebmlList::push_back(const ebmlElement_sp& elem) {
        this->_attachChild(elem);
        _data.push_back(elem);
    }

    void ebmlList::push_back(ebmlElement_sp&& elem) {
        this->_attachChild(elem);
        _data.push_back(std::move(elem));
    }

    void ebmlList::_addChild(const ebmlElement_sp& elem) {
        _data.push_back(elem);
    }

    void ebmlList::_addChild(ebmlElement_sp&& elem) {
        _data.push_back(std::move(elem));
    }

    ebmlMasterElement::_iterator* ebmlList::_begin() {
        return new ebmlList::_iterator(sp(), _data.begin());
    }

    ebmlMasterElement::_iterator* ebmlList::_end() {
        return new ebmlList::_iterator(sp(), _data.end());
    }

    ebmlMasterElement::_const_iterator* ebmlList::_cbegin() const {
        return new ebmlList::_const_iterator(sp(), _data.cbegin());
    }

    ebmlMasterElement::_const_iterator* ebmlList::_cend() const {
        return new ebmlList::_const_iterator(sp(), _data.cend());
    }

    ebmlList::_iterator::_iterator(const ebml::ptr<ebmlList>& elem, const std::vector<ebmlElement_sp>::iterator& iter) : _elem(elem), _iter(iter) {}
    ebmlList::_iterator::_iterator(ebml::ptr<ebmlList>&& elem, std::vector<ebmlElement_sp>::iterator&& iter) : _elem(std::move(elem)), _iter(std::move(iter)) {}

    ebmlList::_iterator::_iterator() {}
    ebmlList::_iterator::~_iterator() {}

    ebmlMasterElement::_iterator* ebmlList::_iterator::copy() const {
        return new ebmlList::_iterator(this->_elem, this->_iter);
    }

    ebmlElement& ebmlList::_iterator::operator*() const {
        return **this->_iter;
    }

    ebmlMasterElement::_iterator& ebmlList::_iterator::operator++() {
        ++(this->_iter);
        return *this;
    }

    ebmlMasterElement::_iterator& ebmlList::_iterator::operator=(const ebmlMasterElement::_iterator& other) {
        if (auto recast = dynamic_cast<const ebmlList::_iterator*>(&other)) {
            this->_elem = recast->_elem;
            this->_iter = recast->_iter;
        } else {
            throw std::runtime_error("incompatible iterators");
        }
        return *this;
    }

    bool ebmlList::_iterator::operator==(const ebmlMasterElement::_iterator& other) const {
        if (auto recast_p = dynamic_cast<const ebmlList::_iterator*>(&other)) {
            auto& recast = *recast_p;
            return (_elem == recast._elem) and (_iter == recast._iter);
        }
        return false;
    }

    bool ebmlList::_iterator::operator!=(const ebmlMasterElement::_iterator& other) const {
        if (auto recast = dynamic_cast<const ebmlList::_iterator*>(&other)) {
            return (this->_elem != recast->_elem) or (this->_iter != recast->_iter);
        }
        return true;
    }

    ebmlList::_const_iterator::_const_iterator(const ebml::ptr<const ebmlList>& elem, const std::vector<ebmlElement_sp>::const_iterator& iter)
    : _elem(elem), _iter(iter) {}
    ebmlList::_const_iterator::_const_iterator(ebml::ptr<const ebmlList>&& elem, std::vector<ebmlElement_sp>::const_iterator&& iter)
    : _elem(std::move(elem)), _iter(std::move(iter)) {}
    ebmlList::_const_iterator::_const_iterator() {}
    ebmlList::_const_iterator::~_const_iterator() {}


    ebmlMasterElement::_const_iterator* ebmlList::_const_iterator::copy() const {
        return new ebmlList::_const_iterator(this->_elem, this->_iter);
    }

    const ebmlElement& ebmlList::_const_iterator::operator*() const {
        return **this->_iter;
    }

    ebmlMasterElement::_const_iterator& ebmlList::_const_iterator::operator++() {
        ++(this->_iter);
        return *this;
    }

    ebmlMasterElement::_const_iterator& ebmlList::_const_iterator::operator=(const ebmlMasterElement::_const_iterator& other) {
        if (auto recast = dynamic_cast<const ebmlList::_const_iterator*>(&other)) {
            this->_elem = recast->_elem;
            this->_iter = recast->_iter;
        } else {
            throw std::runtime_error("incompatible iterators");
        }
        return *this;
    }

    bool ebmlList::_const_iterator::operator==(const ebmlMasterElement::_const_iterator& other) const {
        if (auto recast = dynamic_cast<const ebmlList::_const_iterator*>(&other)) {
            return (this->_elem == recast->_elem) and (this->_iter == recast->_iter);
        }
        return false;
    }

    bool ebmlList::_const_iterator::operator!=(const ebmlMasterElement::_const_iterator& other) const {
        if (auto recast = dynamic_cast<const ebmlList::_const_iterator*>(&other)) {
            return (this->_elem != recast->_elem) or (this->_iter != recast->_iter);
        }
        return true;
    }

    template class childSlot_t<ebmlList>;
    template childSlot_t<ebmlMasterElement>& childSlot_t<ebmlMasterElement>::operator=<ebmlList>(const ebmlList_sp&);
    template childSlot_t<ebmlMasterElement>& childSlot_t<ebmlMasterElement>::operator=<ebmlList>(ebmlList_sp&&);
}
#endif
