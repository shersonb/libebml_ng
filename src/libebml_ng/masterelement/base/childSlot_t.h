#ifndef EBML_NG_MASTERELEMENT_CHILDSLOT_H
#define EBML_NG_MASTERELEMENT_CHILDSLOT_H

#include "../../forwards.h"
#include "../../ptrs.h"
#include "../../struct/datetime.h"
#include "../../ebmlDataElement.h"

namespace ebml {
    class childTypeSpec_t;

    /**
     * @brief A templated helper class managing a child slot within an EBML master element.
     *
     * This class provides methods to assign, compare, and access a child element
     * using smart pointers. It ensures that the child being attached is valid
     * according to the parent's type specification and updates parent pointers.
     *
     * @tparam T The type of the EBML element stored in the slot. Defaults to ebmlElement.
     */
    template<typename T=ebmlElement>
    class childSlot_t {
    private:
        ebmlMasterElement* _parent;
        const childTypeSpec_t * _spec;
        // ebmlElement_sp* _childslot;
        ebml::ptr<T>* _childslot;
        T* _raw_ptr = nullptr;
        bool _allownull = true;

    public:
        childSlot_t(ebmlMasterElement* parent, const childTypeSpec_t &, ebml::ptr<T>& childslot, bool allownull=true);
        childSlot_t<T>& operator=(const ebml::ptr<T>& item);
        childSlot_t<T>& operator=(ebml::ptr<T>&& item);

        template<typename U>
        std::enable_if_t<std::is_base_of<ebmlElement, T>::value && std::is_base_of<T, U>::value && !std::is_same<T, U>::value, childSlot_t<T>&>
        operator=(const ebml::ptr<U>& item);

        template<typename U>
        std::enable_if_t<std::is_base_of<ebmlElement, T>::value && std::is_base_of<T, U>::value && !std::is_same<T, U>::value, childSlot_t<T>&>
        operator=(ebml::ptr<U>&& item);

        bool operator==(const std::nullptr_t&);
        bool operator==(const ebml::ptr<T>& item);
        bool operator==(const ebml::ptr<const T>& item);

        template<typename U>
        std::enable_if_t<std::is_base_of<ebmlElement, T>::value && std::is_base_of<T, U>::value && !std::is_same<T, U>::value, bool&>
        operator==(const ebml::ptr<U>& item);

        bool operator==(const childSlot_t<T>& item);
        operator ebml::ptr<T>() const;
        T* operator->() const;
        const ebml::ptr<T>& get() const;
        operator T&() const;

        template<typename U>
        inline U& as() const;
    };

    template<>
    class childSlot_t<ebmlElement> {
    private:
        ebmlMasterElement* _parent;
        const childTypeSpec_t * _spec;
        ebmlElement_sp* _childslot;
        bool _allownull = true;
        ebmlElement* _raw_ptr;

    public:
        childSlot_t(ebmlMasterElement* parent, const childTypeSpec_t &, ebmlElement_sp& childslot, bool allownull=true);
        childSlot_t<ebmlElement>& operator=(const ebmlElement_sp& item);
        childSlot_t<ebmlElement>& operator=(ebmlElement* item);
        childSlot_t<ebmlElement>& operator=(ebmlElement_sp&& item);

        bool operator==(const ebmlElement_sp& item);
        bool operator==(const c_ebmlElement_sp& item);
        bool operator==(const childSlot_t<ebmlElement>& item);
        operator ebmlElement&() const;

        // Mirror methods
        const ebmlElementType & cls() const;
        ebmlID_t ebmlID() const;

        template<typename T>
        inline T& as();

        ebml::ptr<ebmlElement> sp();
        ebml::ptr<const ebmlElement> sp() const;

        template<typename T>
        inline ebml::ptr<T> sp();

        template<typename T>
        inline ebml::ptr<T> sp() const;

        ebml::wptr<ebmlElement> wp();
        ebml::wptr<const ebmlElement> wp() const;

        template<typename T>
        inline ebml::wptr<T> wp();

        template<typename T>
        inline ebml::wptr<T> wp() const;

        bool parent_is_const() const; // Member function that specifies if pointer is to a const parent or non-const parent.
        ebml::ptr<ebmlMasterElement> parent() const; // Returns pointer to non-const parent. Throws ebmlException if parent_is_const() returns true.
        ebml::ptr<const ebmlMasterElement> c_parent() const; // Returns pointer to const parent.
        ebmlElement_sp root() const;
        c_ebmlElement_sp c_root() const;
        bool hasParent() const;
        unsigned char headSize() const;
        unsigned char headSize(size_t) const; // May be deprecated
        unsigned char sizeWidth() const;
        unsigned char sizeWidth(size_t) const; // May be deprecated
        size_t outerSize() const;
        size_t outerSize(size_t) const; // May be deprecated
        size_t dataSize() const;
        off_t offsetInParent() const;
        off_t offsetInFile() const;
        off_t dataOffsetInParent() const;
        off_t dataOffsetInFile() const;
        std::string encode() const;
        size_t encode(char*) const;
        size_t encode(const ioBase_sp&) const;
        size_t encode(const ioBase_sp&, off_t) const;
        size_t encode(ioBase*) const;
        size_t encode(ioBase*, off_t) const;
        size_t encode(char*, size_t) const;
        ebml::ptr<ebmlElement> clone() const;
        std::wstring minirepr() const;
        std::wstring repr() const;
    };

    // template<typename T>
    // T& data(const childSlot_t<ebmlElement>& elem);

    extern template class childSlot_t<ebmlElement>;
    extern template class childSlot_t<ebmlMasterElement>;
    extern template class childSlot_t<ebmlDataElement<unsigned long long>>;
    extern template class childSlot_t<ebmlDataElement<long long>>;
    extern template class childSlot_t<ebmlDataElement<double>>;
    extern template class childSlot_t<ebmlDataElement<std::string>>;
    extern template class childSlot_t<ebmlDataElement<std::wstring>>;
    extern template class childSlot_t<ebmlDataElement<timepoint_t>>;
}
#endif
