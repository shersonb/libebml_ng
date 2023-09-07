#ifndef EBML_NG_MASTERELEMENT_LIST_H
#define EBML_NG_MASTERELEMENT_LIST_H

#include "libebml_ng/base.h"
#include "libebml_ng/masterelement/base.h"
#include <unordered_map>
#include <vector>

namespace ebml {
    class ebmlListClass : public ebmlMasterElementClass {
    public:
        ebmlListClass(ebmlID_t, std::wstring, const occurSpec_t& recursive={0, 0});
        ebmlListClass(ebmlID_t, std::wstring, const childClassSpecArg_l&, const occurSpec_t& recursive={0, 0});
        ebmlListClass(ebmlID_t, std::wstring, const childClassSpecArg_init_l&, const occurSpec_t& recursive={0, 0});

        ebmlElement_sp operator()() const;
        ebmlElement_sp operator()(const ebmlElement_l&) const;
        ebmlElement_sp operator()(ebmlElement_l&&) const;

    private:
        friend class ebmlList;
    };

    class ebmlList : public ebmlMasterElement {
    private:
        std::vector<ebmlElement_sp> _data;

    protected:
        ebmlList(const ebmlListClass*);
        ebmlList(const ebmlListClass*, const ebmlElement_l&);
        ebmlList(const ebmlListClass*, ebmlElement_l&&);
        void _clear() override;

    public:
        void setData(const ebmlElement_l&);
        void setData(ebmlElement_l&&);
        std::wstring minirepr() const override;

        // Element access
        childSlot_t operator[](off_t);
        childSlot_t at(off_t);
        childSlot_t front();
        childSlot_t back();

        // Const element access
#if RAW
        const ebmlElement* operator[](off_t) const;
        const ebmlElement* at(off_t) const;
        const ebmlElement* front() const;
        const ebmlElement* back() const;
#else
        c_ebmlElement_sp operator[](off_t) const;
        c_ebmlElement_sp at(off_t) const;
        c_ebmlElement_sp front() const;
        c_ebmlElement_sp back() const;
#endif

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
            ebmlElement_sp _elem;
            std::vector<ebmlElement_sp>::iterator _iter;

        protected:
            _iterator(const ebmlElement_sp& elem, const std::vector<ebmlElement_sp>::iterator& iter);

        public:
            _iterator();
            virtual ~_iterator();

            ebmlMasterElement::_iterator* copy();

            const ebmlElement_sp& operator*() const;
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
            c_ebmlElement_sp _elem;
            std::vector<ebmlElement_sp>::const_iterator _iter;

        protected:
            _const_iterator(const c_ebmlElement_sp& elem, const std::vector<ebmlElement_sp>::const_iterator& iter);

        public:
            _const_iterator();
            virtual ~_const_iterator();

            ebmlMasterElement::_const_iterator* copy();
#if RAW
            const ebmlElement* operator*() const;
#else
            c_ebmlElement_sp operator*() const;
#endif
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
    };
}
#endif
