#ifndef EBML_NG_MASTERELEMENT_LIST_H
#define EBML_NG_MASTERELEMENT_LIST_H

// #include "libebml_ng/base.h"
#include <unordered_map>
#include <vector>

// #include "base.h"
#include "base/ebmlMasterElement.h"
#include "base/ebmlMasterElementType.h"
#include "base/childSlot_t.h"

namespace ebml {
    /**
     * @ingroup type_definitions
     * @ingroup type_implementations
     * @ingroup master_element_type_definitions
     * @ingroup master_element_type_implementations
     * @class ebmlListType
     * @brief Represents a list-like EBML master element.
     *
     * This class defines an EBML element that contains a list of children, allowing for
     * multiple occurrences of a single child type. It inherits from `ebmlMasterElementType`.
     */
    class ebmlListType : public ebmlTypeCRTP<ebmlListType, ebmlList, ebmlMasterElementType> {
    public:
        ebmlListType(const char*, const std::wstring&, const occurSpec_t& recursive={0, 0});
        ebmlListType(const char*, const std::wstring&, const childTypeSpecArg_l &, const occurSpec_t& recursive={0, 0});
        ebmlListType(const char*, const std::wstring&, childClassSpecArg_init_l, const occurSpec_t& recursive={0, 0});

        ebmlListType(ebmlID_t, const std::wstring&, const occurSpec_t& recursive={0, 0});
        ebmlListType(ebmlID_t, const std::wstring&, const childTypeSpecArg_l &, const occurSpec_t& recursive={0, 0});
        ebmlListType(ebmlID_t, const std::wstring&, childClassSpecArg_init_l, const occurSpec_t& recursive={0, 0});
        friend class ebmlList;
    };

    class ebmlList : public ebmlElementCRTP<ebmlListType, ebmlList, ebmlMasterElement> {
        friend class ebmlTypeCRTP<ebmlListType, ebmlList, ebmlMasterElementType>;
        friend class ebmlElementCRTP<ebmlListType, ebmlList, ebmlMasterElement>;
    public:
        typedef ebmlListType class_t;
    private:
        std::vector<ebmlElement_sp> _data;

    protected:
        ebmlList(const ebmlListType *);
        ebmlList(const ebmlListType *, const ebmlElement_l&);
        ebmlList(const ebmlListType *, ebmlElement_l&&);
        ebmlList(const ebmlList&);

        void _clear() override;
        void _validateData(const ebmlElement_l&);
        void _setData(const ebmlElement_l&);
        void _setData(ebmlElement_l&&);

    public:
        void setData(const ebmlElement_l&);
        void setData(ebmlElement_l&&);
        std::wstring minirepr() const override;

        // Element access
        childSlot_t<> operator[](off_t);

        template<typename T=ebmlElement>
        childSlot_t<T> at(off_t);

        template<typename T=ebmlElement>
        childSlot_t<T> front();

        template<typename T=ebmlElement>
        childSlot_t<T> back();

        // Const element access
        const ebmlElement& operator[](off_t) const;
        const ebmlElement& at(off_t) const;
        const ebmlElement& front() const;
        const ebmlElement& back() const;

        // Element search
        bool contains(const c_ebmlElement_sp&) const;
        off_t index(const c_ebmlElement_sp&) const;

        // Element modification
        void push_back(const ebmlElement_sp&);
        void push_back(ebmlElement_sp&&);
        void pop_back();
        void insert(off_t, const ebmlElement_sp&);
        void insert(off_t, ebmlElement_sp&&);
        void erase(off_t);
        void clear();

        // Container size
        size_t size() const;

        // Iteration
    protected:
        class _iterator : public ebmlMasterElement::_iterator {
        private:
            ebml::ptr<ebmlList> _elem;
            std::vector<ebmlElement_sp>::iterator _iter;

        protected:
            _iterator(const ebml::ptr<ebmlList>& elem, const std::vector<ebmlElement_sp>::iterator& iter);
            _iterator(ebml::ptr<ebmlList>&& elem, std::vector<ebmlElement_sp>::iterator&& iter);

        public:
            _iterator();
            virtual ~_iterator();

            ebmlMasterElement::_iterator* copy() const;

            ebmlElement& operator*() const;
            ebmlMasterElement::_iterator& operator++();
            ebmlMasterElement::_iterator& operator=(const ebmlMasterElement::_iterator&);
            bool operator==(const ebmlMasterElement::_iterator&) const;
            bool operator!=(const ebmlMasterElement::_iterator&) const;
            friend class ebmlList;
            friend class ebmlMasterElement::iterator;
        };

        ebmlMasterElement::_iterator* _begin() override;
        ebmlMasterElement::_iterator* _end() override;


        class _const_iterator : public ebmlMasterElement::_const_iterator {
        private:
            ebml::ptr<const ebmlList> _elem;
            std::vector<ebmlElement_sp>::const_iterator _iter;

        protected:
            _const_iterator(const ebml::ptr<const ebmlList>& elem, const std::vector<ebmlElement_sp>::const_iterator& iter);
            _const_iterator(ebml::ptr<const ebmlList>&& elem, std::vector<ebmlElement_sp>::const_iterator&& iter);

        public:
            _const_iterator();
            virtual ~_const_iterator();

            ebmlMasterElement::_const_iterator* copy() const;
            const ebmlElement& operator*() const;
            ebmlMasterElement::_const_iterator& operator++();
            ebmlMasterElement::_const_iterator& operator=(const ebmlMasterElement::_const_iterator&);
            bool operator==(const ebmlMasterElement::_const_iterator&) const;
            bool operator!=(const ebmlMasterElement::_const_iterator&) const;

            friend class ebmlList;
            friend class ebmlMasterElement::const_iterator;
        };

        ebmlMasterElement::_const_iterator* _cbegin() const override;
        ebmlMasterElement::_const_iterator* _cend() const override;

        void _addChild(const ebmlElement_sp&) override;
        void _addChild(ebmlElement_sp&&) override;

        friend class ebmlListClass;
        // template<typename T, typename U, typename... Args>
        // friend ebml::ptr<U> new_sp(Args... args);
    };

    extern template class childSlot_t<ebmlList>;
    extern template childSlot_t<ebmlMasterElement>& childSlot_t<ebmlMasterElement>::operator=<ebmlList>(const ebmlList_sp&);
    extern template childSlot_t<ebmlMasterElement>& childSlot_t<ebmlMasterElement>::operator=<ebmlList>(ebmlList_sp&&);
}
#endif
