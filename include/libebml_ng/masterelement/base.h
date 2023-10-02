#ifndef EBML_NG_MASTERELEMENT_H
#define EBML_NG_MASTERELEMENT_H

#include "libebml_ng/base.h"
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <list>
#include <vector>

namespace ebml {
    // Creates a shared_ptr<U> to a new T object. T must be a subclass of U.

    // Used in the creation of objects in which a shared_ptr must be constructed in the constructor.
    // In order for this to work, the constructor MUST catch all exceptions and assign an exception
    // pointer to a reference.

    // Destructors must also be written to anticipate failed construction of T objects.

    // Any constructor used must be formatted as follows:
    // T::T(Args... args, std::shared_ptr<U>& this_sp, std::exception_ptr& exc) : (initializers) {
    //     auto ptr = std::shared_ptr<U>(this);
    //
    //     try {
    //         // Place constructor code here;
    //     } catch (...) {
    //         ptr = nullptr;
    //         exc = std::current_exception();
    //         return;
    //     }
    //     this_sp = std::move(ptr);
    // }

    template<typename T, typename U, typename... Args>
    std::shared_ptr<U> new_sp(Args... args) {
        std::shared_ptr<U> sp;
        std::exception_ptr exc;
        new T(args..., sp, exc);

        if (exc) {
            std::rethrow_exception(exc);
        }

        return sp;
    }

    // CHILD ELEMENT MANAGEMENT

    // Used in the initialization of a new ebmlMasterElementClass (and its c++ subclass) instances.
    struct childClassSpecArg_t {
        childClassSpecArg_t();
        childClassSpecArg_t(const ebmlElementClass*, unsigned long min=0, long max=-1);
        const ebmlElementClass* cls;
        unsigned long min; // Minimum number of occurrences required.
        long max; // Maximum number of occurrences permitted. Negative values interpreted as infinity.
    };

    typedef std::list<childClassSpecArg_t> childClassSpecArg_l;
    typedef std::initializer_list<childClassSpecArg_t> childClassSpecArg_init_l;

    struct occurSpec_t {
        unsigned long min;
        long max;
    };

    typedef std::unordered_map<ebmlID_t, const ebmlElementClass*> ebmlElementClass_d;
    typedef std::unordered_set<const ebmlElementClass*> ebmlElementClass_s;
    typedef std::unordered_map<ebmlID_t, occurSpec_t> occurSpec_d;

    // Child element class specification. Used to specify accepted EBML Element types within a parent element.
    // Also used to look up EBMLElementClass* instances by ebmlID_t

    class childClassSpec_t {
    private:
        ebmlElementClass_d _cls_by_ebmlID;
        ebmlElementClass_s _clss;
        occurSpec_d _occurspec;
    public:
        childClassSpec_t();
        childClassSpec_t(const childClassSpecArg_init_l&);
        childClassSpec_t(const childClassSpecArg_l&);
        childClassSpec_t(const childClassSpec_t&);

        void add(const childClassSpecArg_t&);
        void add(const ebmlElementClass*, unsigned long min=0, long max=-1);
        void remove(ebmlID_t);

        const ebmlElementClass* operator[](ebmlID_t) const;
        size_t count(ebmlID_t) const;
        size_t count(const ebmlElementClass*) const;
        size_t size() const;

        const occurSpec_d& occurSpec() const;
        const occurSpec_t& occurSpec(ebmlID_t) const;
        const occurSpec_t& occurSpec(const ebmlElementClass*) const;

        bool isValid(const ebmlElement_sp&) const;

        ebmlElementClass_d::const_iterator begin() const;
        ebmlElementClass_d::const_iterator end() const;
    };

    class ebmlMasterElement;

    // childSlot_t: Used to simulate a reference to an ebmlElement_sp. Allows for setting a new value to the ebmlElement_sp,
    // but also performs validation checks, and automatically manages parent pointers.

    class childSlot_t {
    private:
        ebmlMasterElement* _parent;
        const childClassSpec_t* _spec;
        ebmlElement_sp* _childslot;
        bool _allownull = true;

    public:
        childSlot_t(ebmlMasterElement* parent, const childClassSpec_t&, ebmlElement_sp& childslot, bool allownull=true);
        childSlot_t& operator=(const ebmlElement_sp& item);
        childSlot_t& operator=(ebmlElement* item);
        childSlot_t& operator=(ebmlElement_sp&& item);

        bool operator==(const ebmlElement_sp& item);
        bool operator==(const c_ebmlElement_sp& item);
        bool operator==(const childSlot_t& item);
        operator ebmlElement_sp() const;
        operator const ebmlElement_sp&() const;
        ebmlElement* operator->() const;
        ebmlElement_sp get() const;
    };

    template<typename T>
    T& data(const childSlot_t& elem);

    // Containers of ebmlElement_sp, c_ebmlElement_sp

    typedef std::vector<ebmlElement_sp> ebmlElement_l;
    // typedef std::vector<ebmlElement_sp> ebmlElement_l;

    // c_ebmlElement_l: A const view of ebmlElement_l.

    class c_ebmlElement_l {
    private:
        const ebmlElement_l* _items;
    public:
        c_ebmlElement_l(const ebmlElement_l&);
        c_ebmlElement_l(const c_ebmlElement_l&);
#if RAW
        const ebmlElement* front() const;
        const ebmlElement* back() const;
        const ebmlElement* at(size_t) const;
        const ebmlElement* operator[](size_t) const;
#else
        c_ebmlElement_sp front() const;
        c_ebmlElement_sp back() const;
        c_ebmlElement_sp at(size_t) const;
        c_ebmlElement_sp operator[](size_t) const;
#endif
        size_t size() const;
        bool empty() const;

        class const_iterator {
        private:
            ebmlElement_l::const_iterator _iter;

        protected:
            const_iterator(const ebmlElement_l::const_iterator&);
            const_iterator(ebmlElement_l::const_iterator&&);

        public:
            const_iterator(const c_ebmlElement_l&);
            const_iterator(const const_iterator&);
            const_iterator(const_iterator&&);

            bool operator=(const const_iterator&);
            bool operator=(const_iterator&&);

#if RAW
            const ebmlElement* operator*() const;
#else
            c_ebmlElement_sp operator*() const;
#endif

            const ebmlElement* operator->() const;
            const_iterator& operator++();
            const_iterator operator++(int);
            bool operator==(const const_iterator&) const;
            bool operator!=(const const_iterator&) const;
        };

        const_iterator cbegin() const;
        const_iterator cend() const;
    };


    // Used to keep track of element count
    typedef std::unordered_map<ebmlID_t, unsigned long> occur_d;

    struct sizetree_t {
        unsigned char ebmlIDWidth;
        unsigned char sizeWidth;
        size_t dataSize;
        std::deque<sizetree_t> children;

        sizetree_t();
        sizetree_t(sizetree_t&&);
        sizetree_t(unsigned char _ebmlIDWidth, unsigned char _sizeWidth, size_t _dataSize);
        sizetree_t& operator=(sizetree_t&&);
        size_t outerSize() const;
    };

    class ebmlMasterElement;

    // Base class for EBML Master Element Types.

    class ebmlMasterElementClass : public ebmlElementClass {
    protected:
        childClassSpec_t _childClasses;

    public:
        ebmlMasterElementClass(const char*, const std::wstring&);
        ebmlMasterElementClass(ebmlID_t, const std::wstring&);

        // Abstract method inherited from ebml::ebmlElementClass
        // virtual ebmlElement_sp operator()() const = 0;

        // Offers const accsss to _childClasses member.
        const childClassSpec_t& childClasses() const;
        friend class ebmlMasterElement;
    };

    // Base class for EBML Master Element instances.
    // Implements framework for decode, encode, and public-facing iteration.
    // Each subclass must implement its own child element placement and private iteration scheme

    class ebmlMasterElement : public ebmlElement {
        // Constructors
    protected:
        ebmlMasterElement(const ebmlMasterElementClass*);

        // Reminder: This method should be overriden in every subclass
        virtual const ebmlMasterElementClass* cls() const;

        // dataSize override
    public:
        size_t dataSize() const;

        // Size Tree methods
    public:
        sizetree_t sizetree() const;

    // protected:
    //     sizetree_t _build_sizetree() const;
    //     void _build_sizetree(sizetree_t&) const;

        // Encode functions.
    protected:
        // NEW in ebmlMasterElement. Protected _encode that makes use of a sizetree_t
        // to avoid redundant calls to outerSize() on all of its decendants.
        size_t _encode(char*, const sizetree_t&) const;

        size_t _encode(char*) const;
        // size_t _encode(char*, size_t) const;

    public:
        // Override the default encode to make use of _encode(char*, const sizetree_t&) instead of _encode(char*);
        std::string encode() const;
        size_t encode(char*) const;
        size_t encode(char*, const sizetree_t&) const;

    protected:
        virtual void _clear(); // Clear all children.

        // Decode functions:
            // From string
        virtual void _decode(const parseString&);
        virtual void _decodeChildren(parseString::iterator&);
        virtual ebmlElement_sp _decodeChild(const parseString&);

        virtual void _cdecode(const parseString&);
        virtual void _cdecodeChildren(parseString::iterator&);
        virtual ebmlElement_sp _cdecodeChild(const parseString&);

            // From file
        virtual void _decode(const parseFile&); // Decode from a file
        virtual void _scanChildren(parseFile::iterator&); // Decode children from an iterParseFile instance created by _decode.
        virtual void _handleSeekData(const parseFile&); // Controls the handling of seek data pointing to a child element.
        // virtual void _scanChild(const parseFile&); // Implement in ebmlLazyLoadMasterElement
        virtual ebmlElement_sp _decodeChild(const parseFile&);

        virtual void _cdecode(const parseFile&); // Decode from a file
        virtual void _cscanChildren(parseFile::iterator&); // Decode children from an iterParseFile instance created by _decode.
        virtual void _chandleSeekData(const parseFile&); // Controls the handling of seek data pointing to a child element.
        // virtual void _scanChild(const parseFile&); // Implement in ebmlLazyLoadMasterElement
        virtual ebmlElement_sp _cdecodeChild(const parseFile&);

        // Add child element (whether decoded from string or from file)
        virtual void _addChild(const ebmlElement_sp&) = 0;
        virtual void _addChild(ebmlElement_sp&&) = 0;

        // Iteration
    public:
        class iterator;

    protected:
        class _iterator {
        public:
            // _iterator();

            // virtual std::shared_ptr<_iterator> copy() = 0;
            virtual _iterator* copy() const = 0;

            virtual const ebmlElement_sp& operator*() const = 0;
            virtual _iterator& operator++() = 0;

            virtual _iterator& operator=(const _iterator&) = 0;

            virtual bool operator==(const _iterator&) const = 0;
            virtual bool operator!=(const _iterator&) const = 0;

            virtual ~_iterator() {};

            friend class ebmlMasterElement;
            friend class ebmlMasterElement::iterator;
        };

        virtual ebmlMasterElement::_iterator* _begin() = 0;
        virtual ebmlMasterElement::_iterator* _end() = 0;

    public:
        class iterator {
        private:
            // std::shared_ptr<_iterator> _iter;
            // iterator(const std::shared_ptr<_iterator>&);

            _iterator* _iter;

        protected:
            iterator(_iterator*);

        public:
            const ebmlElement_sp& operator*() const;
            iterator& operator++();

            iterator(const iterator&);
            iterator(iterator&&);
            iterator& operator=(const iterator&);
            iterator& operator=(iterator&&);

            bool operator==(const iterator&) const;
            bool operator!=(const iterator&) const;

            ~iterator();

            friend class ebmlMasterElement;
        };

        iterator begin();
        iterator end();

        // Const Iteration
    public:
        class const_iterator;

    protected:
        class _const_iterator {
        public:
            // _const_iterator();

            // virtual std::shared_ptr<_const_iterator> copy() = 0;
            virtual _const_iterator* copy() const = 0;

#if RAW
            virtual const ebmlElement* operator*() const = 0;
#else
            virtual c_ebmlElement_sp operator*() const = 0;
#endif
            virtual _const_iterator& operator++() = 0;

            virtual _const_iterator& operator=(const _const_iterator&) = 0;

            virtual bool operator==(const _const_iterator&) const = 0;
            virtual bool operator!=(const _const_iterator&) const = 0;

            virtual ~_const_iterator() {};

            friend class ebmlMasterElement;
            friend class ebmlMasterElement::const_iterator;
        };

        // virtual std::shared_ptr<ebmlMasterElement::_const_iterator> _cbegin() const;
        // virtual std::shared_ptr<ebmlMasterElement::_const_iterator> _cend() const;

        virtual ebmlMasterElement::_const_iterator* _cbegin() const = 0;
        virtual ebmlMasterElement::_const_iterator* _cend() const = 0;

    public:
        class const_iterator {
        private:
            // std::shared_ptr<_const_iterator> _iter;
            // const_iterator(const std::shared_ptr<_const_iterator>&);

            _const_iterator* _iter;
            const_iterator(_const_iterator*);

        public:
#if RAW
            virtual const ebmlElement* operator*() const;
#else
            virtual c_ebmlElement_sp operator*() const;
#endif
            virtual const_iterator& operator++();

            const_iterator(const const_iterator&);
            const_iterator(const_iterator&&);
            virtual const_iterator& operator=(const const_iterator&);
            virtual const_iterator& operator=(const_iterator&&);

            virtual bool operator==(const const_iterator&) const;
            virtual bool operator!=(const const_iterator&) const;

            ~const_iterator();

            friend class ebmlMasterElement;
        };

        const_iterator cbegin() const;
        const_iterator cend() const;

    protected:
        static iterator make_iter(_iterator*);
        static const_iterator make_iter(_const_iterator*);

        // Cloning functions:
    protected:
        void _clonedata(const ebmlElement*);

    protected:
        occur_d ebmlIDCount;
        void _attachChild(const ebmlElement_sp& child, bool weak = true);
        void _attachChildren(const ebmlElement_l& elems, bool weak=true);
        void _detachChildren(const ebmlElement_l& elems);

    public:
        friend class ebmlMasterElementClass;
        friend class std::shared_ptr<ebmlMasterElement::_iterator>;
        friend class std::shared_ptr<ebmlMasterElement::_const_iterator>;
        friend class childSlot_t;

        friend std::shared_ptr<ebmlMasterElement::_iterator> std::make_shared<ebmlMasterElement::_iterator>();
        friend std::shared_ptr<ebmlMasterElement::_const_iterator> std::make_shared<ebmlMasterElement::_const_iterator>();
    };

    typedef std::shared_ptr<ebmlMasterElement> ebmlMasterElement_sp;
    typedef std::shared_ptr<const ebmlMasterElement> c_ebmlMasterElement_sp;
}
#endif
