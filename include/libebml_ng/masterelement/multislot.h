#ifndef EBML_NG_MASTERELEMENT_MULTSLOT_H
#define EBML_NG_MASTERELEMENT_MULTSLOT_H

#include "libebml_ng/base.h"
#include "libebml_ng/masterelement/base.h"
#include <unordered_map>
#include <vector>
#include <variant>
#include <ext/new_allocator.h>

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

    class ebmlMultiSlotClass : public ebmlMasterElementClass {
    public:
        ebmlMultiSlotClass(const char*, const std::wstring&, const slotSpec_l&);
        ebmlMultiSlotClass(const char*, const std::wstring&, const slotSpec_l&, const occurSpec_t& recursive, size_t recurseslot);

        ebmlMultiSlotClass(ebmlID_t, const std::wstring&, const slotSpec_l&);
        ebmlMultiSlotClass(ebmlID_t, const std::wstring&, const slotSpec_l&, const occurSpec_t& recursive, size_t recurseslot);

        ebmlElement_sp operator()(const slotArg_l&) const;
        ebmlElement_sp operator()(const slotArg_d&) const;
        ebmlElement_sp operator()(const slotArg_l&, const slotArg_d&) const;

        ebmlElement_sp operator()(slotArg_l&&) const;
        ebmlElement_sp operator()(slotArg_d&&) const;
        ebmlElement_sp operator()(slotArg_l&&, slotArg_d&&) const;
        // std::wstring repr() const;

    protected:
        ebmlElement* _new() const;
        std::vector<slotSpec_t> _slotSpecs;
        std::unordered_map<ebmlID_t, const slotSpec_t*> _slots_by_ebmlID;
        std::unordered_map<std::string, const slotSpec_t*> _slots_by_name;
        friend class ebmlMultiSlot;

    public:
        const std::vector<slotSpec_t>& slotSpecs() const;
        const std::unordered_map<std::string, const slotSpec_t*>& slotSpecsByKey() const;
        const std::unordered_map<ebmlID_t, const slotSpec_t*>& slotSpecsByID() const;
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

    class _slot_t;
    typedef std::shared_ptr<ebmlMultiSlot> ebmlMultiSlot_sp;
    typedef std::shared_ptr<const ebmlMultiSlot> c_ebmlMultiSlot_sp;
    class slot_t;

    class const_slot_t {
    private:
        c_ebmlMultiSlot_sp _parent;
        const _slot_t* _slot;

    protected:
        const_slot_t(const c_ebmlElement_sp&, const _slot_t&);

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
        slot_t(const ebmlElement_sp&, _slot_t&);
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

        childSlot_t operator[](size_t);
        childSlot_t at(size_t);
        childSlot_t front();
        childSlot_t back();
        childSlot_t push_back(const ebmlElement_sp&);
        childSlot_t push_back(ebmlElement_sp&&);

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

    template<typename T>
    T& data(slot_t& slot) {
        const ebmlElement_sp& elem = slot;
        return data<T>(elem);
    }


    class ebmlMultiSlot : public ebmlMasterElement {
    private:
        std::vector<_slot_t> _slots;
        std::unordered_map<std::string, _slot_t*> _slots_by_name;
        std::unordered_map<ebmlID_t, _slot_t*> _slots_by_ebmlID;

    protected:
        ebmlMultiSlot(const ebmlMultiSlotClass*);
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
    //     // const ebmlElement_sp& operator[](off_t);
    //     // const ebmlElement_sp& at(off_t);
    //     // const ebmlElement_sp& front();
    //     // const ebmlElement_sp& back();
    //     //
    //     // // Const element access
    //     // c_ebmlElement_sp operator[](off_t) const;
    //     // c_ebmlElement_sp at(off_t) const;
    //     // c_ebmlElement_sp front() const;
    //     // c_ebmlElement_sp back() const;
    //
    //     // // Element search
    //     // bool contains(const c_ebmlElement_sp&) const;
    //     // off_t index(const c_ebmlElement_sp&) const;
    //
    //     // // Element modification
    //     // void assign(off_t, const ebmlElement_sp&);
    //     // void push_back(const ebmlElement_sp&);
    //     // void pop_back();
    //     // void insert(off_t, const ebmlElement_sp&);
    //     // void erase(off_t);
    //     // void clear();
    //
    //     // Container size
    //     // size_t size() const;

        // Iteration
    protected:
        class _iterator : public ebmlMasterElement::_iterator {
        private:
            ebmlElement_sp _elem;
            std::vector<_slot_t>::iterator _slotiter;
            std::vector<_slot_t>::iterator _slotiterend;
            slot_t::iterator _iter;
            slot_t::iterator _iterend;

        protected:
            _iterator(
                const ebmlElement_sp& elem,
                const std::vector<_slot_t>::iterator& slotiter,
                const std::vector<_slot_t>::iterator& slotiterend,
                const slot_t::iterator& iter,
                const slot_t::iterator& iterend
            );

            _iterator(
                ebmlElement_sp&& elem,
                std::vector<_slot_t>::iterator&& slotiter,
                std::vector<_slot_t>::iterator&& slotiterend,
                slot_t::iterator&& iter,
                slot_t::iterator&& iterend
            );

        public:
            _iterator();
            virtual ~_iterator();

            ebmlMasterElement::_iterator* copy() const;

            const ebmlElement_sp& operator*() const;
            ebmlMasterElement::_iterator& operator++();
            ebmlMasterElement::_iterator& operator=(const ebmlMasterElement::_iterator&);
            bool operator==(const ebmlMasterElement::_iterator&) const;
            bool operator!=(const ebmlMasterElement::_iterator&) const;

            friend class ebmlMultiSlot;
            friend class ebmlMasterElement::iterator;
            friend class std::shared_ptr<_iterator>;
        };

        ebmlMasterElement::_iterator* _begin() override;
        ebmlMasterElement::_iterator* _end() override;


        class _const_iterator : public ebmlMasterElement::_const_iterator {
        private:
            c_ebmlElement_sp _elem;
            std::vector<_slot_t>::const_iterator _slotiter;
            std::vector<_slot_t>::const_iterator _slotiterend;
            const_slot_t::iterator _iter;
            const_slot_t::iterator _iterend;

        protected:
            _const_iterator(
                const c_ebmlElement_sp& elem,
                const std::vector<_slot_t>::const_iterator& slotiter,
                const std::vector<_slot_t>::const_iterator& slotiterend,
                const const_slot_t::iterator& iter,
                const const_slot_t::iterator& iterend
            );
            _const_iterator(
                c_ebmlElement_sp&& elem,
                std::vector<_slot_t>::const_iterator&& slotiter,
                std::vector<_slot_t>::const_iterator&& slotiterend,
                const_slot_t::iterator&& iter,
                const_slot_t::iterator&& iterend
            );

        public:
            _const_iterator();
            virtual ~_const_iterator();
            ebmlMasterElement::_const_iterator* copy() const;

#if RAW
            const ebmlElement* operator*() const;
#else
            c_ebmlElement_sp operator*() const;
#endif
            ebmlMasterElement::_const_iterator& operator++();
            ebmlMasterElement::_const_iterator& operator=(const ebmlMasterElement::_const_iterator&);
            bool operator==(const ebmlMasterElement::_const_iterator&) const;
            bool operator!=(const ebmlMasterElement::_const_iterator&) const;
            friend class ebmlMultiSlot;
            friend class ebmlMasterElement::const_iterator;
        };

        // std::shared_ptr<ebmlMasterElement::_const_iterator> _cbegin() const override;
        // std::shared_ptr<ebmlMasterElement::_const_iterator> _cend() const override;

        ebmlMasterElement::_const_iterator* _cbegin() const override;
        ebmlMasterElement::_const_iterator* _cend() const override;

    protected:
        void _addChild(const ebmlElement_sp&) override;
        void _addChild(ebmlElement_sp&&) override;
    //     // virtual void _scanChildren(parseFile::iterator&); // Decode children from an iterParseFile instance created by _decode.
        friend class ebmlMultiSlotClass;
        friend class slot_t;
    };
}
#endif
