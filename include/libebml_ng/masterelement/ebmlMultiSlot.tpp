#ifndef EBML_NG_MASTERELEMENT_MULTSLOT_TPP
#define EBML_NG_MASTERELEMENT_MULTSLOT_TPP

#include "ebmlMultiSlot.h"
// #include "../element.tpp"
#include "../exceptions.h"
#include "../ebmlDataElement.h"
#include "../struct/unicode.h"

namespace ebml {
    // template<typename T>
    // T data(const const_slot_t& slot) {
    //     c_ebmlElement_sp elem = slot;
    //     return data<T>(elem);
    // }

    // Provides a public view of _slot_t instance
    template<typename T>
    childSlot_t<T> slot_t::at(size_t offset) {
        ebmlElement_sp& elem = this->_slot->at(offset);
        return {this->_parent.get(), this->_slot->slotSpec().childClasses(), elem, false};
    }

    template<typename T>
    childSlot_t<T> slot_t::front() {
        ebmlElement_sp& elem = this->_slot->front();
        return {this->_parent.get(), this->_slot->slotSpec().childClasses(), elem, false};
    }

    template<typename T>
    childSlot_t<T> slot_t::back() {
        ebmlElement_sp& elem = this->_slot->back();
        return {this->_parent.get(), this->_slot->slotSpec().childClasses(), elem, false};
    }

    template<typename T>
    T& slot_t::data(size_t) {
        ebmlElement_sp& elem = this->_slot->back();
        return ebml::data<T>(elem);
    }

    template<typename T>
    const T& slot_t::data(size_t) const {
        ebmlElement_sp& elem = this->_slot->back();
        return ebml::data<const T>(elem);
    }

    // template<typename T>
    // T& data(slot_t& slot) {
    //     const ebmlElement_sp& elem = slot;
    //     return data<T>(elem);
    // }


    // class ebmlMultiSlot : public ebmlMasterElement {
    template<typename T>
    T& ebmlMultiSlot::data(size_t offset) {
        if (offset >= this->_slots.size()) {
            std::string errmsg = "slot index '";
            errmsg += std::to_string(offset);
            errmsg += "' out of range";
            throw ebmlException(errmsg);
        }

        _slot_t& slot = this->_slots[offset];
        const ebmlElement_sp& elem = slot;

        if (elem == nullptr) {
            auto clss = slot.slotSpec().childClasses();

            if (clss.size() > 1) {
                throw ebmlException("ambiguous slot element type");
            }

            auto cls = (*clss.begin()).second;
            auto dcls = dynamic_cast<const ebmlDataElementClass<T>*>(cls);

            if (dcls == nullptr) {
                throw std::bad_cast();
            }

            slot = (*dcls)();
            this->_attachChild(elem);
            // slot = elem;
        }

        return ebml::data<T>(elem);
    }

    template<typename T>
    T& ebmlMultiSlot::data(const std::string& key) {
        if (this->_slots_by_name.count(key) == 0) {
            std::string errmsg = "key '";
            errmsg += key;
            errmsg += "' not found";
            throw ebmlException(errmsg);
        }

        _slot_t& slot = *this->_slots_by_name[key];
        const ebmlElement_sp& elem = slot;


        if (elem == nullptr) {
            auto clss = slot.slotSpec().childClasses();

            if (clss.size() > 1) {
                throw ebmlException("ambiguous slot element type");
            }

            auto cls = (*clss.begin()).second;
            auto dcls = dynamic_cast<const ebmlDataElementClass<T>*>(cls);

            if (dcls == nullptr) {
                throw std::bad_cast();
            }


            ebmlElement_sp newelem = (*dcls)();
            this->_attachChild(newelem);
            slot = newelem;
            return ebml::data<T>(newelem);
        }

        return ebml::data<T>(elem);
    }

    template<typename T>
    const T& ebmlMultiSlot::data(size_t offset) const {
        if (offset >= this->_slots.size()) {
            std::string errmsg = "slot index '";
            errmsg += std::to_string(offset);
            errmsg += "' out of range";
            throw ebmlException(errmsg);
        }

        const _slot_t& slot = this->_slots[offset];
        const ebmlElement_sp& elem = slot;

        if (elem == nullptr) {
            throw ebmlException("slot is empty");
        }

        return ebml::data<T>(elem);
    }

    template<typename T>
    const T& ebmlMultiSlot::data(const std::string& key) const {
        if (this->_slots_by_name.count(key) == 0) {
            std::string errmsg = "key '";
            errmsg += key;
            errmsg += "' not found";
            throw ebmlException(errmsg);
        }

        const _slot_t& slot = *this->_slots_by_name.at(key);
        const ebmlElement_sp& elem = slot;


        if (elem == nullptr) {
            throw ebmlException("slot is empty");
        }

        return ebml::data<T>(elem);
    }
}
#endif
