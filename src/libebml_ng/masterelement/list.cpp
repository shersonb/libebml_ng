#ifndef EBML_NG_MASTERELEMENT_LIST_CPP
#define EBML_NG_MASTERELEMENT_LIST_CPP
#include "libebml_ng/masterelement/list.h"
#include <mutex>

namespace ebml {
    ebmlListClass::ebmlListClass(ebmlID_t ebmlID, std::wstring name, const occurSpec_t& recursive) : ebmlMasterElementClass(ebmlID, name) {
        if (recursive.max != 0) {
            this->_childClasses.add({this, recursive.min, recursive.max});
        }
    }

    ebmlListClass::ebmlListClass(ebmlID_t ebmlID, std::wstring name, const childClassSpecArg_l& childSpec, const occurSpec_t& recursive) : ebmlListClass(ebmlID, name) {
        this->_childClasses = childSpec;

        if (recursive.max != 0) {
            this->_childClasses.add({this, recursive.min, recursive.max});
        }
    }

    // ebmlElement_sp ebmlListClass::operator()() const {
    //     auto elem = new ebmlList(this);
    //     return std::shared_ptr<ebmlElement>(elem);
    // }

    ebmlElement* ebmlListClass::_new() const {
        return new ebmlList(this);
    }

    ebmlElement_sp ebmlListClass::operator()(const ebmlElement_l& items) const {
        auto elem = new ebmlList(this, items);
        return std::shared_ptr<ebmlElement>(elem);
    }

    ebmlElement_sp ebmlListClass::operator()(ebmlElement_l&& items) const {
        auto elem = new ebmlList(this, std::move(items));
        return std::shared_ptr<ebmlElement>(elem);
    }



    ebmlList::ebmlList(const ebmlListClass* cls) : ebmlMasterElement(cls) {}

    ebmlList::ebmlList(const ebmlListClass* cls, const ebmlElement_l& items) : ebmlMasterElement(cls) {
        this->setData(items);
    }

    ebmlList::ebmlList(const ebmlListClass* cls, ebmlElement_l&& items) : ebmlMasterElement(cls) {
        this->setData(std::move(items));
    }

    void ebmlList::setData(const ebmlElement_l& items) {
        auto start = items.cbegin();
        auto iter = start;
        auto end = items.cend();

        while (iter != end) {
            try {
                this->_attachChild(*iter);
                ++iter;
            } catch (...) {
                while (iter != start) {
                    --iter;
                    this->_detachChild(*iter);
                }
                throw;
            }
        }

        this->_clear();
        this->_data.reserve(items.size());

        for (const auto& item : items) {
            this->_data.push_back(item);
        }
    }

    void ebmlList::setData(ebmlElement_l&& items) {
        auto start = items.begin();
        auto iter = start;
        auto end = items.end();

        while (iter != end) {
            try {
                this->_attachChild(*iter);
                ++iter;
            } catch (...) {
                while (iter != start) {
                    --iter;
                    this->_detachChild(*iter);
                }
                throw;
            }
        }

        this->_clear();
        this->_data.reserve(items.size());

        for (auto& item : items) {
            this->_data.push_back(std::move(item));
        }

        items.clear();
    }

    std::wstring ebmlList::minirepr() const {
        std::wstring ret = L"[";

        if (this->_data.size()) {
            auto iter = this->_data.cbegin();
            auto end = this->_data.cend();

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
        return this->_data.size();
    }

    void ebmlList::_clear() {
        for (const auto& elem : this->_data) {
            this->_detachChild(elem);
        }

        this->_data.resize(0);
    }

    // const ebmlElement_sp& ebmlList::operator[](off_t offset) {
    //     return this->_data[offset];
    // }

    childSlot_t ebmlList::operator[](off_t offset) {
        return {this, this->cls()->childClasses(), this->_data[offset]};
    }

    childSlot_t ebmlList::at(off_t offset) {
        if ((offset < 0) or (offset >= this->_data.size())) {
            throw std::out_of_range("list index out of range");
        }

        return {this, this->cls()->childClasses(), this->_data.at(offset)};
    }

#if RAW
    const ebmlElement* ebmlList::operator[](off_t offset) const {
        return this->_data.at(offset).get();
    }

    const ebmlElement* ebmlList::at(off_t offset) const {
        return this->_data.at(offset).get();
    }

    const ebmlElement* ebmlList::front() const {
        return this->_data.front().get();
    }

    const ebmlElement* ebmlList::back() const {
        return this->_data.back().get();
    }
#else
    c_ebmlElement_sp ebmlList::operator[](off_t offset) const {
        return std::const_pointer_cast<const ebmlElement>(this->_data.at(offset));
    }

    c_ebmlElement_sp ebmlList::at(off_t offset) const {
        return std::const_pointer_cast<const ebmlElement>(this->_data.at(offset));
    }

    c_ebmlElement_sp ebmlList::front() const {
        return std::const_pointer_cast<const ebmlElement>(this->_data.front());
    }

    c_ebmlElement_sp ebmlList::back() const {
        return std::const_pointer_cast<const ebmlElement>(this->_data.back());
    }
#endif

    childSlot_t ebmlList::front() {
        return {this, this->cls()->childClasses(), this->_data.front()};
    }

    childSlot_t ebmlList::back() {
        return {this, this->cls()->childClasses(), this->_data.back()};
    }
    void ebmlList::insert(off_t offset, const ebmlElement_sp& elem) {
        if ((offset < 0) or (offset > this->_data.size())) {
            throw std::out_of_range("list insertion index out of range");
        }
        this->_attachChild(elem);
        this->_data.insert(this->_data.begin() + offset, elem);
    }

    void ebmlList::erase(off_t offset) {
        if ((0 <= offset) and (offset < this->_data.size())) {
            this->_detachChild(this->_data.at(offset));
        } else {
            throw std::out_of_range("list assignment index out of range");
        }
        this->_data.erase(this->_data.begin() + offset);
    }

    void ebmlList::push_back(const ebmlElement_sp& elem) {
        this->_attachChild(elem);
        this->_data.push_back(elem);
    }

    void ebmlList::push_back(ebmlElement_sp&& elem) {
        this->_attachChild(elem);
        this->_data.push_back(std::move(elem));
    }

    void ebmlList::_addChild(const ebmlElement_sp& elem) {
        this->_data.push_back(elem);
    }

    void ebmlList::_addChild(ebmlElement_sp&& elem) {
        this->_data.push_back(std::move(elem));
    }

    ebmlMasterElement::_iterator* ebmlList::_begin() {
        return new ebmlList::_iterator(this->shared_from_this(), this->_data.begin());
    }

    ebmlMasterElement::_iterator* ebmlList::_end() {
        return new ebmlList::_iterator(this->shared_from_this(), this->_data.end());
    }

    ebmlMasterElement::_const_iterator* ebmlList::_cbegin() const {
        return new ebmlList::_const_iterator(this->shared_from_this(), this->_data.cbegin());
    }

    ebmlMasterElement::_const_iterator* ebmlList::_cend() const {
        return new ebmlList::_const_iterator(this->shared_from_this(), this->_data.cend());
    }

    ebmlList::_iterator::_iterator(const ebmlElement_sp& elem, const std::vector<ebmlElement_sp>::iterator& iter) : _elem(elem), _iter(iter) {}

    ebmlList::_iterator::_iterator() {}
    ebmlList::_iterator::~_iterator() {}

    ebmlMasterElement::_iterator* ebmlList::_iterator::copy() {
        return new ebmlList::_iterator(this->_elem, this->_iter);
    }

    const ebmlElement_sp& ebmlList::_iterator::operator*() const {
        return *(this->_iter);
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
        if (auto recast = dynamic_cast<const ebmlList::_iterator*>(&other)) {
            return (this->_elem == recast->_elem) and (this->_iter == recast->_iter);
        }
        return false;
    }

    bool ebmlList::_iterator::operator!=(const ebmlMasterElement::_iterator& other) const {
        if (auto recast = dynamic_cast<const ebmlList::_iterator*>(&other)) {
            return (this->_elem != recast->_elem) or (this->_iter != recast->_iter);
        }
        return true;
    }

    ebmlList::_const_iterator::_const_iterator(const c_ebmlElement_sp& elem, const std::vector<ebmlElement_sp>::const_iterator& iter) : _elem(elem), _iter(iter) {}
    ebmlList::_const_iterator::_const_iterator() {}
    ebmlList::_const_iterator::~_const_iterator() {}


    ebmlMasterElement::_const_iterator* ebmlList::_const_iterator::copy() {
        return new ebmlList::_const_iterator(this->_elem, this->_iter);
    }

#if RAW
    const ebmlElement* ebmlList::_const_iterator::operator*() const {
        return (*(this->_iter)).get();
    }
#else
    c_ebmlElement_sp ebmlList::_const_iterator::operator*() const {
        return std::const_pointer_cast<const ebmlElement>(*(this->_iter));
    }
#endif

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
}
// struct ebmlMasterElementListPayload {
//     ebmlElement_l* items;
//     ebmlMasterElementListPayload() {
//         this->items = new ebmlElement_l();
//     };
//     ~ebmlMasterElementListPayload() {
//         delete this->items;
//     }
// };
//
// struct ebmlMasterElementListIterator {
//     ebmlElement_i iterator;
//     ebmlMasterElementListIterator(ebmlElement_i _iterator) {
//         this->iterator = _iterator;
//     };
// };
//
// ebmlMasterElementListClass::ebmlMasterElementListClass(unsigned long long _ebmlID, std::string _name) :
//         ebmlMasterElementClass(_ebmlID, _name) {};
//
// ebmlMasterElementListClass::~ebmlMasterElementListClass() {};
//
// void ebmlMasterElementListClass::_create(ebmlElement* elem) const {
//     elem->data.as_ptr = new ebmlMasterElementListPayload();
// }
//
// void ebmlMasterElementListClass::_destroy(ebmlElement* elem) const {
//     delete (ebmlMasterElementListPayload*)elem->data.as_ptr;
// }
//
// void ebmlMasterElementListClass::_init(ebmlElement* elem, const void* initdata) const {
// }
//
// void ebmlMasterElementListClass::_append(ebmlElement* elem, ebmlElement* child) const {
//     auto l = (ebmlMasterElementListPayload*)elem->data.as_ptr;
//     l->items->push_back(child);
// }
//
// void* ebmlMasterElementListClass::_init_begin_iter(void* data) const {
//     auto recast = (ebmlMasterElementListPayload*)data;
//     return new ebmlMasterElementListIterator(recast->items->begin());
// }
//
// void* ebmlMasterElementListClass::_init_end_iter(void* data) const {
//     auto recast = (ebmlMasterElementListPayload*)data;
//     return new ebmlMasterElementListIterator(recast->items->end());
// }
//
// void* ebmlMasterElementListClass::_copy_iter(const void* data) const {
//     auto recast = (ebmlMasterElementListIterator*)data;
//     return new ebmlMasterElementListIterator(recast->iterator);
// }
//
// void ebmlMasterElementListClass::_copy_iter(const void* src, void* dest) const {
//     auto recast_src = static_cast<const ebmlMasterElementListIterator*>(src);
//     auto recast_dest = (ebmlMasterElementListIterator*)dest;
//     *recast_dest = *recast_src;
// }
//
// void ebmlMasterElementListClass::_destroy_iter(void* data) const {
// //     printf("begin _destroy_iter(void*) %llu\n", (unsigned long long)data);
//     delete (ebmlMasterElementListIterator*)data;
// //     printf("end _destroy_iter(void*) %llu\n", (unsigned long long)data);
// }
//
// bool ebmlMasterElementListClass::_iseq_iter(const void* lhs, const void* rhs) const {
//     auto recast_lhs = static_cast<const ebmlMasterElementListIterator*>(lhs);
//     auto recast_rhs = static_cast<const ebmlMasterElementListIterator*>(rhs);
//     return recast_lhs->iterator == recast_rhs->iterator;
// };
//
// ebmlElement* ebmlMasterElementListClass::_dereference_iter(void* data) const {
//     auto recast = (ebmlMasterElementListIterator*)data;
//     return *(recast->iterator);
// };
//
// void ebmlMasterElementListClass::_increment_iter(void* data) const {
//     auto recast = (ebmlMasterElementListIterator*)data;
//     ++(recast->iterator);
// };
//
#endif
