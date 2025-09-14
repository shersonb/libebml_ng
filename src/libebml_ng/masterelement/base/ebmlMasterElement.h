#ifndef EBML_NG_MASTERELEMENT_H
#define EBML_NG_MASTERELEMENT_H

// #include "../ebmlElementType.h"
#include "ebmlMasterElementType.h"
#include "../../ebmlElement.h"
#include "c_ebmlElement_l.h"

namespace ebml {
    /**
     * @brief Abstract base class for EBML master element instances.
     * @ingroup inst_definitions
     * @ingroup master_element_type_definitions
     *
     * This class provides the functionality to manage child elements, perform encoding/decoding,
     * and traverse child elements through iterators. It uses CRTP to enable type safety.
     *
     * @see ebml::ebmlMasterElementType
     */
    class ebmlMasterElement : public ebmlElementCRTP<ebmlMasterElementType, ebmlMasterElement> {
        // Constructors
    protected:
        ebmlMasterElement(const ebmlMasterElementType*);

        /**
         * @brief Initializes the element from a parsed EBML string.
         *
         * @param parsed The parsed EBML string.
         */
        void _init(const parseString&);

        /**
         * @brief Initializes the element from parsed file data.
         *
         * @param parsed The parsed file data.
         */
        void _init(const parseFile&);

        /**
         * @brief Const initialization from a parsed EBML string.
         *
         * @param parsed The parsed EBML string.
         */
        void _cinit(const parseString&);

        /**
         * @brief Const initialization from parsed file data.
         *
         * @param parsed The parsed file data.
         */
        void _cinit(const parseFile&);

        /**
         * @brief Decodes a child element from a parsed string.
         *
         * @param parsed The parsed EBML string.
         * @return A pointer to the decoded EBML element.
         */
        ebmlElement* _decodeChild(const parseString&) const;

        /**
         * @brief Decodes a child element from parsed file data.
         *
         * @param parsed The parsed file data.
         * @return A pointer to the decoded EBML element.
         */
        ebmlElement* _decodeChild(const parseFile&) const;

        /**
         * @brief Const version: Decodes a child element from a parsed string.
         *
         * @param parsed The parsed EBML string.
         * @return A pointer to the decoded EBML element.
         */
        ebmlElement* _cdecodeChild(const parseString&) const;

        /**
         * @brief Const version: Decodes a child element from parsed file data.
         *
         * @param parsed The parsed file data.
         * @return A pointer to the decoded EBML element.
         */
        ebmlElement* _cdecodeChild(const parseFile&) const;

    public:
        size_t dataSize() const;

    public:
        /**
         * @brief Generates the size tree for the element hierarchy.
         *
         * @return A sizetree_t representing the size breakdown of the element and its descendants.
         */
        sizetree_t sizetree() const;

    protected:
        size_t _encode(char*) const;

        // NEW in ebmlMasterElement. Protected _encode that makes use of a sizetree_t
        // to avoid redundant calls to outerSize() on all of its decendants.
        /**
         * @brief Encodes the element using a precomputed size tree.
         *
         * @param dest Pointer to the destination buffer.
         * @param _sizetree A precomputed sizetree_t for the element.
         * @return The number of bytes encoded.
         */
        size_t _encode(char*, const sizetree_t&) const;

    public:
        /**
         * @name Element Encoding Overloads
         * @brief Provides a set of overloaded methods for encoding an EBML master element.
         * The default implementations of `encode()` functions makes a call to the
         * `dataSize()` function, determining the entire size of the element without any
         * information on the size of its descendants. This results in redundant calls to
         * `dataSize()` on each of its descendants, with greater redundancy at higher depths.
         * This implementation instead computes a `sizetree_t` containing size information
         * for all the master element's descendants, thereby mitigating the redundancy.
         * @{
         */

        size_t encode(char*) const;
        std::string encode() const;
        size_t encode(ioBase&) const;
        size_t encode(ioBase&, off_t) const;
        /**
         * @}
         */

        size_t encode(char*, const sizetree_t&) const;

    protected:
        virtual void _clear(); // Clear all children.
        virtual void _decodeChildren(parseString::iterator&);
        virtual void _cdecodeChildren(parseString::iterator&);
        virtual void _scanChildren(parseFile::iterator&); // Decode children from an iterParseFile instance created by _decode.
        virtual void _handleParseFile(const parseFile&); // Controls the handling of seek data pointing to a child element.
        virtual void _cscanChildren(parseFile::iterator&); // Decode children from an iterParseFile instance created by _decode.
        virtual void _chandleParseFile(const parseFile&); // Controls the handling of seek data pointing to a child element.
        virtual void _addChild(const ebmlElement_sp&) = 0;
        virtual void _addChild(ebmlElement_sp&&) = 0;

        // Iteration
    public:
        class iterator;

    protected:
        class _iterator {
        public:
            virtual _iterator* copy() const = 0;

            virtual ebmlElement& operator*() const = 0;
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
            ebmlElement& operator*() const;
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
            virtual _const_iterator* copy() const = 0;
            virtual const ebmlElement& operator*() const = 0;
            virtual _const_iterator& operator++() = 0;
            virtual _const_iterator& operator=(const _const_iterator&) = 0;
            virtual bool operator==(const _const_iterator&) const = 0;
            virtual bool operator!=(const _const_iterator&) const = 0;
            virtual ~_const_iterator() {};

            friend class ebmlMasterElement;
            friend class ebmlMasterElement::const_iterator;
        };

        virtual ebmlMasterElement::_const_iterator* _cbegin() const = 0;
        virtual ebmlMasterElement::_const_iterator* _cend() const = 0;

    public:
        class const_iterator {
        private:
            _const_iterator* _iter;
            const_iterator(_const_iterator*);

        public:
            virtual const ebmlElement& operator*() const;
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
        void _attachChild(const ebmlElement_sp& child, bool weak = true) const;
        void _detachChild(const ebmlElement_sp& child) const;

        void _attachChild(ebmlElement& child, bool weak = true);
        void _attachChild(ebmlElement& child, bool weak = true) const;
        void _detachChild(ebmlElement& child) const;

        void _attachChildren(const ebmlElement_l& elems, bool weak=true);
        void _attachChildren(const ebmlElement_l& elems, bool weak=true) const;
        void _detachChildren(const ebmlElement_l& elems) const;

    public:
        friend class ebmlMasterElementType;
        friend class std::shared_ptr<ebmlMasterElement::_iterator>;
        friend class std::shared_ptr<ebmlMasterElement::_const_iterator>;

        template<typename T>
        friend class childSlot_t;

        friend std::shared_ptr<ebmlMasterElement::_iterator> std::make_shared<ebmlMasterElement::_iterator>();
        friend std::shared_ptr<ebmlMasterElement::_const_iterator> std::make_shared<ebmlMasterElement::_const_iterator>();
    };

    extern template class ebmlElementCRTP<ebmlMasterElementType, ebmlMasterElement, ebmlElement>;
    extern template class ebml_shared_ptr<ebmlMasterElement>;
    extern template class ebml_weak_ptr<ebmlMasterElement>;
}
#endif
