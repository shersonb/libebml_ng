#ifndef EBML_NG_MASTERELEMENTCLASS_H
#define EBML_NG_MASTERELEMENTCLASS_H

#include "../../ebmlElementClass.h"
// #include "../element.h"
// #include "../exceptions.h"
#include "childClassSpec.h"
#include "sizetree_t.h"

// #include <unordered_map>
// #include <unordered_set>
// #include <deque>
// #include <list>
#include <vector>
// #include "../void.h"
// #include <optional>

namespace ebml {
    class ebmlMasterElement;

//     typedef std::vector<ebmlElement_sp> ebmlElement_l;
//
//     class c_ebmlElement_l {
//     private:
//         const ebmlElement_l* _items;
//     public:
//         c_ebmlElement_l(const ebmlElement_l&);
//         c_ebmlElement_l(const c_ebmlElement_l&);
// #if RAW
//         const ebmlElement* front() const;
//         const ebmlElement* back() const;
//         const ebmlElement* at(size_t) const;
//         const ebmlElement* operator[](size_t) const;
// #else
//         c_ebmlElement_sp front() const;
//         c_ebmlElement_sp back() const;
//         c_ebmlElement_sp at(size_t) const;
//         c_ebmlElement_sp operator[](size_t) const;
// #endif
//         size_t size() const;
//         bool empty() const;
//
//         class const_iterator {
//         private:
//             ebmlElement_l::const_iterator _iter;
//
//         protected:
//             const_iterator(const ebmlElement_l::const_iterator&);
//             const_iterator(ebmlElement_l::const_iterator&&);
//
//         public:
//             const_iterator(const c_ebmlElement_l&);
//             const_iterator(const const_iterator&);
//             const_iterator(const_iterator&&);
//
//             bool operator=(const const_iterator&);
//             bool operator=(const_iterator&&);
//
//             const ebmlElement& operator*() const;
//
//             const ebmlElement* operator->() const;
//             const_iterator& operator++();
//             const_iterator operator++(int);
//             bool operator==(const const_iterator&) const;
//             bool operator!=(const const_iterator&) const;
//         };
//
//         const_iterator cbegin() const;
//         const_iterator cend() const;
//     };


    // Used to keep track of element count
    typedef std::unordered_map<ebmlID_t, unsigned long> occur_d;

    // struct sizetree_t {
    //     vintWidth_t ebmlIDWidth;
    //     vintWidth_t sizeWidth;
    //     size_t dataSize;
    //     // std::deque<sizetree_t> children;
    //     std::optional<std::deque<sizetree_t>> children;
    //
    //     sizetree_t();
    //     sizetree_t(const sizetree_t&);
    //     sizetree_t(sizetree_t&&);
    //     sizetree_t(vintWidth_t _ebmlIDWidth, vintWidth_t _sizeWidth, size_t _dataSize);
    //     sizetree_t& operator=(const sizetree_t&);
    //     sizetree_t& operator=(sizetree_t&&);
    //     size_t outerSize() const;
    //     void append(const sizetree_t&);
    //     void append(sizetree_t&&);
    // };

    // Base class for EBML Master Element Types.

    class ebmlMasterElementClass : public ClsMixin<ebmlMasterElementClass, ebmlMasterElement> {
        friend class ebmlMasterElement;

    protected:
        childClassSpec_t _childClasses;

    public:
        ebmlMasterElementClass(const char*, const std::wstring&, const childClassSpecArg_l&);
        ebmlMasterElementClass(ebmlID_t, const std::wstring&, const childClassSpecArg_l&);
        ebmlMasterElementClass(const char*, const std::wstring&, const childClassSpecArg_l&, const seekHelper_t* seekHelper);
        ebmlMasterElementClass(ebmlID_t, const std::wstring&, const childClassSpecArg_l&, const seekHelper_t* seekHelper);

    protected:
        ebmlMasterElementClass(const char*, const std::wstring&);
        ebmlMasterElementClass(ebmlID_t, const std::wstring&);
        ebmlMasterElementClass(const char*, const std::wstring&, const seekHelper_t* seekHelper);
        ebmlMasterElementClass(ebmlID_t, const std::wstring&, const seekHelper_t* seekHelper);

        // ebmlElement_sp _decode(const parseString&) const override;
        // ebmlElement_sp _decode(const parseFile&) const override;
        // ebmlElement_sp _cdecode(const parseString&) const override;
        // ebmlElement_sp _cdecode(const parseFile&) const override;

        inline ebmlElement* _decodeChild(const parseString&) const;
        inline ebmlElement* _decodeChild(const parseFile&) const;
        inline ebmlElement* _cdecodeChild(const parseString&) const;
        inline ebmlElement* _cdecodeChild(const parseFile&) const;

        // ebmlElement* _decodeChild(const parseString&) const;
        // ebmlElement* _decodeChild(const parseFile&) const;
        // ebmlElement* _cdecodeChild(const parseString&) const;
        // ebmlElement* _cdecodeChild(const parseFile&) const;

    public:
        // Offers const accsss to _childClasses member.
        inline const childClassSpec_t& childClasses() const {
            return _childClasses;
        };
    };

    // Base class for EBML Master Element instances.
    // Implements framework for decode, encode, and public-facing iteration.
    // Each subclass must implement its own child element placement and private iteration scheme

    // class ebmlMasterElement : public ebmlElementTmpl<ebmlMasterElementClass, ebmlMasterElement> {
    // class ebmlMasterElement : public InstMixin<ebmlMasterElementClass, ebmlMasterElement> {
    //     // Constructors
    // protected:
    //     ebmlMasterElement(const ebmlMasterElementClass*);
    //     void _init(const parseString&);
    //     void _init(const parseFile&);
    //     void _cinit(const parseString&);
    //     void _cinit(const parseFile&);
    //
    //     ebmlElement* _decodeChild(const parseString&) const;
    //     ebmlElement* _decodeChild(const parseFile&) const;
    //
    //     ebmlElement* _cdecodeChild(const parseString&) const;
    //     ebmlElement* _cdecodeChild(const parseFile&) const;
    //
    //     // Reminder: This method should be overriden in every subclass
    // public:
    //     // virtual const ebmlMasterElementClass* cls() const;
    //
    //     // dataSize override
    //     size_t dataSize() const;
    //
    //     // Size Tree methods
    // public:
    //     sizetree_t sizetree() const;
    //
    // // protected:
    // //     sizetree_t _build_sizetree() const;
    // //     void _build_sizetree(sizetree_t&) const;
    //
    //     // Encode functions.
    // protected:
    //     // NEW in ebmlMasterElement. Protected _encode that makes use of a sizetree_t
    //     // to avoid redundant calls to outerSize() on all of its decendants.
    //     size_t _encode(char*, const sizetree_t&) const;
    //     size_t _encode(char*) const;
    //     // size_t _encode(char*, size_t) const;
    //
    // public:
    //     // Override the default encode to make use of _encode(char*, const sizetree_t&) instead of _encode(char*);
    //     std::string encode() const;
    //     size_t encode(char*) const;
    //     size_t encode(char*, const sizetree_t&) const;
    //     size_t encode(ioBase&) const;
    //     // size_t encode(ioBase&, const sizetree_t&) const;
    //     size_t encode(ioBase&, off_t) const;
    //     // size_t encode(ioBase&, off_t, const sizetree_t&) const;
    //
    // protected:
    //     virtual void _clear(); // Clear all children.
    //     virtual void _decodeChildren(parseString::iterator&);
    //     virtual void _cdecodeChildren(parseString::iterator&);
    //     virtual void _scanChildren(parseFile::iterator&); // Decode children from an iterParseFile instance created by _decode.
    //     virtual void _handleParseFile(const parseFile&); // Controls the handling of seek data pointing to a child element.
    //     virtual void _cscanChildren(parseFile::iterator&); // Decode children from an iterParseFile instance created by _decode.
    //     virtual void _chandleParseFile(const parseFile&); // Controls the handling of seek data pointing to a child element.
    //     virtual void _addChild(const ebmlElement_sp&) = 0;
    //     virtual void _addChild(ebmlElement_sp&&) = 0;
    //
    //     // Iteration
    // public:
    //     class iterator;
    //
    // protected:
    //     class _iterator {
    //     public:
    //         virtual _iterator* copy() const = 0;
    //
    //         virtual ebmlElement& operator*() const = 0;
    //         virtual _iterator& operator++() = 0;
    //
    //         virtual _iterator& operator=(const _iterator&) = 0;
    //
    //         virtual bool operator==(const _iterator&) const = 0;
    //         virtual bool operator!=(const _iterator&) const = 0;
    //
    //         virtual ~_iterator() {};
    //
    //         friend class ebmlMasterElement;
    //         friend class ebmlMasterElement::iterator;
    //     };
    //
    //     virtual ebmlMasterElement::_iterator* _begin() = 0;
    //     virtual ebmlMasterElement::_iterator* _end() = 0;
    //
    // public:
    //     class iterator {
    //     private:
    //         // std::shared_ptr<_iterator> _iter;
    //         // iterator(const std::shared_ptr<_iterator>&);
    //
    //         _iterator* _iter;
    //
    //     protected:
    //         iterator(_iterator*);
    //
    //     public:
    //         ebmlElement& operator*() const;
    //         iterator& operator++();
    //
    //         iterator(const iterator&);
    //         iterator(iterator&&);
    //         iterator& operator=(const iterator&);
    //         iterator& operator=(iterator&&);
    //
    //         bool operator==(const iterator&) const;
    //         bool operator!=(const iterator&) const;
    //
    //         ~iterator();
    //
    //         friend class ebmlMasterElement;
    //     };
    //
    //     iterator begin();
    //     iterator end();
    //
    //     // Const Iteration
    // public:
    //     class const_iterator;
    //
    // protected:
    //     class _const_iterator {
    //     public:
    //         virtual _const_iterator* copy() const = 0;
    //         virtual const ebmlElement& operator*() const = 0;
    //         virtual _const_iterator& operator++() = 0;
    //         virtual _const_iterator& operator=(const _const_iterator&) = 0;
    //         virtual bool operator==(const _const_iterator&) const = 0;
    //         virtual bool operator!=(const _const_iterator&) const = 0;
    //         virtual ~_const_iterator() {};
    //
    //         friend class ebmlMasterElement;
    //         friend class ebmlMasterElement::const_iterator;
    //     };
    //
    //     virtual ebmlMasterElement::_const_iterator* _cbegin() const = 0;
    //     virtual ebmlMasterElement::_const_iterator* _cend() const = 0;
    //
    // public:
    //     class const_iterator {
    //     private:
    //         _const_iterator* _iter;
    //         const_iterator(_const_iterator*);
    //
    //     public:
    //         virtual const ebmlElement& operator*() const;
    //         virtual const_iterator& operator++();
    //
    //         const_iterator(const const_iterator&);
    //         const_iterator(const_iterator&&);
    //         virtual const_iterator& operator=(const const_iterator&);
    //         virtual const_iterator& operator=(const_iterator&&);
    //
    //         virtual bool operator==(const const_iterator&) const;
    //         virtual bool operator!=(const const_iterator&) const;
    //
    //         ~const_iterator();
    //
    //         friend class ebmlMasterElement;
    //     };
    //
    //     const_iterator cbegin() const;
    //     const_iterator cend() const;
    //
    // protected:
    //     static iterator make_iter(_iterator*);
    //     static const_iterator make_iter(_const_iterator*);
    //
    //     // Cloning functions:
    // protected:
    //     void _clonedata(const ebmlElement*);
    //
    // protected:
    //     occur_d ebmlIDCount;
    //     void _attachChild(const ebmlElement_sp& child, bool weak = true);
    //     void _attachChild(const ebmlElement_sp& child, bool weak = true) const;
    //     void _detachChild(const ebmlElement_sp& child) const;
    //
    //     void _attachChild(ebmlElement& child, bool weak = true);
    //     void _attachChild(ebmlElement& child, bool weak = true) const;
    //     void _detachChild(ebmlElement& child) const;
    //
    //     void _attachChildren(const ebmlElement_l& elems, bool weak=true);
    //     void _attachChildren(const ebmlElement_l& elems, bool weak=true) const;
    //     void _detachChildren(const ebmlElement_l& elems) const;
    //
    // public:
    //     friend class ebmlMasterElementClass;
    //     friend class std::shared_ptr<ebmlMasterElement::_iterator>;
    //     friend class std::shared_ptr<ebmlMasterElement::_const_iterator>;
    //
    //     template<typename T>
    //     friend class childSlot_t;
    //
    //     friend std::shared_ptr<ebmlMasterElement::_iterator> std::make_shared<ebmlMasterElement::_iterator>();
    //     friend std::shared_ptr<ebmlMasterElement::_const_iterator> std::make_shared<ebmlMasterElement::_const_iterator>();
    // };

    inline ebmlElement* ebmlMasterElementClass::_decodeChild(const parseString& parsed) const {
        const ebmlElementClass* childcls;

        try {
            childcls = this->_childClasses[parsed.ebmlID];
        } catch (ebmlNoChildMatch& e) {
            e.erroroffset = parsed.offset;
            throw;
        }

        return childcls->_decode_nocheck(parsed);
    }

    inline ebmlElement* ebmlMasterElementClass::_cdecodeChild(const parseString& parsed) const {
        const ebmlElementClass* childcls;

        try {
            childcls = this->_childClasses[parsed.ebmlID];
        } catch (ebmlNoChildMatch& e) {
            e.erroroffset = parsed.offset;
            throw;
        }

        return childcls->_decode_nocheck(parsed);
    }

    inline ebmlElement* ebmlMasterElementClass::_decodeChild(const parseFile& parsed) const {
        const ebmlElementClass* childcls;

        try {
            childcls = this->_childClasses[parsed.ebmlID];
        } catch (ebmlNoChildMatch& e) {
            e.erroroffset = parsed.offset;
            throw;
        }

        return childcls->_decode_nocheck(parsed);
    }

    inline ebmlElement* ebmlMasterElementClass::_cdecodeChild(const parseFile& parsed) const {
        const ebmlElementClass* childcls;

        try {
            childcls = this->_childClasses[parsed.ebmlID];
        } catch (ebmlNoChildMatch& e) {
            e.offset = parsed.offset;
            throw;
        }

        // return childcls->mcdecode(parsed);
        return childcls->_decode_nocheck(parsed);
    }

    extern template class ClsMixin<ebmlMasterElementClass, ebmlMasterElement, ebmlElementClass>;
}
#endif
