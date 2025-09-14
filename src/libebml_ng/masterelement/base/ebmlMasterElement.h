#ifndef EBML_NG_MASTERELEMENT_H
#define EBML_NG_MASTERELEMENT_H

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
    protected:
        occur_d ebmlIDCount;
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
         * The default implementations of `ebml::ebmlElement::encode()` functions makes a call to the
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

        /**
         * @brief Encodes the element using a precomputed size tree.
         *
         * @param dest Pointer to the destination buffer.
         * @param _sizetree A precomputed sizetree_t for the element.
         * @return The number of bytes encoded.
         */
        size_t encode(char*, const sizetree_t&) const;

    protected:
        /**
         * @brief Clears all child elements.
         *
         * Removes all currently attached child elements from the master element.
         */
        virtual void _clear();

        /**
         * @brief Decodes child elements from a parseString iterator.
         *
         * Default implementation terates through the given parseString iterator to decode and attach child elements.
         *
         * @param iter Iterator over the parseString.
         */
        virtual void _decodeChildren(parseString::iterator&);

        /**
         * @brief Const version of _decodeChildren.
         *
         * Default implementation iterates through the given parseString iterator to decode and attach child elements
         * in decoding to a const `ebml::ebmlMasterElement` instance.
         *
         * @param iter Iterator over the parseString.
         */
        virtual void _cdecodeChildren(parseString::iterator&);

        /**
         * @brief Scans for child elements from a parseFile iterator.
         *
         * Default implementation iterates over the `ebml::parseFile::iterator` and passes each dereferenced `ebml::parseFile` to
         * `_handleParseFile` in decoding to an `ebml::ebmlMasterElement` instance.
         *
         * @param iter Iterator over the parseFile.
         */
        virtual void _scanChildren(parseFile::iterator&);

        /**
         * @brief Scans for child elements from a parseFile iterator. (const version)
         *
         * Default implementation iterates over the `ebml::parseFile::iterator` and passes each dereferenced `ebml::parseFile` to
         * `_chandleParseFile` in decoding to a const `ebml::ebmlMasterElement` instance.
         *
         * @param iter Iterator over the parseFile.
         */
        virtual void _cscanChildren(parseFile::iterator&);

        /**
         * @brief Handles the parsing of seek data for a child element.
         *
         * Processes the parsed file data corresponding to a child element. Default implementation decodes child and attaches it
         * through _addChild.
         *
         * @param parsed Parsed file data for a child element.
         */
        virtual void _handleParseFile(const parseFile&);

        /**
         * @brief Handles the parsing of seek data for a child element. (const version)
         *
         * Processes the parsed file data corresponding to a child element. Default implementation decodes child and attaches it
         * through _addChild.
         *
         * @param parsed Parsed file data for a child element.
         */
        virtual void _chandleParseFile(const parseFile&);

        /**
         * @brief Attaches a child element (rvalue version).
         *
         * Implements the mechanism for attaching a child element to the parent element.
         *
         * @param child Shared pointer to the child element.
         */
        virtual void _addChild(const ebmlElement_sp&) = 0;

        /**
         * @brief Attaches a child element (move version).
         *
         * Implements the mechanism for moving a child element to the parent element.
         *
         * @param child Rvalue reference to a shared pointer for the child element.
         */
        virtual void _addChild(ebmlElement_sp&&) = 0;

        /**
         * @brief Attaches a child element to the master element.
         *
         * Manages the parent-child relationship by attaching the provided child element,
         * with an option for a weak reference.
         *
         * @param child Shared pointer to the child element.
         * @param weak If true, attaches the child as a weak reference.
         */
        void _attachChild(const ebmlElement_sp& child, bool weak = true) const;

        /**
        * @brief Detaches a child element.
        *
        * Removes the association between the master element and the specified child element.
        *
        * @param child Shared pointer to the child element to detach.
        */
        void _detachChild(const ebmlElement_sp& child) const;

        /**
        * @brief Const version of _attachChildren.
        *
        * Attaches multiple child elements in a const context.
        *
        * @param elems List of child elements.
        * @param weak If true, attaches children as weak references.
        */
        void _attachChildren(const ebmlElement_l& elems, bool weak=true) const;

        /**
        * @brief Detaches multiple child elements.
        *
        * Iterates through the provided list and detaches each child element from the master element.
        *
        * @param elems List of child elements to detach.
        */
        void _detachChildren(const ebmlElement_l& elems) const;

        // Iteration
    public:
        class iterator;

        // Abstract base iterator class to be implemented for each different subclass of ebmlMasterElement.
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

        // Public-facing ebml::ebmlMasterElement::iterator that wraps around a pointer to ebml::ebmlMasterElement::_iterator
    public:
        class iterator {
        private:
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
