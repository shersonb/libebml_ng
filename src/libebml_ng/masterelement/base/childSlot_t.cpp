#ifndef EBML_NG_MASTERELEMENT_CHILDSLOT_CPP
#define EBML_NG_MASTERELEMENT_CHILDSLOT_CPP

#include "libebml_ng/masterelement/base/childSlot_t.tpp"

namespace ebml {
    childSlot_t<ebmlElement>::childSlot_t(ebmlMasterElement* parent, const childClassSpec_t& spec, ebmlElement_sp& childslot, bool allownull) {
        this->_parent = parent;
        this->_spec = &spec;
        this->_childslot = &childslot;
        this->_allownull = allownull;
        this->_raw_ptr = childslot.get();
    }

    childSlot_t<ebmlElement>& childSlot_t<ebmlElement>::operator=(const ebmlElement_sp& item) {
        if ((item == nullptr) && (!this->_allownull)) {
            throw std::invalid_argument("cannot set to nullptr");
        }

        this->_parent->_attachChild(item);

        if ((*this->_childslot) != nullptr) {
            this->_parent->_detachChild(*this->_childslot);
        }

        this->_raw_ptr = item.get();
        (*this->_childslot) = item;
        return *this;
    }

    childSlot_t<ebmlElement>& childSlot_t<ebmlElement>::operator=(ebmlElement_sp&& item) {
        if ((item == nullptr) && (!this->_allownull)) {
            throw std::invalid_argument("cannot set to nullptr");
        }

        this->_parent->_attachChild(item);

        if ((*this->_childslot) != nullptr) {
            this->_parent->_detachChild(*this->_childslot);
        }

        this->_raw_ptr = item.get();
        (*this->_childslot) = std::move(item);
        return *this;
    }

    bool childSlot_t<ebmlElement>::operator==(const ebmlElement_sp& item) {
        return this->_raw_ptr == item.get();
    }

    bool childSlot_t<ebmlElement>::operator==(const c_ebmlElement_sp& item) {
        return this->_raw_ptr == item.get();
    }

    bool childSlot_t<ebmlElement>::operator==(const childSlot_t& item) {
        return this->_raw_ptr == item._raw_ptr;
    }

    // childSlot_t<ebmlElement>::operator ebmlElement_sp() const {
    //     return *this->_childslot;
    // }

    childSlot_t<ebmlElement>::operator ebmlElement&() const {
        return **this->_childslot;
    }

    // childSlot_t<ebmlElement>::operator const ebmlElement_sp&() const {
    //     return *this->_childslot;
    // }

    // ebmlElement* childSlot_t<ebmlElement>::operator->() const {
    //     return this->_raw_ptr;
    // }



    template class childSlot_t<ebmlElement>;
    template class childSlot_t<ebmlMasterElement>;
    template class childSlot_t<ebmlDataElement<unsigned long long>>;
    template class childSlot_t<ebmlDataElement<long long>>;
    template class childSlot_t<ebmlDataElement<double>>;
    template class childSlot_t<ebmlDataElement<std::string>>;
    template class childSlot_t<ebmlDataElement<std::wstring>>;
    template class childSlot_t<ebmlDataElement<timepoint_t>>;
}
#endif
