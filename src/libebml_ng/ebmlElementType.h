/**
 * @file ebmlElementType.h
 * @brief Declaration of the EBML element type base class and CRTP templates.
 *
 * This header defines the abstract base class used to describe EBML element types.
 * Every subclass of ebmlElementType represents a distinct EBML element type and must
 * have a companion ebmlElement (or subclass) declared as a friend. It provides functionality
 * for constructing element type instances based on an EBML ID (either as a VINT string or an unsigned integer)
 * and a name (as a wide string). The header also defines a CRTP template, ebmlTypeCRTP, that facilitates
 * creation, decoding, and type-safe casting of concrete element instances.
 *
 * @note Many member functions that perform trivial computations such as returning sizes or addresses
 * are declared inline.
 *
 * @author Brian Sherson
 * @date September 11, 2025
 */

#ifndef EBML_NG_ELEMENTCLS_H
#define EBML_NG_ELEMENTCLS_H

#include "ptrs.h"
#include "seekdata.h"
#include "parsing/string.h"
#include "parsing/io.h"
#include "exceptions.h"
#include "vint.h"
#include <utility>

#include "typeof.h"

namespace ebml {
    /**
     * @defgroup type_definitions EBML Element Type Definitions
     * @brief Classes that define the structure and properties of EBML element types.
     *
     * These classes act as singletons that provide metadata and factory
     * methods for creating and decoding specific types of EBML elements.
     *
     * @see @ref inst_definitions
     */

    /**
     * @defgroup type_templates EBML Element Type Templates
     * @brief Template implementations of the `ebml::ebmlElementType` abstract base class.
     *
     * This group contains the template classes that serve as blueprints for
     * creating concrete `ebmlElementType` specializations. These templates
     * define the shared behavior for different data types, such as integers,
     * strings, and floats, allowing for consistent implementation across the library.
     *
     * This allows for compile-time generation of concrete element types for
     * a given data type, but they are not considered concrete implementations
     * until they are instantiated with a specific type.
     *
     * @ingroup type_definitions
     * @see @ref inst_templates
     */

    /**
     * @defgroup type_implementations EBML Element Type Implementation
     * @brief Concrete implementations of the `ebml::ebmlElementType` abstract base class.
     * This group contains the classes that define the metadata and behavior
     * for specific EBML element types, such as signed integers, strings,
     * and master elements.
     *
     * @ingroup type_definitions
     * @see @ref inst_implementations
     */

    /**
     * @brief Abstract base class for EBML Element Type objects.
     *
     * Abstract base class for EBML Element Type objects. Every
     * subclass of ebmlElementType must have a companion subclass
     * of ebmlElement declared as a friend class.
     *
     * Instances of ebmlElementType are constructed using an
     * EBML ID, either as an ebmlID_t (unsigned long long) or as
     * a VINT (const char*), and a class name (unicode string).
     *
     * This class declares these abstract member functions:
     * - `ebmlElement* _new() const`,
     * - `ebmlElement* _decode_v(const parseString&) const`,
     * - `ebmlElement* _decode_v(const parseFile&) const`,
     * - `ebmlElement* _cdecode_v(const parseString&) const`, and
     * - `ebmlElement* _cdecode_v(const parseFile&) const`.
     *
     * If you are creating an element type that encapsulates a
     * basic data type T, consider deriving from
     * `ebml::ebmlDataType<T>` and `ebml::ebmlDataElement<T>`.
     *
     * If creating an element type that encapsulates other EBML
     * elements, please see `ebml::ebmlMasterElementType` and
     * `ebml::ebmlMasterElement`.
     *
     * If you wish to subclass this (and `ebml::ebmlElement`), consider
     * deriving directly from
     * `ebml::ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>` and
     * `ebml::ebmlElementCRTP<ebmltype_t, ebmlinst_t, instbase_t>`.
     *
     * @see `ebml::ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>`
     * @see ebml::ebmlElement
     * @ingroup type_definitions
     */
    class ebmlElementType {
        friend class ebmlElement;
        friend class ebmlMasterElementType;

    public:

        /**
         * @brief Constructor taking an EBML ID as a VINT string and an element name.
         * @param ebmlID VINT string representing the EBML ID.
         * @param name The name of the EBML element type.
         */
        inline ebmlElementType(const char* ebmlID, const std::wstring& name);
        inline ebmlElementType(const char* ebmlID, const std::wstring& name, const seekHelper_t* seekHelper);

        /**
         * @brief Constructor taking an EBML ID as an unsigned integer and an element name.
         * @param ebmlID The EBML ID value.
         * @param name The name of the EBML element type.
         */
        inline ebmlElementType(ebmlID_t ebmlID, const std::wstring& name);
        inline ebmlElementType(ebmlID_t ebmlID, const std::wstring& name, const seekHelper_t* seekHelper);

        /// EBML ID associated with this element type.
        ebmlID_t ebmlID;
        /// Name (as a wide string) of the element type.
        std::wstring name;

        /**
         * @brief Factory operator to create a default EBML element instance.
         *
         * Internally, this calls the protected _new() function.
         *
         * @return A shared pointer to a newly created ebmlElement instance.
         */
        inline ebml::ptr<ebmlElement> operator()() const;

    protected:
        /**
         * @brief Create a default instance of an EBML element.
         * @return Pointer to a new ebmlElement instance.
         * @note Must be implemented by derived classes.
         */
        virtual ebmlElement* _new() const = 0;

        /**
         * @name Decode Functions (virtual)
         * These functions perform the decoding assuming
         * preliminary EBML ID checks have passed.
         * @param parsed The parsed EBML string (`ebml::parseString`)
         *     or file data (`ebml::parseFile`).
         * @return Raw pointer to a new ebmlElement instance.
         * @note Must be implemented by derived classes.
         * @{
         */

        virtual ebmlElement* _decode_v(const parseString&) const = 0;
        virtual ebmlElement* _decode_v(const parseFile&) const = 0;
        virtual ebmlElement* _cdecode_v(const parseString&) const = 0;
        virtual ebmlElement* _cdecode_v(const parseFile&) const = 0;

        /**
         * @}
         */

        /**
         * @name Decode Functions (No Check)
         * These functions wrap around the virtual `_decode_v` functions,
         * modifying and rethrowing any `ebml::ebmlDecodeError` exception
         * thrown with EBML type and offset data set.
         *
         * These functions can be used by friend classes if a manual EBML type
         * check is performed before decoding (as is done with
         * `ebml::ebmlMasterElementType`).
         *
         * @{
         */

        /**
         * @brief Decode element from `ebml::parseString` instance.
         * @param parsed The parsed EBML string.
         *
         * @return Raw pointer to a new ebmlElement instance.
         */
        inline ebmlElement* _decode_nocheck(const parseString& parsed) const;

        /**
         * @brief Decode element from `ebml::parseFile` instance.
         * @param parsed The parsed file data.
         *
         * @return Raw pointer to a new ebmlElement instance.
         */
        inline ebmlElement* _decode_nocheck(const parseFile& parsed) const;

        inline ebmlElement* _cdecode_nocheck(const parseString& parsed) const;
        inline ebmlElement* _cdecode_nocheck(const parseFile& parsed) const;
        /**
         * @}
         */

        /**
         * @name Protected Decode Functions
         * @brief Decode functions for various input types.
         *
         * These functions perform EBML type checks and call `_decode_nocheck`.
         * @return Raw pointer to a new `ebml::ebmlElement` instance.
         * @{
         */

        /**
         * @brief Decode element from `ebml::parseString` instance.
         * @param parsed The parsed EBML string.
         *
         * @return Raw pointer to a new `ebml::ebmlElement` instance.
         */
        inline ebmlElement* _decode(const parseString&) const;

        /**
         * @brief Decode and create a new EBML element instance from `ebml::parseFile` instance.
         * @param parsed Parsing information parsed from an IO file.
         *
         * @return Raw pointer to a new `ebml::ebmlElement` instance.
         */
        inline ebmlElement* _decode(const parseFile&) const;

        /**
         * @brief Decode from a raw data buffer.
         * @param data Pointer to the data buffer.
         * @param size Number of bytes in the data buffer..
         *
         * @return Pointer to a new `ebml::ebmlElement` instance.
         */
        inline ebmlElement* _decode(const char*, size_t) const;

        /**
         * @brief Decode from `std::string`.
         * @param data String data.
         *
         * @return Pointer to a new `ebml::ebmlElement` instance.
         */
        inline ebmlElement* _decode(const std::string&) const;

        /**
         * @brief Decode and create a new EBML element instance from
         *  `ebml::ioBase` instance at the current file position.
         * @param file File object.
         *
         * @return Pointer to a new `ebml::ebmlElement` instance.
         */
        inline ebmlElement* _decode(ioBase&) const;

        /**
         * @brief Decode and create a new EBML element instance from
         *  `ebml::ioBase` instance with a specified offset.
         * @param file File object.
         * @param offset Location of start of data in file.
         *
         * @return Pointer to a new `ebml::ebmlElement` instance.
         */
        inline ebmlElement* _decode(ioBase&, off_t&) const;

        /**
         * @brief Decode from `ebml::ioBase` instance with a specified offset and store the end offset.
         * @param file File object.
         * @param offset Location of start of data in file.
         * @param endOffset Reference to variable to set end offset.
         *
         * @return Pointer to a new `ebml::ebmlElement` instance.
         */
        inline ebmlElement* _decode(ioBase&, off_t, off_t&) const;

        inline ebmlElement* _cdecode(const parseString&) const;
        inline ebmlElement* _cdecode(const parseFile&) const;
        inline ebmlElement* _cdecode(const char*, size_t) const;
        inline ebmlElement* _cdecode(const std::string&) const;
        inline ebmlElement* _cdecode(ioBase&) const;
        inline ebmlElement* _cdecode(ioBase&, off_t& offset) const;
        inline ebmlElement* _cdecode(ioBase&, off_t offset, off_t& end) const;

        /**
         * @}
         */

    public:
        /**
         * @name Decode Functions
         * @brief Decode functions for various input types.
         * @return A shared pointer to the new `ebml::ebmlElement` instance.
         * @throws ebml::ebmlDecodeError If there was an error decoding the data (may be subclass).
         * @throws ebml::ebmlNoMatch If the EBML ID does not match.
         * @throws ebml::ebmlUnexpectedEndOfData If the data terminates before end of data (in either).
         * @throws ebml::ebmlDataContinues If the data continues past expected end.
         * @throws std::ios_base::failure If an IO error occurs while attempting to read from a file.
         * @{
         */

        /**
         * @brief Decode element from `ebml::parseString` instance.
         * @param parsed The parsed EBML string.
         *
         * @return Shared pointer to a new `ebml::ebmlElement` instance.
         */
        inline ebml::ptr<ebmlElement> decode(const parseString&) const;

        /**
         * @brief Decode and create a new EBML element instance from `ebml::parseFile` instance.
         * @param parsed Parsing information parsed from an IO file.
         *
         * @return Shared pointer to a new `ebml::ebmlElement` instance.
         */
        inline ebml::ptr<ebmlElement> decode(const parseFile&) const;

        /**
         * @brief Decode and create a new EBML element instance from a raw data buffer.
         * @param data Pointer to the data buffer.
         * @param size Number of bytes in the data buffer.
         *
         * @return Shared pointer to a new `ebml::ebmlElement` instance.
         */
        inline ebml::ptr<ebmlElement> decode(const char*, size_t) const;

        /**
         * @brief Decode and create a new EBML element instance from a `std::string` instance.
         * @param data String data.
         *
         * @return Shared pointer to a new `ebml::ebmlElement` instance.
         */
        inline ebml::ptr<ebmlElement> decode(const std::string&) const;

        /**
         * @brief Decode and create a new EBML element instance from
         *  `ebml::ioBase` instance at the current file position.
         * @param file File object.
         *
         * @return Shared pointer to a new `ebml::ebmlElement` instance.
         */
        inline ebml::ptr<ebmlElement> decode(ioBase&) const;

        /**
         * @brief Decode and create a new EBML element instance from
         *  `ebml::ioBase` instance with a specified offset.
         * @param file File object.
         * @param offset Location of start of data in file.
         *
         * @return Shared pointer to a new `ebml::ebmlElement` instance.
         */
        inline ebml::ptr<ebmlElement> decode(ioBase&, off_t&) const;

        /**
         * @brief Decode from `ebml::ioBase` instance with a specified offset and store the end offset.
         * @param file File object.
         * @param offset Location of start of data in file.
         * @param endOffset Reference to variable to set end offset.
         *
         * @return Shared pointer to a new `ebml::ebmlElement` instance.
         */
        inline ebml::ptr<ebmlElement> decode(ioBase&, off_t, off_t&) const;

        inline ebml::ptr<const ebmlElement> cdecode(const parseString&) const;
        inline ebml::ptr<const ebmlElement> cdecode(const parseFile&) const;
        inline ebml::ptr<const ebmlElement> cdecode(const char*, size_t) const;
        inline ebml::ptr<const ebmlElement> cdecode(const std::string&) const;
        inline ebml::ptr<const ebmlElement> cdecode(ioBase&) const;
        inline ebml::ptr<const ebmlElement> cdecode(ioBase&, off_t& offset) const;
        inline ebml::ptr<const ebmlElement> cdecode(ioBase&, off_t offset, off_t& end) const;
        /**
         * @}
         */

        const seekHelper_t* _seekHelper = &seekHelper;

    public:
        seekMapBase* initSeekMap() const;
        seekData_t* makeSeekData(const parseString&) const;
        seekData_t* makeSeekData(const parseString&, const ebmlMasterElement_wp&) const;
        seekData_t* makeSeekData(const parseString&, ebmlMasterElement_wp&&) const;
        seekData_t* makeSeekData(const parseFile&) const;
        seekData_t* makeSeekData(const parseFile&, const ebmlMasterElement_wp&) const;
        seekData_t* makeSeekData(const parseFile&, ebmlMasterElement_wp&&) const;

    public:
        /**
         * Convenience member function that dynamically casts
         * ebmlElementType to a subclass.
         *
         * @return Reference to instance as a T object.
         *
         * Example:
         * @code
         * ebmlElementType cls = ebmlVoidClass();
         * ebmlVoidClass& Void = cls.as<ebmlVoidClass>();
         * @endcode
         * @throws std::bad_cast If instance is not an instance of T.
         */

        template<typename T=ebmlElementType>
        inline T& as();

        /**
         * Convenience member function that dynamically casts
         * const ebmlElementType to a subclass.
         *
         * @return Const reference to instance as a T object.
         *
         * Example:
         * @code
         * const ebmlElementType cls = ebmlVoidClass();
         * const ebmlVoidClass& Void = cls.as<ebmlVoidClass>();
         * @endcode
         * @throws std::bad_cast If instance is not an instance of T.
         */

        template<typename T=ebmlElementType>
        inline T& as() const;

        // Repr function
        virtual std::wstring minirepr() const;
        std::wstring repr() const;
    };

    /**
     * @brief CRTP template for EBML Element Type objects.
     *
     * This template implements the Curiously Recurring Template Pattern (CRTP) to enhance
     * EBML element type classes. By wrapping their `decode` and `cdecode` functions,
     * it provides type-safe pointer conversions, eliminating the need for manual recasting.
     *
     * ## Benefits and Design Rationale
     *
     * By inheriting from `ebmlTypeCRTP`, derived EBML element type classes receive:
     * - Type-safe instance creation through `operator()(Args&&...)`
     * - Automatic conversion of pointer types in decoding functions, returning:
     * - `ebml::ptr<ebmlinst_t>` for mutable instances
     * - `ebml::ptr<const ebmlinst_t>` for const instances
     *
     * @see @ref crtp_usage "Using the CRTP Mixins" for detailed usage instructions.
     * @see `ebml::ebmlElementCRTP<ebmltype_t, ebmlinst_t, instbase_t>`
     * @see ebml::ebmlElementType
     *
     * @tparam ebmltype_t The concrete element type class using this CRTP mixin.
     * @tparam ebmlinst_t The associated concrete element instance class.
     * @tparam typebase_t The base element type class (default is `ebml::ebmlElementType`).
     *
     * @ingroup type_definitions
     * @ingroup type_templates
     * @author Brian Sherson
     * @date 9/12/2025
     */
    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t=ebmlElementType>
    class ebmlTypeCRTP : public typebase_t {
        using inst_t = ebmlinst_t;
    protected:
        using typebase_t::_decode;
        ebmlElement* _new() const;
        ebmlElement* _decode_v(const parseString&) const;
        ebmlElement* _decode_v(const parseFile&) const;

        ebmlElement* _cdecode_v(const parseString&) const;
        ebmlElement* _cdecode_v(const parseFile&) const;

    public:
        template<typename... Args>
        inline ebmlTypeCRTP(Args&&... args) : typebase_t(std::forward<Args>(args)...) {}

        /**
         * @brief Creates a new instance of an EBML element.
         *
         * This operator wraps around the constructor of `ebmlinst_t` of the form
         * `ebmlinst_t(const ebmltype_t*, Args&&...)`, forwarding the provided arguments,
         * validating the created instance, and returning a shared pointer to the new element.
         *
         * @tparam ebmltype_t The concrete EBML element type class.
         * @tparam ebmlinst_t The concrete EBML element instance class.
         * @tparam typebase_t The base element type class.
         * @tparam Args Arguments to be forwarded to the constructor of ebmlinst_t.
         * @param args The arguments to forward.
         * @return ebml::ptr<ebmlinst_t> A shared pointer to the created EBML element instance.
         */
        template<typename... Args>
        inline ebml::ptr<ebmlinst_t> operator()(Args&&... args) const;

        inline ebml::ptr<ebmlinst_t> decode(const parseString& parsed) const;
        inline ebml::ptr<ebmlinst_t> decode(const parseFile& parsed) const;
        inline ebml::ptr<ebmlinst_t> decode(const char* data, size_t size) const;
        inline ebml::ptr<ebmlinst_t> decode(const std::string& data) const;
        inline ebml::ptr<ebmlinst_t> decode(ioBase& file) const;
        inline ebml::ptr<ebmlinst_t> decode(ioBase& file, off_t& offset) const;
        inline ebml::ptr<ebmlinst_t> decode(ioBase& file, off_t offset, off_t& endoffset) const;

        inline ebml::ptr<const ebmlinst_t> cdecode(const parseString& parsed) const;
        inline ebml::ptr<const ebmlinst_t> cdecode(const parseFile& parsed) const;
        inline ebml::ptr<const ebmlinst_t> cdecode(const char* data, size_t size) const;
        inline ebml::ptr<const ebmlinst_t> cdecode(const std::string& data) const;
        inline ebml::ptr<const ebmlinst_t> cdecode(ioBase& file) const;
        inline ebml::ptr<const ebmlinst_t> cdecode(ioBase& file, off_t& offset) const;
        inline ebml::ptr<const ebmlinst_t> cdecode(ioBase& file, off_t offset, off_t& endoffset) const;
    };

    ebml::ptr<ebmlElement> ebmlElementType::operator()() const {
        return ebml::ptr<ebmlElement>(this->_new());
    }

    inline ebmlElementType::ebmlElementType(const char* _ebmlID, const std::wstring& _name) : ebmlElementType(unpackVint(_ebmlID), _name) {}

    inline ebmlElementType::ebmlElementType(const char* _ebmlID, const std::wstring& _name, const seekHelper_t* seekHelper)
      : ebmlElementType(unpackVint(_ebmlID), _name, seekHelper) {}

    inline ebmlElementType::ebmlElementType(ebmlID_t _ebmlID, const std::wstring& _name) : ebmlID(_ebmlID), name(_name) {}

    inline ebmlElementType::ebmlElementType(ebmlID_t _ebmlID, const std::wstring& _name, const seekHelper_t* seekHelper)
      : ebmlID(_ebmlID), name(_name), _seekHelper(seekHelper) {}

    inline ebmlElement* ebmlElementType::_decode(const parseString& parsed) const {
        if (parsed.ebmlID != ebmlID) {
            throw ebmlNoMatch("EBML ID does not match.", this, 0, 0, 0);
        }

        ebmlElement* elem;

        try {
            elem = _decode_nocheck(parsed);

        } catch (ebmlDecodeError& e) {
            if (e.cls == nullptr) {
                e.cls = this;
                e.headSize = parsed.ebmlIDWidth + parsed.sizeWidth;
            }

            throw;
        }

        return elem;
    }

    inline ebml::ptr<ebmlElement> ebmlElementType::decode(const parseString& parsed) const {
        return _decode(parsed)->sp();
    }

    inline ebmlElement* ebmlElementType::_decode(const parseFile& parsed) const {
        if (parsed.ebmlID != this->ebmlID) {
            throw ebmlNoMatch("EBML ID does not match.", this, parsed.offset);
        }

        return _decode_nocheck(parsed);
    }

    inline ebml::ptr<ebmlElement> ebmlElementType::decode(const parseFile& parsed) const {
        return _decode(parsed)->sp();
    }

    inline ebmlElement* ebmlElementType::_decode(const char* data, size_t size) const {
        auto parsed = parseString(data, size);

        if (size > parsed.outerSize()) {
            throw ebmlDataContinues("Data continues past expected end.", this, 0, parsed.ebmlIDWidth + parsed.sizeWidth, parsed.dataSize);
        }

        return _decode(parsed);
    }

    inline ebml::ptr<ebmlElement> ebmlElementType::decode(const char* data, size_t size) const {
        return _decode(data, size)->sp();
    }

    ebmlElement* ebmlElementType::_decode(const std::string& data) const {
        return _decode(data.data(), data.size());
    }

    ebml::ptr<ebmlElement> ebmlElementType::decode(const std::string& data) const {
        return _decode(data)->sp();
    }

    // ebmlElement* ebmlElementType::_decode(const ioBase_sp& file) const {
    //     if (file == nullptr) {
    //         throw std::runtime_error("ebmlElementType::decode(const ioBase_sp&): nullptr detected.");
    //     }
    //
    //     return _decode(*file);
    // }

    // ebml::ptr<ebmlElement> ebmlElementType::decode(const ioBase_sp& file) const {
    //     return _decode(file)->sp();
    // }

    ebmlElement* ebmlElementType::_decode(ioBase& file) const {
        auto parsed = parseFile(file);
        // auto elem = this->decode(parsed);
        auto elem = _decode(parsed);
        file.seek(parsed.endOffset());
        return elem;
    }

    ebml::ptr<ebmlElement> ebmlElementType::decode(ioBase& file) const {
        return _decode(file)->sp();
    }

    // ebmlElement* ebmlElementType::_decode(const ioBase_sp& file, off_t& offset) const {
    //     if (file == nullptr) {
    //         throw std::runtime_error("ebmlElementType::decode(const ioBase_sp&, off_t&): nullptr detected.");
    //     }
    //
    //     return _decode(*file, offset);
    // }

    // ebml::ptr<ebmlElement> ebmlElementType::decode(const ioBase_sp& file, off_t& offset) const {
    //     return _decode(file, offset)->sp();
    // }

    ebmlElement* ebmlElementType::_decode(ioBase& file, off_t& offset) const {
        auto parsed = parseFile(file, offset);
        auto elem = _decode(parsed);
        offset = parsed.endOffset();
        return elem;
    }

    ebml::ptr<ebmlElement> ebmlElementType::decode(ioBase& file, off_t& offset) const {
        return _decode(file, offset)->sp();
    }

    // ebmlElement* ebmlElementType::_decode(const ioBase_sp& file, off_t offset, off_t& endOffset) const {
    //     if (file == nullptr) {
    //         throw std::runtime_error("ebmlElementType::decode(const ioBase_sp&, off_t&): nullptr detected.");
    //     }
    //
    //     return _decode(*file, offset, endOffset);
    // }

    // ebml::ptr<ebmlElement> ebmlElementType::decode(const ioBase_sp& file, off_t offset, off_t& endOffset) const {
    //     return _decode(file, offset, endOffset)->sp();
    // }

    ebmlElement* ebmlElementType::_decode(ioBase& file, off_t offset, off_t& endOffset) const {
        auto parsed = parseFile(file, offset);
        auto elem = _decode(parsed);
        endOffset = parsed.endOffset();
        return elem;
    }

    ebml::ptr<ebmlElement> ebmlElementType::decode(ioBase& file, off_t offset, off_t& endOffset) const {
        return _decode(file, offset, endOffset)->sp();
    }

    // CONST DECODING

    ebmlElement* ebmlElementType::_cdecode(const parseString& parsed) const {
        if (parsed.ebmlID != ebmlID) {
            throw ebmlNoMatch("EBML ID does not match.", this, 0, 0, 0);
        }

        ebmlElement* elem;

        try {
            elem = _cdecode_nocheck(parsed);

        } catch (ebmlDecodeError& e) {
            if (e.cls == nullptr) {
                e.cls = this;
                e.headSize = parsed.ebmlIDWidth + parsed.sizeWidth;
            }

            throw;
        }

        return elem;
    }


    ebml::ptr<const ebmlElement> ebmlElementType::cdecode(const parseString& parsed) const {
        const auto elem = _cdecode(parsed);
        return elem->sp();
    }

    ebmlElement* ebmlElementType::_cdecode(const parseFile& parsed) const {
        if (parsed.ebmlID != this->ebmlID) {
            throw ebmlNoMatch("EBML ID does not match.", this, parsed.offset);
        }

        return _cdecode_nocheck(parsed);
    }

    ebml::ptr<const ebmlElement> ebmlElementType::cdecode(const parseFile& parsed) const {
        const auto elem = _cdecode(parsed);
        return elem->sp();
    }

    ebmlElement* ebmlElementType::_cdecode(const char* data, size_t size) const {
        auto parsed = parseString(data, size);

        if (size > parsed.outerSize()) {
            throw ebmlDataContinues("Data continues past expected end.", this, 0, parsed.ebmlIDWidth + parsed.sizeWidth, parsed.dataSize);
        }

        return _cdecode(parsed);
    }

    ebml::ptr<const ebmlElement> ebmlElementType::cdecode(const char* data, size_t size) const {
        const auto elem = _cdecode(data, size);
        return elem->sp();
    }

    ebmlElement* ebmlElementType::_cdecode(const std::string& data) const {
        return _cdecode(data.data(), data.size());
    }

    ebml::ptr<const ebmlElement> ebmlElementType::cdecode(const std::string& data) const {
        const auto elem = _cdecode(data);
        return elem->sp();
    }

    // ebmlElement* ebmlElementType::_cdecode(const ioBase_sp& file) const {
    //     if (file == nullptr) {
    //         throw std::runtime_error("ebmlElementType::decode(const ioBase_sp&): nullptr detected.");
    //     }
    //
    //     return _cdecode(*file);
    // }

    // ebml::ptr<const ebmlElement> ebmlElementType::cdecode(const ioBase_sp& file) const {
    //     const auto elem = _cdecode(file);
    //     return elem->sp();eb
    // }

    ebmlElement* ebmlElementType::_cdecode(ioBase& file) const {
        auto parsed = parseFile(file);
        // auto elem = this->decode(parsed);
        auto elem = _cdecode(parsed);
        file.seek(parsed.endOffset());
        return elem;
    }

    ebml::ptr<const ebmlElement> ebmlElementType::cdecode(ioBase& file) const {
        const auto elem = _cdecode(file);
        return elem->sp();
    }

    // ebmlElement* ebmlElementType::_cdecode(const ioBase_sp& file, off_t& offset) const {
    //     if (file == nullptr) {
    //         throw std::runtime_error("ebmlElementType::decode(const ioBase_sp&, off_t&): nullptr detected.");
    //     }
    //
    //     return _cdecode(*file, offset);
    // }

    // ebml::ptr<const ebmlElement> ebmlElementType::cdecode(const ioBase_sp& file, off_t& offset) const {
    //     const auto elem = _cdecode(file, offset);
    //     return elem->sp();
    // }

    ebmlElement* ebmlElementType::_cdecode(ioBase& file, off_t& offset) const {
        auto parsed = parseFile(file, offset);
        auto elem = _cdecode(parsed);
        offset = parsed.endOffset();
        return elem;
    }

    ebml::ptr<const ebmlElement> ebmlElementType::cdecode(ioBase& file, off_t& offset) const {
        const auto elem = _cdecode(file, offset);
        return elem->sp();
    }

    // ebmlElement* ebmlElementType::_cdecode(const ioBase_sp& file, off_t offset, off_t& endOffset) const {
    //     if (file == nullptr) {eb
    //         throw std::runtime_error("ebmlElementType::decode(const ioBase_sp&, off_t&): nullptr detected.");
    //     }
    //
    //     return _cdecode(*file, offset, endOffset);
    // }

    // ebml::ptr<const ebmlElement> ebmlElementType::cdecode(const ioBase_sp& file, off_t offset, off_t& endOffset) const {
    //     const auto elem = _cdecode(file, offset, endOffset);
    //     return elem->sp();
    // }

    ebmlElement* ebmlElementType::_cdecode(ioBase& file, off_t offset, off_t& endOffset) const {
        auto parsed = parseFile(file, offset);
        auto elem = _cdecode(parsed);
        endOffset = parsed.endOffset();
        return elem;
    }

    ebml::ptr<const ebmlElement> ebmlElementType::cdecode(ioBase& file, off_t offset, off_t& endOffset) const {
        const auto elem = _cdecode(file, offset, endOffset);
        return elem->sp();
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::decode(const char* data, size_t size) const {
        return static_cast<ebmlinst_t*>(this->_decode(data, size))->sp();
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::decode(const std::string& data) const {
        return static_cast<ebmlinst_t*>(this->_decode(data))->sp();
    }

    inline ebmlElement* ebmlElementType::_decode_nocheck(const parseString& parsed) const {
        try {
            return _decode_v(parsed);

        } catch (ebmlDecodeError& e) {
            if (e.cls == nullptr) {
                e.cls = this;
                e.headSize = parsed.ebmlIDWidth + parsed.sizeWidth;
            }

            throw;
        }
    }

    inline ebmlElement* ebmlElementType::_decode_nocheck(const parseFile& parsed) const {
        ebmlElement* elem;

        try {
            elem = _decode_v(parsed);

        } catch (ebmlDecodeError& e) {
            if (e.cls == nullptr) {
                e.cls = this;
                e.headSize = parsed.ebmlIDWidth + parsed.sizeWidth;
            }

            throw;
        }

        elem->_offsetInParent = parsed.offset - parsed.parentOffset;
        return elem;
    }

    inline ebmlElement* ebmlElementType::_cdecode_nocheck(const parseString& parsed) const {
        try {
            return _cdecode_v(parsed);

        } catch (ebmlDecodeError& e) {
            if (e.cls == nullptr) {
                e.cls = this;
                e.headSize = parsed.ebmlIDWidth + parsed.sizeWidth;
            }

            throw;
        }
    }

    inline ebmlElement* ebmlElementType::_cdecode_nocheck(const parseFile& parsed) const {
        ebmlElement* elem;

        try {
            elem = _cdecode_v(parsed);

        } catch (ebmlDecodeError& e) {
            if (e.cls == nullptr) {
                e.cls = this;
                e.headSize = parsed.ebmlIDWidth + parsed.sizeWidth;
            }

            throw;
        }

        elem->_offsetInParent = parsed.offset - parsed.parentOffset;
        return elem;
    }

    template<typename T>
    inline T& ebmlElementType::as() {
        if (auto recast = dynamic_cast<T*>(this)) {
            return *recast;
        }

        throw std::bad_cast();
    }

    template<typename T>
    inline T& ebmlElementType::as() const {
        if (auto recast = dynamic_cast<T*>(this)) {
            return *recast;
        }

        throw std::bad_cast();
    }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // template<typename... Args>
    // inline ebml::ptr<ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::operator()(const Args&... args) const {
    //     auto elem = new ebmlinst_t(static_cast<const ebmltype_t*>(this), args...);
    //     elem->_validate();
    //     return elem->sp();
    // }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    template<typename... Args>
    inline ebml::ptr<ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::operator()(Args&&... args) const {
        auto elem = new ebmlinst_t(static_cast<const ebmltype_t*>(this), std::forward<Args>(args)...);
        elem->_validate();
        return elem->sp();
    }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // template<typename... Args>
    // inline ebml::ptr<ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::operator()(Args... args) const {
    //     auto elem = new ebmlinst_t(static_cast<const ebmltype_t*>(this), args...);
    //     elem->_validate();
    //     return elem->sp();
    // }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // template<typename... Args1, typename... Args2>
    // inline ebml::ptr<ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::operator()(const Args1&... args1, Args2... args2) const {
    //     auto elem = new ebmlinst_t(static_cast<const ebmltype_t*>(this), args1..., args2...);
    //     elem->_validate();
    //     return elem->sp();
    // }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::decode(const parseString& parsed) const {
        return static_cast<ebmlinst_t*>(this->_decode(parsed))->sp();
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::decode(const parseFile& parsed) const {
        return static_cast<ebmlinst_t*>(this->_decode(parsed))->sp();
    }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // ebml::ptr<ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::decode(const char* data, size_t size) const {
    //     return static_cast<ebmlinst_t*>(this->_decode(data, size))->sp();
    // }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // ebml::ptr<ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::decode(const std::string& data) const {
    //     return static_cast<ebmlinst_t*>(this->_decode(data))->sp();
    // }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::decode(const ioBase_sp& file) const {
    //     return static_cast<ebmlinst_t*>(this->_decode(file))->sp();eb
    // }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::decode(ioBase& file) const {
        return static_cast<ebmlinst_t*>(this->_decode(file))->sp();
    }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::decode(const ioBase_sp& file, off_t& offset) const {
    //     return static_cast<ebmlinst_t*>(this->_decode(file, offset))->sp();
    // }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::decode(ioBase& file, off_t& offset) const {
        return static_cast<ebmlinst_t*>(this->_decode(file, offset))->sp();
    }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::decode(const ioBase_sp& file, off_t offset, off_t& endoffset) const {
    //     return static_cast<ebmlinst_t*>(this->_decode(file, offset, endoffset))->sp();
    // }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::decode(ioBase& file, off_t offset, off_t& endoffset) const {
        return static_cast<ebmlinst_t*>(this->_decode(file, offset, endoffset))->sp();
    }



    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<const ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const parseString& parsed) const {
        return static_cast<const ebmlinst_t*>(this->_cdecode(parsed))->sp();
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<const ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const parseFile& parsed) const {
        return static_cast<const ebmlinst_t*>(this->_cdecode(parsed))->sp();
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<const ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const char* data, size_t size) const {
        return static_cast<const ebmlinst_t*>(this->_cdecode(data, size))->sp();
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<const ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const std::string& data) const {
        return static_cast<const ebmlinst_t*>(this->_cdecode(data))->sp();
    }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<const ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const ioBase_sp& file) const {
    //     return static_cast<const ebmlinst_t*>(this->_cdecode(file))->sp();
    // }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<const ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(ioBase& file) const {
        return static_cast<const ebmlinst_t*>(this->_cdecode(file))->sp();
    }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<const ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const ioBase_sp& file, off_t& offset) const {
    //     return static_cast<const ebmlinst_t*>(this->_cdecode(file, offset))->sp();
    // }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<const ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(ioBase& file, off_t& offset) const {
        return static_cast<const ebmlinst_t*>(this->_cdecode(file, offset))->sp();
    }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<const ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const ioBase_sp& file, off_t offset, off_t& endoffset) const {
    //     return static_cast<const ebmlinst_t*>(this->_cdecode(file, offset, endoffset))->sp();
    // }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<const ebmlinst_t> ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(ioBase& file, off_t offset, off_t& endoffset) const {
        return static_cast<const ebmlinst_t*>(this->_cdecode(file, offset, endoffset))->sp();
    }
}

#endif
