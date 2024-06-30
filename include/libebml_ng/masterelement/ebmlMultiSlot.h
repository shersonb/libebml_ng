#ifndef EBML_NG_MASTERELEMENT_MULTSLOT_H
#define EBML_NG_MASTERELEMENT_MULTSLOT_H

// #include "libebml_ng/base.h"
#include <unordered_map>
#include <vector>
#include <variant>
#include <ext/new_allocator.h>

#include "base/ebmlMasterElement.h"
#include "base/ebmlMasterElementClass.h"
#include "base/childSlot_t.h"

namespace ebml {
    // Slot specification to be stored in ebmlMultiSlotClass.

    class slotSpec_t {
    public:
        std::string name;

    protected:
        childClassSpec_t _childClasses;
        bool _multi;

    public:
        slotSpec_t();
        slotSpec_t(const std::string&, const childClassSpecArg_l&, bool multielem=false);
        slotSpec_t(const slotSpec_t&);
        slotSpec_t(slotSpec_t&&);
        ~slotSpec_t();

        slotSpec_t& operator=(const slotSpec_t&);
        slotSpec_t& operator=(slotSpec_t&&);

        const childClassSpec_t& childClasses() const;
        bool multi() const;
        // const c_ebmlElement_sp& defaultval() const;
        // const c_ebmlElement_l& defaultvals() const;

        friend class _slot_t;
        friend class ebmlMultiSlotClass;
    };

    typedef std::vector<slotSpec_t> slotSpec_l;

    class slotArg_t;
    typedef std::list<slotArg_t> slotArg_l;
    typedef std::unordered_map<std::string, slotArg_t> slotArg_d;

    class ebmlMultiSlot;
    typedef ebml::ptr<ebmlMultiSlot> ebmlMultiSlot_sp;

    // class ebmlMultiSlotClass : public ebmlMasterElementClass, public ebmlElementClassTmpl<ebmlMultiSlotClass, ebmlMultiSlot> {
    class ebmlMultiSlotClass : public ClsMixin<ebmlMultiSlotClass, ebmlMultiSlot, ebmlMasterElementClass> {
        friend class ebmlMultiSlot;

    private:
        std::vector<slotSpec_t> _slotSpecs;
        std::unordered_map<ebmlID_t, const slotSpec_t*> _slots_by_ebmlID;
        std::unordered_map<std::string, const slotSpec_t*> _slots_by_name;

    public:
        ebmlMultiSlotClass(const char*, const std::wstring&, const slotSpec_l&);
        // ebmlMultiSlotClass(const char*, const std::wstring&, const slotSpec_l&, const occurSpec_t& recursive, size_t recurseslot);

        ebmlMultiSlotClass(ebmlID_t, const std::wstring&, const slotSpec_l&);
        // ebmlMultiSlotClass(ebmlID_t, const std::wstring&, const slotSpec_l&, const occurSpec_t& recursive, size_t recurseslot);

        ebmlMultiSlotClass(const char*, const std::wstring&, const slotSpec_l&, const seekHelper_t* seekHelper);
        // ebmlMultiSlotClass(const char*, const std::wstring&, const slotSpec_l&, const occurSpec_t& recursive, size_t recurseslot, const seekHelper_t* seekHelper);

        ebmlMultiSlotClass(ebmlID_t, const std::wstring&, const slotSpec_l&, const seekHelper_t* seekHelper);
        // ebmlMultiSlotClass(ebmlID_t, const std::wstring&, const slotSpec_l&, const occurSpec_t& recursive, size_t recurseslot, const seekHelper_t* seekHelper);

        // ebmlMultiSlot_sp operator()(const slotArg_l&) const;
        // ebmlMultiSlot_sp operator()(const slotArg_d&) const;
        // ebmlMultiSlot_sp operator()(const slotArg_l&, const slotArg_d&) const;
        //
        // ebmlMultiSlot_sp operator()(slotArg_l&&) const;
        // ebmlMultiSlot_sp operator()(slotArg_d&&) const;
        // ebmlMultiSlot_sp operator()(slotArg_l&&, slotArg_d&&) const;
        // std::wstring repr() const;

    protected:
        // ebmlElement* _new() const override;
        // ebmlElement_sp _decode(const parseString&) const override;
        // ebmlElement_sp _decode(const parseFile&) const override;
        // ebmlElement_sp _cdecode(const parseString&) const override;
        // ebmlElement_sp _cdecode(const parseFile&) const override;

    public:
        const std::vector<slotSpec_t>& slotSpecs() const;
        const std::unordered_map<std::string, const slotSpec_t*>& slotSpecsByKey() const;
        const std::unordered_map<ebmlID_t, const slotSpec_t*>& slotSpecsByID() const;
        const ebmlElementClass* clsBySlot(const std::string&) const;
        const ebmlElementClass* clsBySlot(size_t) const;
    };

    // Wrapper for ebmlElement_sp or ebmlElement_l object to be passed via std::list<slotArg_t> to construct an ebmlMultiSlot instance.
    class slotArg_t {
    private:
        void _check_single() const;
        void _check_multi() const;
        bool _multi;
        union {
            ebmlElement_sp elem;
            ebmlElement_l elems;
        };

    public:
        slotArg_t();
        slotArg_t(const std::nullptr_t&);
        slotArg_t(const ebmlElement_sp&);

        template<typename T>
        slotArg_t(const ebml::ptr<T>& ptr) : slotArg_t(ebml_static_pointer_cast<ebmlElement>(ptr)) {}

        template<typename T>
        slotArg_t(ebml::ptr<T>&& ptr) : slotArg_t(ebml_static_pointer_cast<ebmlElement>(std::move(ptr))) {}

        slotArg_t(ebmlElement_sp&&);
        slotArg_t(const ebmlElement_l&);
        slotArg_t(ebmlElement_l&&);

        slotArg_t(const slotArg_t&);
        slotArg_t(slotArg_t&&);

        // Copy/move assign
        slotArg_t& operator=(const slotArg_t&);
        slotArg_t& operator=(slotArg_t&&);

        // Copy/move assign from ebmlElement_sp and ebmlElement_l
        slotArg_t& operator=(const ebmlElement_sp&);
        slotArg_t& operator=(ebmlElement_sp&&);

        slotArg_t& operator=(const ebmlElement_l&);
        slotArg_t& operator=(ebmlElement_l&&);

        bool multi() const;

        operator ebmlElement_sp&();
        operator const ebmlElement_sp&() const;
        operator ebmlElement_l&();
        operator const ebmlElement_l&() const;

        ~slotArg_t();
    };

    typedef ebml::ptr<ebmlMultiSlot> ebmlMultiSlot_sp;
    typedef ebml::ptr<const ebmlMultiSlot> c_ebmlMultiSlot_sp;
    class slot_t;
    class _slot_t;

    class const_slot_t {
    private:
        c_ebmlMultiSlot_sp _parent;
        const _slot_t* _slot;

    protected:
        const_slot_t(const ebml::ptr<const ebmlMultiSlot>&, const _slot_t&);
        const_slot_t(ebml::ptr<const ebmlMultiSlot>&&, const _slot_t&);

    public:
        const_slot_t(const slot_t&);
        const_slot_t(slot_t&&);
        const_slot_t(const const_slot_t&);
        const_slot_t(const_slot_t&&);

        const_slot_t& operator=(const const_slot_t&);
        const_slot_t& operator=(const_slot_t&&);
        const_slot_t& operator=(const slot_t&);
        const_slot_t& operator=(slot_t&&);

        // Single-element access
        operator c_ebmlElement_sp() const;

        // Multi-element access
        operator c_ebmlElement_l() const;
#if RAW
        const ebmlElement* operator[](size_t) const;
#else
        c_ebmlElement_sp operator[](size_t) const;
#endif

        class iterator {
        private:
            const _slot_t* _slot;
            union {
                bool _done;
                ebmlElement_l::const_iterator _iter;
            };

        protected:
            iterator();
            iterator(const _slot_t&, bool);
            iterator(const _slot_t&, const ebmlElement_l::const_iterator&);
            iterator(const _slot_t&, ebmlElement_l::const_iterator&&);
            bool multi() const;

        public:
            iterator(const iterator&);
            iterator(iterator&&);
            iterator& operator=(const iterator&);
            iterator& operator=(iterator&&);

            iterator& operator++();
            iterator operator++(int);
            bool operator==(const iterator&) const;

#if RAW
            const ebmlElement* operator*() const;
#else
            c_ebmlElement_sp operator*() const;
#endif

            ~iterator();
            friend class slot_t;
            friend class const_slot_t;
            friend class ebmlMultiSlot;
        };

        const_slot_t::iterator cbegin() const;
        const_slot_t::iterator cend() const;

        friend class iterator;
        friend class ebmlMultiSlot;
    };

    template<typename T>
    T data(const const_slot_t& slot) {
        c_ebmlElement_sp elem = slot;
        return data<T>(elem);
    }

    // Provides a public view of _slot_t instance
    class slot_t {
    protected:
        ebmlMultiSlot_sp _parent;
        _slot_t* _slot;
        slot_t(const ebml::ptr<ebmlMultiSlot>&, _slot_t&);
        slot_t(ebml::ptr<ebmlMultiSlot>&&, _slot_t&);
        // slot_t(ebmlElement_sp&&, _slot_t&);

    public:
        slot_t(const slot_t&);
        slot_t(slot_t&&);

        slot_t& operator=(const slot_t&);
        slot_t& operator=(slot_t&&);

        // Single-element access
        slot_t& operator=(const ebmlElement_sp&);
        slot_t& operator=(ebmlElement_sp&&);

        // operator ebmlElement_sp();
        operator const ebmlElement_sp&();
        operator c_ebmlElement_sp() const;

        // Multi-element access
        slot_t& operator=(const ebmlElement_l&);
        slot_t& operator=(ebmlElement_l&&);

        operator const ebmlElement_l&();
        operator c_ebmlElement_l() const;

        childSlot_t<> operator[](size_t);

        template<typename T=ebmlElement>
        childSlot_t<T> at(size_t);

        template<typename T=ebmlElement>
        childSlot_t<T> front();

        template<typename T=ebmlElement>
        childSlot_t<T> back();

        template<typename T>
        T& data(size_t);

        template<typename T>
        const T& data(size_t) const;

        childSlot_t<> push_back(const ebmlElement_sp&);
        childSlot_t<> push_back(ebmlElement_sp&&);

        size_t size() const;

#if RAW
        const ebmlElement* operator[](size_t) const;
        const ebmlElement* at(size_t) const;
        const ebmlElement* front() const;
        const ebmlElement* back() const;
#else
        c_ebmlElement_sp operator[](size_t) const;
        c_ebmlElement_sp at(size_t) const;
        c_ebmlElement_sp front() const;
        c_ebmlElement_sp back() const;
#endif

        class iterator {
        private:
            _slot_t* _slot;
            union {
                bool _done;
                ebmlElement_l::iterator _iter;
            };

        protected:
            iterator();
            iterator(_slot_t&, bool);
            iterator(_slot_t&, const ebmlElement_l::iterator&);
            iterator(_slot_t&, ebmlElement_l::iterator&&);
            bool multi() const;

        public:
            iterator(const iterator&);
            iterator(iterator&&);
            iterator& operator=(const iterator&);
            iterator& operator=(iterator&&);

            iterator& operator++();
            iterator operator++(int);
            bool operator==(const iterator&) const;
            const ebmlElement_sp& operator*() const;

            ~iterator();
            friend class _slot_t;
            friend class slot_t;
            friend class ebmlMultiSlot;
        };

        iterator begin();
        iterator end();

        typedef const_slot_t::iterator const_iterator;
        const_iterator cbegin() const;
        const_iterator cend() const;

        friend class const_slot_t;
        friend class iterator;
        // friend class const_iterator;
        friend class ebmlMultiSlot;
    };

    class _slot_t {
    protected:
        const slotSpec_t* _spec;
        union {
            ebmlElement_sp elem;
            ebmlElement_l elems;
        };

    private:
        void _check_single() const;
        void _check_multi() const;

    protected:
        _slot_t(const slotSpec_t& spec);
        _slot_t(const slotSpec_t& spec, const ebmlElement_sp& elem);
        _slot_t(const slotSpec_t& spec, ebmlElement_sp&& elem);
        _slot_t(const slotSpec_t& spec, const ebmlElement_l& elems);
        _slot_t(const slotSpec_t& spec, ebmlElement_l&& elems);

    public:
        _slot_t();
        _slot_t(const _slot_t& other);
        _slot_t(_slot_t&& other);
        _slot_t& operator=(const _slot_t& other);
        _slot_t& operator=(_slot_t&& other);
        ~_slot_t();

    private:
        void _validate(const ebmlElement_sp& elem) const;
        void _validate(const ebmlElement_l& elems) const;

    public:
        void validate(const ebmlElement_sp& elem) const;
        void validate(const ebmlElement_l& elems) const;
        const slotSpec_t& slotSpec() const;
        bool multi() const;
        size_t size() const;

        // Single-element access and modification
        _slot_t& operator=(const ebmlElement_sp& other);
        _slot_t& operator=(ebmlElement_sp&& other);
        operator ebmlElement_sp() const;
        operator ebmlElement_sp&();
        operator const ebmlElement_sp&() const;
        operator c_ebmlElement_sp() const;

        // Mutli-element access and modification
        operator ebmlElement_l&();

    protected:
        operator const ebmlElement_l&() const;

    public:
        operator c_ebmlElement_l() const;
        ebmlElement_sp& operator[](size_t index);
        ebmlElement_sp& at(size_t index);
        ebmlElement_sp& front();
        ebmlElement_sp& back();
        ebmlElement_sp& insert(size_t index, const ebmlElement_sp& elem);
        ebmlElement_sp& insert(size_t index, ebmlElement_sp&& elem);
        void erase(size_t index);
        ebmlElement_sp& push_back(const ebmlElement_sp& elem);
        ebmlElement_sp& push_back(ebmlElement_sp&& elem);
        ebmlElement_sp& emplace_back(const ebmlElement_sp& elem);
        ebmlElement_sp& emplace_back(ebmlElement_sp&& elem);
#if RAW
        const ebmlElement* operator[](size_t index) const;
        const ebmlElement* at(size_t index) const;
        const ebmlElement* front() const;
        const ebmlElement* back() const;
#else
        c_ebmlElement_sp operator[](size_t index) const;
        c_ebmlElement_sp at(size_t index) const;
        c_ebmlElement_sp front() const;
        c_ebmlElement_sp back() const;
#endif
        // operator ebmlElement_l();
        // operator c_ebmlElement_l();

        // Assign multi-element at once
        _slot_t& operator=(const ebmlElement_l& other);
        _slot_t& operator=(ebmlElement_l&& other);

        std::wstring repr() const;

        friend class slot_t;
        friend class slot_t::iterator;
        friend class const_slot_t;
        friend class const_slot_t::iterator;
        friend class ebmlMultiSlot;
    };

    template<typename T>
    T& data(slot_t& slot) {
        const ebmlElement_sp& elem = slot;
        return data<T>(elem);
    }


    // class ebmlMultiSlot : public ebmlMasterElement, public ebmlElementTmpl<ebmlMultiSlotClass, ebmlMultiSlot> {
    class ebmlMultiSlot : public InstMixin<ebmlMultiSlotClass, ebmlMultiSlot, ebmlMasterElement> {
    private:
        std::vector<_slot_t> _slots;
        std::unordered_map<std::string, _slot_t*> _slots_by_name;
        std::unordered_map<ebmlID_t, _slot_t*> _slots_by_ebmlID;

    protected:
        ebmlMultiSlot(const ebmlMultiSlotClass*);
        ebmlMultiSlot(const ebmlMultiSlotClass*, const slotArg_l&);
        ebmlMultiSlot(const ebmlMultiSlotClass*, slotArg_l&&);
        ebmlMultiSlot(const ebmlMultiSlotClass*, const slotArg_d&);
        ebmlMultiSlot(const ebmlMultiSlotClass*, slotArg_d&&);
        ebmlMultiSlot(const ebmlMultiSlotClass*, const slotArg_l&, const slotArg_d&);
        ebmlMultiSlot(const ebmlMultiSlotClass*, slotArg_l&&, const slotArg_d&);
        ebmlMultiSlot(const ebmlMultiSlotClass*, const slotArg_l&, slotArg_d&&);
        ebmlMultiSlot(const ebmlMultiSlotClass*, slotArg_l&&, slotArg_d&&);
        ebmlMultiSlot(const ebmlMultiSlot&);
        void _clear() override;

        // Check existence of required args and extraneous args
        void _validateArgs(const slotArg_l& args);
        void _validateArgs(const slotArg_d& kwargs);
        void _validateArgs(const slotArg_l& args, const slotArg_d& kwargs);

        // Check if data can be set by attempting to attach children.
        void _validateData(const slotArg_l& args);
        void _validateData(const slotArg_d& args);

        // Set data, assuming above checks have passed.
        void _setData(const slotArg_l& args);
        void _setData(slotArg_l&& args);

        void _setData(const slotArg_d& kwargs);
        void _setData(slotArg_d&& kwargs);

    public:
        void setData(const slotArg_l&);
        void setData(slotArg_l&&);
        void setData(const slotArg_d&);
        void setData(slotArg_d&&);
        void setData(const slotArg_l&, const slotArg_d&);
        void setData(slotArg_l&&, slotArg_d&&);
        std::wstring minirepr() const override;

        // Slot access
        slot_t operator[](size_t);
        slot_t operator[](const std::string&);

        const_slot_t operator[](size_t) const;
        const_slot_t operator[](const std::string&) const;

        template<typename T>
        T& data(size_t);

        template<typename T>
        T& data(const std::string&);

        template<typename T>
        const T& data(size_t) const;

        template<typename T>
        const T& data(const std::string&) const;

        // Iteration
    protected:
        class _iterator : public ebmlMasterElement::_iterator {
        private:
            ebml::ptr<ebmlMultiSlot> _elem;
            std::vector<_slot_t>::iterator _slotiter;
            std::vector<_slot_t>::iterator _slotiterend;
            slot_t::iterator _iter;
            slot_t::iterator _iterend;

        protected:
            _iterator(
                const ebml::ptr<ebmlMultiSlot>& elem,
                const std::vector<_slot_t>::iterator& slotiter,
                const std::vector<_slot_t>::iterator& slotiterend,
                const slot_t::iterator& iter,
                const slot_t::iterator& iterend
            );

            _iterator(
                ebml::ptr<ebmlMultiSlot>&& elem,
                std::vector<_slot_t>::iterator&& slotiter,
                std::vector<_slot_t>::iterator&& slotiterend,
                slot_t::iterator&& iter,
                slot_t::iterator&& iterend
            );

        public:
            _iterator();
            virtual ~_iterator();

            ebmlMasterElement::_iterator* copy() const;

            ebmlElement& operator*() const;
            ebmlMasterElement::_iterator& operator++();
            ebmlMasterElement::_iterator& operator=(const ebmlMasterElement::_iterator&);
            bool operator==(const ebmlMasterElement::_iterator&) const;
            bool operator!=(const ebmlMasterElement::_iterator&) const;

            friend class ebmlMultiSlot;
            friend class ebmlMasterElement::iterator;
            // friend class ebml::ptr<_iterator>;
        };

        ebmlMasterElement::_iterator* _begin() override;
        ebmlMasterElement::_iterator* _end() override;


        class _const_iterator : public ebmlMasterElement::_const_iterator {
        private:
            ebml::ptr<const ebmlMultiSlot> _elem;
            std::vector<_slot_t>::const_iterator _slotiter;
            std::vector<_slot_t>::const_iterator _slotiterend;
            const_slot_t::iterator _iter;
            const_slot_t::iterator _iterend;

        protected:
            _const_iterator(
                const ebml::ptr<const ebmlMultiSlot>& elem,
                const std::vector<_slot_t>::const_iterator& slotiter,
                const std::vector<_slot_t>::const_iterator& slotiterend,
                const const_slot_t::iterator& iter,
                const const_slot_t::iterator& iterend
            );
            _const_iterator(
                ebml::ptr<const ebmlMultiSlot>&& elem,
                std::vector<_slot_t>::const_iterator&& slotiter,
                std::vector<_slot_t>::const_iterator&& slotiterend,
                const_slot_t::iterator&& iter,
                const_slot_t::iterator&& iterend
            );

        public:
            _const_iterator();
            virtual ~_const_iterator();
            ebmlMasterElement::_const_iterator* copy() const;
            const ebmlElement& operator*() const;
            ebmlMasterElement::_const_iterator& operator++();
            ebmlMasterElement::_const_iterator& operator=(const ebmlMasterElement::_const_iterator&);
            bool operator==(const ebmlMasterElement::_const_iterator&) const;
            bool operator!=(const ebmlMasterElement::_const_iterator&) const;
            friend class ebmlMultiSlot;
            friend class ebmlMasterElement::const_iterator;
        };

        // ebml::ptr<ebmlMasterElement::_const_iterator> _cbegin() const override;
        // ebml::ptr<ebmlMasterElement::_const_iterator> _cend() const override;

        ebmlMasterElement::_const_iterator* _cbegin() const override;
        ebmlMasterElement::_const_iterator* _cend() const override;

    protected:
        void _addChild(const ebmlElement_sp&) override;
        void _addChild(ebmlElement_sp&&) override;
    //     // virtual void _scanChildren(parseFile::iterator&); // Decode children from an iterParseFile instance created by _decode.
        friend class ClsMixin<ebmlMultiSlotClass, ebmlMultiSlot, ebmlMasterElementClass>;
        friend class InstMixin<ebmlMultiSlotClass, ebmlMultiSlot, ebmlMasterElement>;
        friend class slot_t;
    };

    extern template class ClsMixin<ebmlMultiSlotClass, ebmlMultiSlot, ebmlMasterElementClass>;
    extern template class InstMixin<ebmlMultiSlotClass, ebmlMultiSlot, ebmlMasterElement>;
    extern template class flexible_ptr<ebmlMultiSlot>;
    extern template class ebml_shared_ptr<ebmlMultiSlot>;
    extern template class ebml_weak_ptr<ebmlMultiSlot>;
}
#endif
