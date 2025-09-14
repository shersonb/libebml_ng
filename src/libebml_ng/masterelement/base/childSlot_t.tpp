#ifndef EBML_NG_MASTERELEMENT_CHILDSLOT_TPP
#define EBML_NG_MASTERELEMENT_CHILDSLOT_TPP

#include "childSlot_t.h"
// #include "base.h"
#include <typeinfo>

namespace ebml {
    // DEFAULT

    template<typename T>
    childSlot_t<T>::childSlot_t(ebmlMasterElement* parent, const childTypeSpec_t& spec, ebml::ptr<T>& childslot, bool allownull) {
        this->_parent = parent;
        this->_spec = &spec;
        this->_childslot = &childslot;
        this->_allownull = allownull;
        this->_raw_ptr = dynamic_cast<T*>(childslot.get());
    }

    template<typename T>
    childSlot_t<T>& childSlot_t<T>::operator=(const ebml::ptr<T>& item) {
        if ((item == nullptr) && (!this->_allownull)) {
            throw std::invalid_argument("cannot set to nullptr");
        }

        this->_parent->_attachChild(item);

        if ((*this->_childslot) != nullptr) {
            this->_parent->_detachChild(*this->_childslot);
        }

        this->_raw_ptr = item.get();
        auto recast = ebml_static_pointer_cast<ebmlElement>(item);
        (*this->_childslot) = std::move(recast);
        return *this;
    }

    template<typename T>
    childSlot_t<T>& childSlot_t<T>::operator=(ebml::ptr<T>&& item) {
        if ((item == nullptr) && (!this->_allownull)) {
            throw std::invalid_argument("cannot set to nullptr");
        }

        this->_parent->_attachChild(item);

        if ((*this->_childslot) != nullptr) {
            this->_parent->_detachChild(*this->_childslot);
        }

        this->_raw_ptr = item.get();
        auto recast = ebml_static_pointer_cast<ebmlElement>(std::move(item));
        (*this->_childslot) = std::move(recast);
        return *this;
    }

    // template<typename T>
    // childSlot_t<T>& childSlot_t<T>::operator=(const ebmlElement_sp& item) {
    //     T* raw_ptr;
    //
    //     if (item != nullptr) {
    //         raw_ptr = dynamic_cast<T*>(item.get());
    //
    //         if (raw_ptr) {
    //             throw std::bad_cast();
    //         }
    //     }
    //
    //     if ((item == nullptr) && (!this->_allownull)) {
    //         throw std::invalid_argument("cannot set to nullptr");
    //     }
    //
    //     this->_parent->_attachChild(item);
    //
    //     if ((*this->_childslot) != nullptr) {
    //         this->_parent->_detachChild(*this->_childslot);
    //     }
    //
    //     this->_raw_ptr = raw_ptr;
    //     (*this->_childslot) = item;
    //     return *this;
    // }
    //
    // template<typename T>
    // childSlot_t<T>& childSlot_t<T>::operator=(ebmlElement_sp&& item) {
    //     T* raw_ptr;
    //
    //     if (item != nullptr) {
    //         raw_ptr = dynamic_cast<T*>(item.get());
    //
    //         if (raw_ptr) {
    //             throw std::bad_cast();
    //         }
    //     }
    //
    //     if ((item == nullptr) && (!this->_allownull)) {
    //         throw std::invalid_argument("cannot set to nullptr");
    //     }
    //
    //     this->_parent->_attachChild(item);
    //
    //     if ((*this->_childslot) != nullptr) {
    //         this->_parent->_detachChild(*this->_childslot);
    //     }
    //
    //     this->_raw_ptr = raw_ptr;
    //     (*this->_childslot) = std::move(item);
    //     return *this;
    // }

    template<typename T>
    template<typename U>
    std::enable_if_t<std::is_base_of<ebmlElement, T>::value && std::is_base_of<T, U>::value && !std::is_same<T, U>::value, childSlot_t<T>&>
    childSlot_t<T>::operator=(const ebml::ptr<U>& item) {
        if ((item == nullptr) && (!this->_allownull)) {
            throw std::invalid_argument("cannot set to nullptr");
        }

        this->_parent->_attachChild(item);

        if ((*this->_childslot) != nullptr) {
            this->_parent->_detachChild(*this->_childslot);
        }

        this->_raw_ptr = item.get();
        auto recast = ebml_static_pointer_cast<ebmlElement>(item);
        (*this->_childslot) = std::move(recast);
        return *this;
    }

    template<typename T>
    template<typename U>
    std::enable_if_t<std::is_base_of<ebmlElement, T>::value && std::is_base_of<T, U>::value && !std::is_same<T, U>::value, childSlot_t<T>&>
    childSlot_t<T>::operator=(ebml::ptr<U>&& item) {
        if ((item == nullptr) && (!this->_allownull)) {
            throw std::invalid_argument("cannot set to nullptr");
        }

        this->_parent->_attachChild(item);

        if ((*this->_childslot) != nullptr) {
            this->_parent->_detachChild(*this->_childslot);
        }

        this->_raw_ptr = item.get();
        auto recast = ebml_static_pointer_cast<ebmlElement>(std::move(item));
        (*this->_childslot) = std::move(recast);
        return *this;
    }

    template<typename T>
    bool childSlot_t<T>::operator==(const std::nullptr_t&) {
        return _raw_ptr == nullptr;
    }

    template<typename T>
    bool childSlot_t<T>::operator==(const ebml::ptr<T>& item) {
        return _raw_ptr == item.get();
    }

    template<typename T>
    bool childSlot_t<T>::operator==(const childSlot_t<T>& item) {
        return this->_raw_ptr == item._raw_ptr;
    }

    template<typename T>
    childSlot_t<T>::operator ebml::ptr<T>() const {
        return ebml_dynamic_pointer_cast<T>(*this->_childslot);
    }

    template<typename T>
    childSlot_t<T>::operator T&() const {
        return **_childslot;
    }

    template<typename T>
    template<typename U>
    inline U& childSlot_t<T>::as() const {
        if (auto recast = dynamic_cast<U*>(_raw_ptr)) {
            return *recast;
        }

        throw std::bad_cast();
    }

    template<typename T>
    T* childSlot_t<T>::operator->() const {
        return this->_raw_ptr;
    }
}
#endif
