/**
 * @file ebmlElement.h
 * @brief Declaration of the EBML element instance base class and CRTP templates.
 *
 * This header defines the abstract base class used to describe EBML element instances.
 *
 * @author Brian Sherson
 * @date September 11, 2025
 */

#ifndef EBML_NG_ELEMENT_H
#define EBML_NG_ELEMENT_H

#include "ptrs.h"
#include "parsing/string.h"
#include "parsing/io.h"
#include "flexptr.h"

namespace ebml {
    extern template class flexible_ptr<ebmlElement>;
    typedef flexible_ptr<ebmlElement> ebmlElement_fp;

    extern template class flexible_ptr<ebmlMasterElement>;
    typedef flexible_ptr<ebmlMasterElement> ebmlMasterElement_fp;

    /**
     * @defgroup inst_definitions EBML Element Instance Definitions
     * @brief Classes that form the basis for all EBML element instances.
     *
     * These classes are the functional components that hold the data and manage the
     * hierarchical relationships within an EBML document. They represent the concrete
     * elements that are created and manipulated.
     *
     * @see @ref type_definitions
     */

    /**
     * @defgroup inst_templates EBML Element Instance Templates
     * @brief Template implementations of the `ebml::ebmlElement` abstract base class.
     *
     * This group contains the template classes that are specialized with a data type
     * to create a concrete EBML element instance. These classes directly hold the
     * element's value and are the functional objects that are created and manipulated
     * by the library's users.
     *
     * @ingroup inst_definitions
     * @see @ref type_templates
     */

    /**
     * @defgroup inst_implementations EBML Element Instance Implementations
     * @brief Concrete implementations of the `ebml::ebmlElement` abstract base class.
     *
     * This group contains the final, non-templated classes that are ready for
     * direct use. They are typically instantiated by the factory methods in the
     * `ebmlElementType` classes and represent the final objects within a parsed
     * EBML document.
     *
     * @ingroup inst_definitions
     * @see @ref type_implementations
     */

    /**
     * @class ebmlElement
     * @brief Abstract base class for EBML Element instances.
     *
     * Each EBML element instance corresponds to an entry in an EBML file or data stream.
     * This abstract class defines the interface for encoding/decoding, cloning,
     * accessing parent/child relationships, and obtaining size and location information.
     *
     * The class also employs shared and weak pointer semantics tailored for EBML operations.
     *
     * @ingroup inst_definitions
     *
     * @see type_definitions
     * @see `ebml::ebmlElementCRTP<ebmltype_t, ebmlinst_t, instbase_t>`
     * @see ebml::ebmlElementType
     */
    class ebmlElement {
        friend class ebmlDocument;
        friend class ebmlElementType;
        friend class ebmlMasterElement;
        friend class ebmlLazyLoad;
        // friend class ebmlSchema;

        template<typename T>
        friend class ebmlDataElementClass;

        // Constructor/Destructor
    protected:
        /**
         * @brief Protected constructor.
         * @param cls Pointer to the companion `ebml::ebmlElementType` instance describing this element type.
         *
         * This constructor can only be called by friend classes (typically the companion element type).
         */
        ebmlElement(const ebmlElementType*);
        void _validate();

    public:
        virtual ~ebmlElement();

    protected:
        /// Pointer to the companion element type (an ebmlElementClass subclass).
        const ebmlElementType* _cls;
        /// Custom weak pointer to self (used for shared-from-this semantics).
        wptr<ebmlElement> _self;

    public:
        /**
         * @brief Returns a reference to the associated element class.
         *
         * This function typically performs a dynamic_cast to obtain a more specific
         * subclass based on the element’s actual type.
         *
         * @return Reference to the specific ebmlElementType.
         * @throws std::bad_cast If the dynamic_cast fails.
         */
        virtual const ebmlElementType& cls() const;

        /**
         * @brief Retrieve the EBML ID of the element.
         * @return EBML ID as an unsigned integer.
         */
        ebmlID_t ebmlID() const;

        /**
         * @brief Dynamically cast the element to the requested type.
         * @tparam T The desired element type (defaults to ebmlElement).
         * @return A reference to the element cast to type T.
         * @throws std::bad_cast if the dynamic_cast fails.
         */
        template<typename T=ebmlElement>
        inline T& as();

        /**
         * @brief Const version of the dynamic cast.
         * @tparam T The desired element type.
         * @return A reference to the element cast to type T.
         * @throws std::bad_cast if the dynamic_cast fails.
         */
        template<typename T=ebmlElement>
        inline T& as() const;

        /**
         * @brief Returns a shared pointer to the element instance.
         * @return The shared pointer version of this element.
         */
        ebml::ptr<ebmlElement> sp();

        /**
         * @brief Returns a shared pointer to a const element.
         * @return The shared pointer (const) version of this element.
         */
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

        // Hierarchy members.
    public:
        /**
         * @brief Check if the element's parent is a const object.
         * @return True if parent is a const pointer; otherwise false.
         */
        bool parent_is_const() const;

        /**
         * @brief Retrieve the parent of this element.
         * @return A shared pointer to the parent ebmlMasterElement.
         * @throws std::runtime_error If the parent is const.
         */
        ebml::ptr<ebmlMasterElement> parent() const; // Returns pointer to non-const parent. Throws ebmlException if parent_is_const() returns true.

        /**
         * @brief Retrieve a const shared pointer to the parent element.
         * @return A shared pointer to const ebmlMasterElement.
         */
        ebml::ptr<const ebmlMasterElement> c_parent() const; // Returns pointer to const parent.

    private:
        flexible_ptr<ebmlMasterElement> _parent;

    protected:
        void _setParent(const ebml::ptr<ebmlMasterElement>& parent, bool weak = true);
        void _setParent(const ebml::ptr<const ebmlMasterElement>& parent, bool weak = true);
        void _setParent(ebml::ptr<ebmlMasterElement>&& parent, bool weak = true);
        void _setParent(ebml::ptr<const ebmlMasterElement>&& parent, bool weak = true);

        void _setParent(ebmlMasterElement& parent, bool weak = true);
        void _setParent(const ebmlMasterElement& parent, bool weak = true);


        void _detach();

    public:
        ebmlElement_sp root() const;
        c_ebmlElement_sp c_root() const;

    protected:
        ebmlDocument_wp _document;

    public:
        ebmlDocument_sp document() const;

    public:
        bool hasParent() const;

        // Size functions.

    protected:
        vintWidth_t _sizeWidth = 0;

    public:
        unsigned char headSize() const;
        unsigned char headSize(size_t) const; // May be deprecated
        unsigned char sizeWidth() const;
        unsigned char sizeWidth(size_t) const; // May be deprecated
        size_t outerSize() const;
        size_t outerSize(size_t) const; // May be deprecated
        virtual size_t dataSize() const = 0;

        // Location attributes and methods.
        off_t offsetInParent() const;
        off_t offsetInFile() const;
        off_t dataOffsetInParent() const;
        off_t dataOffsetInFile() const;

    protected:
        off_t _offsetInParent = -1; // value of -1 indicates undefined.

        // Encode functions.
    protected:
        size_t _encode_head(char*, size_t) const;
        virtual size_t _encode(char*) const = 0;

    public:
        /**
         * @name Encode Functions
         * @brief Provides a set of overloaded methods for encoding an EBML element.
         *
         * This group of methods converts an EBML element instance into a raw byte
         * representation according to the EBML specification.
         *
         * @throws ebml::ebmlEncodeError if an internal encoding error occurs.
         * @throws std::ios_base::failure if an I/O error occurs when writing to a file or stream.
         * @{
         */
        inline std::string encode() const;      ///< @brief Encodes the element and returns it as a `std::string`.
        inline size_t encode(char*) const;      ///< @brief Encodes the element into a pre-allocated buffer.
        inline size_t encode(ioBase&) const;    ///< @brief Encodes the element and writes it to a generic I/O stream.
        inline size_t encode(ioBase&, off_t) const; ///< @brief Encodes the element and writes it to a specific offset in a stream.

        /**
         * @brief Encodes the element using a pre-computed size.
         *
         * This specialized overload takes a pre-computed size as an argument, which
         * avoids redundant `dataSize()` calls for master elements and their descendants,
         * significantly improving performance for deep trees.
         *
         * @param dest A pointer to the destination buffer where the encoded element will be written.
         * @param dataSize The pre-computed size of the element's data payload.
         * @return The total size, in bytes, of the encoded element, including its header.
         */
        size_t encode(char*, size_t) const;

        /**
         * @}
         */

        // Cloning functions:
    protected:
        // virtual void _clonedata(const ebmlElement*) = 0;
        virtual ebmlElement* _clone() const = 0;

    public:
        // virtual ebmlElement_sp clone() const;
        ebml::ptr<ebmlElement> clone() const;

        // Repr function:
        virtual std::wstring minirepr() const = 0;
        virtual std::wstring repr() const;

        seekData_t* makeSeekData();
        seekData_t* makeSeekData() const;
    };

    /**
     * @ingroup inst_definitions
     * @ingroup inst_templates
     * @class ebmlElementCRTP
     * @brief CRTP template for EBML Element instances.
     *
     * This template implements the Curiously Recurring Template Pattern (CRTP) to provide
     * type-safe behavior for EBML element instances. By inheriting from this template,
     * concrete element instance classes gain automated, type-safe casting and cloning
     * functionalities, which simplifies memory management and promotes a robust API.
     *
     * @tparam ebmltype_t The concrete element type class associated with this instance.
     * @tparam ebmlinst_t The concrete element instance class using this CRTP mixin.
     * @tparam instbase_t The base element instance class (default is `ebml::ebmlElement`).
     *
     * @see @ref crtp_usage "Using the CRTP Mixins" for detailed usage instructions.
     * @see `ebml::ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>`
     * @see ebml::ebmlElement
     */
    template<typename ebmltype_t, typename ebmlinst_t, typename instbase_t=ebmlElement>
    class ebmlElementCRTP : public instbase_t {
    protected:
        ebmlElement* _clone() const;

    public:
        using ebmltype = ebmltype_t;

        template<typename... Args>
        ebmlElementCRTP(const ebmltype_t* cls, Args&&... args) : instbase_t(cls, std::forward<Args>(args)...) {}
        const ebmltype_t& cls() const;

        using ebmlElement::sp;
        ebml::ptr<ebmlinst_t> sp();
        ebml::ptr<const ebmlinst_t> sp() const;

        using ebmlElement::wp;
        ebml::wptr<ebmlinst_t> wp();
        ebml::wptr<const ebmlinst_t> wp() const;

        ebml::ptr<ebmlinst_t> clone() const;
    };

    template<typename ebmlinst_t, typename parse_t>
    concept has_init = requires(ebmlinst_t& instance, const parse_t& parsed) {
        { instance._init(parsed) };
    };

    template<typename ebmlinst_t, typename parse_t>
    concept has_cinit = requires(ebmlinst_t& instance, const parse_t& parsed) {
        { instance._cinit(parsed) };
    };

    template<typename ebmlinst_t, typename... Args>
    concept has_constructor = requires(Args&&... args) {
        { new ebmlinst_t(std::forward<Args>(args)...) };
    };

    template<typename ebmlinst_t>
    concept has_clone_method = requires(ebmlinst_t& instance, ebmlinst_t& orig) {
        { instance._clone(orig) };
    };

    template<typename T>
    inline T& ebmlElement::as() {
        if (auto recast = dynamic_cast<T*>(this)) {
            return *recast;
        }

        throw std::bad_cast();
    }

    template<typename T>
    inline T& ebmlElement::as() const {
        if (auto recast = dynamic_cast<T*>(this)) {
            return *recast;
        }

        throw std::bad_cast();
    }

    template<typename T>
    ebml::wptr<T> ebmlElement::wp() {
        return ebml_dynamic_pointer_cast<T>(this->_self);
    }

    template<typename T>
    ebml::wptr<T> ebmlElement::wp() const {
        auto ptr = ebml_static_pointer_cast<const ebmlElement>(this->_self);
        return ebml_dynamic_pointer_cast<T>(std::move(ptr));
    }

    inline off_t ebmlElement::offsetInParent() const {
        return _offsetInParent;
    }

    inline void ebmlElement::_validate() {
        auto lock = std::unique_lock(_self.mutex);

        if (_self.ctl == nullptr) {
            _self.ctl = new control_block;
            _self.ctl->weakcount = 1;
            _self.ptr = this;
        }

        _self.ctl->valid = true;
    }

    inline size_t ebmlElement::encode(char* dest) const {
        size_t dataSize = this->dataSize();
        return ebmlElement::encode(dest, dataSize);
    }

    inline std::string ebmlElement::encode() const {
        std::string result;
        size_t dataSize = this->dataSize();
        result.resize(dataSize+16);
        size_t outerSize = this->encode(&result[0], dataSize);
        result.resize(outerSize);
        return result;
    }

    inline size_t ebmlElement::encode(ioBase& dest) const {
        size_t _dataSize = dataSize();
        auto buffer = std::make_unique<char[]>(_dataSize + 16);
        auto outerSize = encode(buffer.get(), _dataSize);
        return dest.write(buffer.get(), outerSize);
    }

    inline size_t ebmlElement::encode(ioBase& dest, off_t offset) const {
        size_t _dataSize = dataSize();
        auto buffer = std::make_unique<char[]>(_dataSize + 16);
        auto outerSize = encode(buffer.get(), _dataSize);
        return dest.write(buffer.get(), offset, outerSize);
    }
}

#endif
