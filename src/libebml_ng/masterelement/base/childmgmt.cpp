#ifndef EBML_NG_MASTERELEMENT_CHILD_MGMT_CPP
#define EBML_NG_MASTERELEMENT_CHILD_MGMT_CPP

#include "libebml_ng/struct/unicode.h"
#include "libebml_ng/masterelement/base.h"
#include "libebml_ng/struct.tpp"
#include "libebml_ng/exceptions.h"

namespace ebml {
    childClassSpecArg_t::childClassSpecArg_t() {
        this->cls = nullptr;
        this->min = 0;
        this->max = -1;
    }

    childClassSpecArg_t::childClassSpecArg_t(const ebmlElementClass* cls, unsigned long min, long max) {
        this->cls = cls;
        this->min = min;
        this->max = max;
    }

    childClassSpec_t::childClassSpec_t() {}

    childClassSpec_t::childClassSpec_t(const childClassSpecArg_init_l& items) {
        for (const auto& item : items) {
            this->add(item);
        }
    }

    childClassSpec_t::childClassSpec_t(const childClassSpecArg_l& items) {
        for (const auto& item : items) {
            this->add(item);
        }
    }

    childClassSpec_t::childClassSpec_t(const childClassSpec_t& other) :
        _cls_by_ebmlID(other._cls_by_ebmlID),
        _clss(other._clss),
        _occurspec(other._occurspec) {}

    void childClassSpec_t::add(ebml::childClassSpecArg_t const& spec) {
        if (this->_cls_by_ebmlID.count(spec.cls->ebmlID)) {
            throw ebmlException("Cannot specify multiple ebml::ebmlElementClass* instances managing the same ebmlID.");
        }

        this->_cls_by_ebmlID[spec.cls->ebmlID] = spec.cls;
        this->_clss.insert(spec.cls);
        this->_occurspec[spec.cls->ebmlID] = {spec.min, spec.max};
    }

    const occurSpec_d& childClassSpec_t::occurSpec() const {
        return this->_occurspec;
    }

    const occurSpec_t& childClassSpec_t::occurSpec(ebmlID_t ebmlID) const {
        return this->_occurspec.at(ebmlID);
    }

    const occurSpec_t& childClassSpec_t::occurSpec(const ebmlElementClass* cls) const {
        auto ebmlID = cls->ebmlID;
        return this->_occurspec.at(ebmlID);
    }

    bool childClassSpec_t::isValid(const ebmlElement_sp& elem) const {
        return this->_clss.count(elem->cls()) != 0;
    }

    size_t childClassSpec_t::count(ebmlID_t ebmlID) const {
        return this->_cls_by_ebmlID.count(ebmlID);
    }

    size_t childClassSpec_t::size() const {
        return this->_cls_by_ebmlID.size();
    }

    const ebmlElementClass* childClassSpec_t::operator[](ebmlID_t ebmlID) const {
        if (this->_cls_by_ebmlID.count(ebmlID) == 0) {
            std::string errmsg = "No child element type matches EBML ID ";
            errmsg += std::to_string(ebmlID);
            throw ebmlNoChildMatch(errmsg, DECODE_ERR_DEFAULT, 0);
        }
        return this->_cls_by_ebmlID.at(ebmlID);
    }

    ebmlElementClass_d::const_iterator childClassSpec_t::begin() const {
        return this->_cls_by_ebmlID.cbegin();
    }

    ebmlElementClass_d::const_iterator childClassSpec_t::end() const {
        return this->_cls_by_ebmlID.cend();
    }

    const childClassSpec_t& ebmlMasterElementClass::childClasses() const {
        return this->_childClasses;
    }

    void ebmlMasterElement::_attachChild(const ebmlElement_sp& child, bool weak) {
        if (child.get() == this) {
            throw ebmlException("cannot attach element to itself");
        } else if (this->cls()->childClasses().count(child->ebmlID()) == 0) {
            std::string errmsg = "cannot add '";
            errmsg += pack(child->cls()->name);
            errmsg += "' object to parent";
            // throw ebmlException(pack<std::wstring>(errmsg));
            throw ebmlException(errmsg);
        }

        ebmlElement::_attachChild(child, weak);
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

                ebmlElement::_attachChild(elem, weak);
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

    void ebmlMasterElement::_detachChildren(const ebmlElement_l& elems) {
        auto start = elems.cbegin();
        auto end = elems.cend();
        auto iter = start;

        while (iter != end) {
            this->_detachChild(*iter);
        }
    }

    childSlot_t::childSlot_t(ebmlMasterElement* parent, const childClassSpec_t& spec, ebmlElement_sp& childslot, bool allownull) {
        this->_parent = parent;
        this->_spec = &spec;
        this->_childslot = &childslot;
        this->_allownull = allownull;
    }

    childSlot_t& childSlot_t::operator=(const ebmlElement_sp& item) {
        if ((item == nullptr) && (!this->_allownull)) {
            throw std::invalid_argument("cannot set to nullptr");
        }

        this->_parent->_attachChild(item);

        if ((*this->_childslot) != nullptr) {
            this->_parent->_detachChild(*this->_childslot);
        }
        (*this->_childslot) = item;
        return *this;
    }

    childSlot_t& childSlot_t::operator=(ebmlElement* item) {
        return this->operator=(std::move(item->shared_from_this()));
    }

    childSlot_t& childSlot_t::operator=(ebmlElement_sp&& item) {
        if ((item == nullptr) && (!this->_allownull)) {
            throw std::invalid_argument("cannot set to nullptr");
        }

        this->_parent->_attachChild(item);

        if ((*this->_childslot) != nullptr) {
            this->_parent->_detachChild(*this->_childslot);
        }
        (*this->_childslot) = std::move(item);
        return *this;
    }

    bool childSlot_t::operator==(const ebmlElement_sp& item) {
        return this->_childslot->get() == item.get();
    }

    bool childSlot_t::operator==(const c_ebmlElement_sp& item) {
        return this->_childslot->get() == item.get();
    }

    bool childSlot_t::operator==(const childSlot_t& item) {
        return this->_childslot->get() == item._childslot->get();
    }

    childSlot_t::operator ebmlElement_sp() const {
        return *this->_childslot;
    }

    childSlot_t::operator const ebmlElement_sp&() const {
        return *this->_childslot;
    }

    ebmlElement* childSlot_t::operator->() const {
        return this->_childslot->get();
    }

    ebmlElement_sp childSlot_t::get() const {
        return *this->_childslot;
    }

    c_ebmlElement_l::c_ebmlElement_l(const ebmlElement_l& items) : _items(&items) {}
    c_ebmlElement_l::c_ebmlElement_l(const c_ebmlElement_l& other) : _items(other._items) {}

#if RAW
    const ebmlElement* c_ebmlElement_l::front() const {
        return this->_items->front().get();
    }
    const ebmlElement* c_ebmlElement_l::back() const {
        return this->_items->back().get();
    }
    const ebmlElement* c_ebmlElement_l::at(size_t offset) const {
        return this->_items->at(offset).get();
    }
    const ebmlElement* c_ebmlElement_l::operator[](size_t offset) const {
        return (*this->_items)[offset].get();
    }
#else
    c_ebmlElement_sp c_ebmlElement_l::front() const {
        return std::const_pointer_cast<const ebmlElement>(this->_items->front());
    }
    c_ebmlElement_sp c_ebmlElement_l::back() const {
        return std::const_pointer_cast<const ebmlElement>(this->_items->back());
    }
    c_ebmlElement_sp c_ebmlElement_l::at(size_t offset) const {
        return std::const_pointer_cast<const ebmlElement>(this->_items->at(offset));
    }
    c_ebmlElement_sp c_ebmlElement_l::operator[](size_t offset) const {
        return std::const_pointer_cast<const ebmlElement>((*this->_items)[offset]);
    }
#endif
    size_t c_ebmlElement_l::size() const {
        return this->_items->size();
    }
    bool c_ebmlElement_l::empty() const {
        return this->_items->empty();
    }
    // ebmlElement_l::const_iterator c_ebmlElement_l::cbegin();
    // ebmlElement_l::const_iterator c_ebmlElement_l::cend();
}

#endif
