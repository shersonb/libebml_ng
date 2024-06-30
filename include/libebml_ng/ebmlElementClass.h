#ifndef EBML_NG_ELEMENTCLS_H
#define EBML_NG_ELEMENTCLS_H

#include "ptrs.h"
#include "seekdata.h"
#include "parsing/string.h"
#include "parsing/io.h"
#include "exceptions.h"
#include "vint.h"
#include <utility>

namespace ebml {
    /**
     * Abstract base class for EBML Element Type objects. Every
     * subclass of ebmlElementClass must have a companion subclass
     * of ebmlElement declared as a friend class.
     *
     * Instances of ebmlElementClass are constructed using an
     * EBML ID, either as an ebmlID_t (unsigned long long) or as
     * a VINT (const char*), and a class name (unicode string).
     */

    class ebmlElementClass {
        friend class ebmlElement;
        friend class ebmlMasterElementClass;

    public:

        /**
        * Constructor for EBML Element Type objects.
        *
        * @param ebmlID EBML ID as a VINT string.
        * @param name Name for EBML Element Type.
        *
        * Example:
        * @code
        * ebmlVoidClass::ebmlVoidClass() : ebmlElementClass("\xec", "EBMLVoid") {}
        * @endcode
        */

        inline ebmlElementClass(const char* ebmlID, const std::wstring& name);
        inline ebmlElementClass(const char* ebmlID, const std::wstring& name, const seekHelper_t* seekHelper);

        /**
        * Constructor for EBML Element Type objects.
        *
        * @param ebmlID EBML ID as an unsigned integer.
        * @param name Name for EBML Element Type.
        *
        * Example:
        * @code
        * ebmlVoidClass::ebmlVoidClass() : ebmlElementClass(108, "EBMLVoid") {}
        * @endcode
        */

        inline ebmlElementClass(ebmlID_t ebmlID, const std::wstring& name);
        inline ebmlElementClass(ebmlID_t ebmlID, const std::wstring& name, const seekHelper_t* seekHelper);

        ebmlID_t ebmlID;
        std::wstring name;

        /**
        * Create default ebmlElement instance.
        *
        * @return Shared pointer to ebmlElement instance.
        *
        * Example:
        * @code
        * ebmlElementClass ebmlClass = ...;
        * ebmlElement_sp elem = ebmlClass();
        * @endcode
        */

        inline ebml::ptr<ebmlElement> operator()() const;

    protected:
        virtual ebmlElement* _new() const = 0;
        virtual ebmlElement* _decode_v(const parseString&) const = 0;
        virtual ebmlElement* _decode_v(const parseFile&) const = 0;
        inline ebmlElement* _decode_nocheck(const parseString& parsed) const;
        inline ebmlElement* _decode_nocheck(const parseFile& parsed) const;

    public:
        /**
         * Create ebmlElement instance, decoding data in a
         * parseString instance.
         *
         * @return Shared pointer to new ebmlElement instance.
         *
         * Example:
         * @code
         * ebmlVoidClass Void;
         * const char* data = "\xec\x82\x00\x00";
         * auto parsed = parseString(data);
         * ebmlElement_sp elem = Void.decode(parsed);
         * @endcode
         */

        /**
         * @defgroup DecodeGroup Decode Functions
         * @{
         */

        /**
         * @brief Decode function for various input types.
         * @param data The input data to decode.
         * @param size The size of the input data.
         * @param ioBase The input IO base.
         * @param off_t The offset value.
         * @param[out] off_t The updated offset value.
         * @return The decoded element.
         */

        inline ebml::ptr<ebmlElement> decode(const parseString&) const;
        inline ebml::ptr<ebmlElement> decode(const parseFile&) const;
        inline ebml::ptr<ebmlElement> decode(const char*, size_t) const;
        inline ebml::ptr<ebmlElement> decode(const std::string&) const;
        // inline ebml::ptr<ebmlElement> decode(const ioBase_sp&) const;
        // inline ebml::ptr<ebmlElement> decode(const ioBase_sp&, off_t&) const;
        // inline ebml::ptr<ebmlElement> decode(const ioBase_sp&, off_t, off_t&) const;
        inline ebml::ptr<ebmlElement> decode(ioBase&) const;
        inline ebml::ptr<ebmlElement> decode(ioBase&, off_t&) const;
        inline ebml::ptr<ebmlElement> decode(ioBase&, off_t, off_t&) const;
        /**
         * @}
         */

    protected:
        inline ebmlElement* _decode(const parseString&) const;
        inline ebmlElement* _decode(const parseFile&) const;
        inline ebmlElement* _decode(const char*, size_t) const;
        inline ebmlElement* _decode(const std::string&) const;
        // inline ebmlElement* _decode(const ioBase_sp&) const;
        // inline ebmlElement* _decode(const ioBase_sp&, off_t&) const;
        // inline ebmlElement* _decode(const ioBase_sp&, off_t, off_t&) const;
        inline ebmlElement* _decode(ioBase&) const;
        inline ebmlElement* _decode(ioBase&, off_t&) const;
        inline ebmlElement* _decode(ioBase&, off_t, off_t&) const;
        // TODO:
        // ebmlElement_sp decode(ioBase_sp&, off_t) const;
        // ebmlElement_sp decode(ioBase*, off_t) const;

    public:
        // Decode functions that return an immutable ebmlElement instance.
        // These are virtual because it may be necessary to reimplement in some cases.
        // c_ebmlElement_sp cdecode(const parseString&) const;
        // c_ebmlElement_sp cdecode(const parseFile&) const;

        // Decode object using cdecode logic, but return it as a mutable object
        // Obviously must be used with care. These functions are offered
        // for use by ebmlMasterElementClass instances to cdecode
        // ebmlMasterElement instances that must retain ebmlElement_sp pointers
        // to its children internally. Any function or class using these methods
        // SHOULD cast element to a const before offering it oublicly.
        // ebmlElement_sp mcdecode(const parseString&) const;
        // ebmlElement_sp mcdecode(const parseFile&) const;

        /**
        * Create const ebmlElement instance, decoding from a const char* array.
        *
        * @param data Data to decode.
        * @param size Size of 'data'.
        * @return Shared pointer to new ebmlElement instance.
        *
        * Example:
        * @code
        * ebmlVoidClass Void;
        * const char* = "\xec\x82\x00\x00";
        * ebmlElement_sp elem = Void.cdecode(data, 4);
        * @endcode
        */

        inline ebml::ptr<const ebmlElement> cdecode(const parseString&) const;
        inline ebml::ptr<const ebmlElement> cdecode(const parseFile&) const;
        inline ebml::ptr<const ebmlElement> cdecode(const char*, size_t) const;
        inline ebml::ptr<const ebmlElement> cdecode(const std::string&) const;
        // inline ebml::ptr<const ebmlElement> cdecode(const ioBase_sp&) const;
        // inline ebml::ptr<const ebmlElement> cdecode(const ioBase_sp&, off_t& offset) const;
        // inline ebml::ptr<const ebmlElement> cdecode(const ioBase_sp&, off_t offset, off_t& endoffset) const;
        inline ebml::ptr<const ebmlElement> cdecode(ioBase&) const;
        inline ebml::ptr<const ebmlElement> cdecode(ioBase&, off_t& offset) const;
        inline ebml::ptr<const ebmlElement> cdecode(ioBase&, off_t offset, off_t& end) const;

    protected:
        inline ebmlElement* _cdecode(const parseString&) const;
        inline ebmlElement* _cdecode(const parseFile&) const;
        inline ebmlElement* _cdecode(const char*, size_t) const;
        inline ebmlElement* _cdecode(const std::string&) const;
        // inline ebmlElement* _cdecode(const ioBase_sp&) const;
        // inline ebmlElement* _cdecode(const ioBase_sp&, off_t& offset) const;
        // inline ebmlElement* _cdecode(const ioBase_sp&, off_t offset, off_t& endoffset) const;
        inline ebmlElement* _cdecode(ioBase&) const;
        inline ebmlElement* _cdecode(ioBase&, off_t& offset) const;
        inline ebmlElement* _cdecode(ioBase&, off_t offset, off_t& end) const;

        const seekHelper_t* _seekHelper = &seekHelper;

    public:
        seekMapBase* initSeekMap() const;
        seekData_t* makeSeekData(const parseString&) const;
        seekData_t* makeSeekData(const parseString&, const ebmlMasterElement_wp&) const;
        seekData_t* makeSeekData(const parseString&, ebmlMasterElement_wp&&) const;
        seekData_t* makeSeekData(const parseFile&) const;
        seekData_t* makeSeekData(const parseFile&, const ebmlMasterElement_wp&) const;
        seekData_t* makeSeekData(const parseFile&, ebmlMasterElement_wp&&) const;

    protected:
        virtual ebmlElement* _cdecode_v(const parseString&) const = 0;
        virtual ebmlElement* _cdecode_v(const parseFile&) const = 0;
        inline ebmlElement* _cdecode_nocheck(const parseString& parsed) const;
        inline ebmlElement* _cdecode_nocheck(const parseFile& parsed) const;

    public:
        /**
        * Convenience member function that dynamically casts
        * ebmlElementClass to a subclass.
        *
        * @return Reference to instance as a T object.
        *
        * Example:
        * @code
        * ebmlElementClass cls = ebmlVoidClass();
        * ebmlVoidClass& Void = cls.as<ebmlVoidClass>();
        * @endcode
        * @throws std::bad_cast If instance is not an instance of T.
        */

        template<typename T=ebmlElementClass>
        inline T& as();

        /**
        * Convenience member function that dynamically casts
        * const ebmlElementClass to a subclass.
        *
        * @return Const reference to instance as a T object.
        *
        * Example:
        * @code
        * const ebmlElementClass cls = ebmlVoidClass();
        * const ebmlVoidClass& Void = cls.as<ebmlVoidClass>();
        * @endcode
        * @throws std::bad_cast If instance is not an instance of T.
        */

        template<typename T=ebmlElementClass>
        inline T& as() const;

        // Repr function
        virtual std::wstring minirepr() const;
        std::wstring repr() const;
    };

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t=ebmlElementClass>
    class ClsMixin : public typebase_t {
    protected:
        using typebase_t::_decode;
        ebmlElement* _new() const;
        ebmlElement* _decode_v(const parseString&) const;
        ebmlElement* _decode_v(const parseFile&) const;

        ebmlElement* _cdecode_v(const parseString&) const;
        ebmlElement* _cdecode_v(const parseFile&) const;

    public:
        template<typename... Args>
        inline ClsMixin(Args... args) : typebase_t(args...) {}

        // template<typename... Args>
        // inline ebml::ptr<ebmlinst_t> operator()(const Args&... args) const;

        template<typename... Args>
        inline ebml::ptr<ebmlinst_t> operator()(Args&&... args) const;

        // template<typename... Args>
        // inline ebml::ptr<ebmlinst_t> operator()(Args... args) const;

        // template<typename... Args1, typename... Args2>
        // inline ebml::ptr<ebmlinst_t> operator()(const Args1&... args1, Args2... args2) const;
        // {
        //     // ebmlinst_t* elem = ;
        //     return ebml::ptr<ebmlinst_t>(new ebmlinst_t(this, args...));
        // }

        // ebml::ptr<ebmlinst_t> operator()() const;

        inline ebml::ptr<ebmlinst_t> decode(const parseString& parsed) const;
        inline ebml::ptr<ebmlinst_t> decode(const parseFile& parsed) const;
        inline ebml::ptr<ebmlinst_t> decode(const char* data, size_t size) const;
        inline ebml::ptr<ebmlinst_t> decode(const std::string& data) const;
        // inline ebml::ptr<ebmlinst_t> decode(const ioBase_sp& file) const;
        // inline ebml::ptr<ebmlinst_t> decode(const ioBase_sp& file, off_t& offset) const;
        // inline ebml::ptr<ebmlinst_t> decode(const ioBase_sp& file, off_t offset, off_t& endoffset) const;
        inline ebml::ptr<ebmlinst_t> decode(ioBase& file) const;
        inline ebml::ptr<ebmlinst_t> decode(ioBase& file, off_t& offset) const;
        inline ebml::ptr<ebmlinst_t> decode(ioBase& file, off_t offset, off_t& endoffset) const;

        // ebml::ptr<ebmlinst_t> decode(ioBase&) const;
        // ebml::ptr<ebmlinst_t> decode(ioBase&, off_t&) const;
        // ebml::ptr<ebmlinst_t> decode(ioBase&, off_t, off_t&) const;

        inline ebml::ptr<const ebmlinst_t> cdecode(const parseString& parsed) const;
        inline ebml::ptr<const ebmlinst_t> cdecode(const parseFile& parsed) const;
        inline ebml::ptr<const ebmlinst_t> cdecode(const char* data, size_t size) const;
        inline ebml::ptr<const ebmlinst_t> cdecode(const std::string& data) const;
        // inline ebml::ptr<const ebmlinst_t> cdecode(const ioBase_sp& file) const;
        // inline ebml::ptr<const ebmlinst_t> cdecode(const ioBase_sp& file, off_t& offset) const;
        // inline ebml::ptr<const ebmlinst_t> cdecode(const ioBase_sp& file, off_t offset, off_t& endoffset) const;
        inline ebml::ptr<const ebmlinst_t> cdecode(ioBase& file) const;
        inline ebml::ptr<const ebmlinst_t> cdecode(ioBase& file, off_t& offset) const;
        inline ebml::ptr<const ebmlinst_t> cdecode(ioBase& file, off_t offset, off_t& endoffset) const;
    };

    ebmlElement_sp ebmlElementClass::operator()() const {
        return ebmlElement_sp(this->_new());
    }

    ebmlElementClass::ebmlElementClass(const char* _ebmlID, const std::wstring& _name) : ebmlElementClass(unpackVint(_ebmlID), _name) {}

    ebmlElementClass::ebmlElementClass(const char* _ebmlID, const std::wstring& _name, const seekHelper_t* seekHelper)
    : ebmlElementClass(unpackVint(_ebmlID), _name, seekHelper) {}

    ebmlElementClass::ebmlElementClass(ebmlID_t _ebmlID, const std::wstring& _name) : ebmlID(_ebmlID), name(_name) {}
    ebmlElementClass::ebmlElementClass(ebmlID_t _ebmlID, const std::wstring& _name, const seekHelper_t* seekHelper)
    : ebmlID(_ebmlID), name(_name), _seekHelper(seekHelper) {}

    ebmlElement* ebmlElementClass::_decode(const parseString& parsed) const {
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

    ebml::ptr<ebmlElement> ebmlElementClass::decode(const parseString& parsed) const {
        return _decode(parsed)->sp();
    }

    ebmlElement* ebmlElementClass::_decode(const parseFile& parsed) const {
        if (parsed.ebmlID != this->ebmlID) {
            throw ebmlNoMatch("EBML ID does not match.", this, parsed.offset);
        }

        return _decode_nocheck(parsed);
    }

    ebml::ptr<ebmlElement> ebmlElementClass::decode(const parseFile& parsed) const {
        return _decode(parsed)->sp();
    }

    ebmlElement* ebmlElementClass::_decode(const char* data, size_t size) const {
        auto parsed = parseString(data, size);

        if (size > parsed.outerSize()) {
            throw ebmlDataContinues("Data continues past expected end.", this, 0, parsed.ebmlIDWidth + parsed.sizeWidth, parsed.dataSize);
        }

        return _decode(parsed);
    }

    ebml::ptr<ebmlElement> ebmlElementClass::decode(const char* data, size_t size) const {
        return _decode(data, size)->sp();
    }

    ebmlElement* ebmlElementClass::_decode(const std::string& data) const {
        return _decode(data.data(), data.size());
    }

    ebml::ptr<ebmlElement> ebmlElementClass::decode(const std::string& data) const {
        return _decode(data)->sp();
    }

    // ebmlElement* ebmlElementClass::_decode(const ioBase_sp& file) const {
    //     if (file == nullptr) {
    //         throw std::runtime_error("ebmlElementClass::decode(const ioBase_sp&): nullptr detected.");
    //     }
    //
    //     return _decode(*file);
    // }

    // ebml::ptr<ebmlElement> ebmlElementClass::decode(const ioBase_sp& file) const {
    //     return _decode(file)->sp();
    // }

    ebmlElement* ebmlElementClass::_decode(ioBase& file) const {
        auto parsed = parseFile(file);
        // auto elem = this->decode(parsed);
        auto elem = _decode(parsed);
        file.seek(parsed.endOffset());
        return elem;
    }

    ebml::ptr<ebmlElement> ebmlElementClass::decode(ioBase& file) const {
        return _decode(file)->sp();
    }

    // ebmlElement* ebmlElementClass::_decode(const ioBase_sp& file, off_t& offset) const {
    //     if (file == nullptr) {
    //         throw std::runtime_error("ebmlElementClass::decode(const ioBase_sp&, off_t&): nullptr detected.");
    //     }
    //
    //     return _decode(*file, offset);
    // }

    // ebml::ptr<ebmlElement> ebmlElementClass::decode(const ioBase_sp& file, off_t& offset) const {
    //     return _decode(file, offset)->sp();
    // }

    ebmlElement* ebmlElementClass::_decode(ioBase& file, off_t& offset) const {
        auto parsed = parseFile(file, offset);
        auto elem = _decode(parsed);
        offset = parsed.endOffset();
        return elem;
    }

    ebml::ptr<ebmlElement> ebmlElementClass::decode(ioBase& file, off_t& offset) const {
        return _decode(file, offset)->sp();
    }

    // ebmlElement* ebmlElementClass::_decode(const ioBase_sp& file, off_t offset, off_t& endOffset) const {
    //     if (file == nullptr) {
    //         throw std::runtime_error("ebmlElementClass::decode(const ioBase_sp&, off_t&): nullptr detected.");
    //     }
    //
    //     return _decode(*file, offset, endOffset);
    // }

    // ebml::ptr<ebmlElement> ebmlElementClass::decode(const ioBase_sp& file, off_t offset, off_t& endOffset) const {
    //     return _decode(file, offset, endOffset)->sp();
    // }

    ebmlElement* ebmlElementClass::_decode(ioBase& file, off_t offset, off_t& endOffset) const {
        auto parsed = parseFile(file, offset);
        auto elem = _decode(parsed);
        endOffset = parsed.endOffset();
        return elem;
    }

    ebml::ptr<ebmlElement> ebmlElementClass::decode(ioBase& file, off_t offset, off_t& endOffset) const {
        return _decode(file, offset, endOffset)->sp();
    }

    // CONST DECODING

    ebmlElement* ebmlElementClass::_cdecode(const parseString& parsed) const {
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


    ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(const parseString& parsed) const {
        const auto elem = _cdecode(parsed);
        return elem->sp();
    }

    ebmlElement* ebmlElementClass::_cdecode(const parseFile& parsed) const {
        if (parsed.ebmlID != this->ebmlID) {
            throw ebmlNoMatch("EBML ID does not match.", this, parsed.offset);
        }

        return _cdecode_nocheck(parsed);
    }

    ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(const parseFile& parsed) const {
        const auto elem = _cdecode(parsed);
        return elem->sp();
    }

    ebmlElement* ebmlElementClass::_cdecode(const char* data, size_t size) const {
        auto parsed = parseString(data, size);

        if (size > parsed.outerSize()) {
            throw ebmlDataContinues("Data continues past expected end.", this, 0, parsed.ebmlIDWidth + parsed.sizeWidth, parsed.dataSize);
        }

        return _cdecode(parsed);
    }

    ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(const char* data, size_t size) const {
        const auto elem = _cdecode(data, size);
        return elem->sp();
    }

    ebmlElement* ebmlElementClass::_cdecode(const std::string& data) const {
        return _cdecode(data.data(), data.size());
    }

    ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(const std::string& data) const {
        const auto elem = _cdecode(data);
        return elem->sp();
    }

    // ebmlElement* ebmlElementClass::_cdecode(const ioBase_sp& file) const {
    //     if (file == nullptr) {
    //         throw std::runtime_error("ebmlElementClass::decode(const ioBase_sp&): nullptr detected.");
    //     }
    //
    //     return _cdecode(*file);
    // }

    // ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(const ioBase_sp& file) const {
    //     const auto elem = _cdecode(file);
    //     return elem->sp();
    // }

    ebmlElement* ebmlElementClass::_cdecode(ioBase& file) const {
        auto parsed = parseFile(file);
        // auto elem = this->decode(parsed);
        auto elem = _cdecode(parsed);
        file.seek(parsed.endOffset());
        return elem;
    }

    ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(ioBase& file) const {
        const auto elem = _cdecode(file);
        return elem->sp();
    }

    // ebmlElement* ebmlElementClass::_cdecode(const ioBase_sp& file, off_t& offset) const {
    //     if (file == nullptr) {
    //         throw std::runtime_error("ebmlElementClass::decode(const ioBase_sp&, off_t&): nullptr detected.");
    //     }
    //
    //     return _cdecode(*file, offset);
    // }

    // ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(const ioBase_sp& file, off_t& offset) const {
    //     const auto elem = _cdecode(file, offset);
    //     return elem->sp();
    // }

    ebmlElement* ebmlElementClass::_cdecode(ioBase& file, off_t& offset) const {
        auto parsed = parseFile(file, offset);
        auto elem = _cdecode(parsed);
        offset = parsed.endOffset();
        return elem;
    }

    ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(ioBase& file, off_t& offset) const {
        const auto elem = _cdecode(file, offset);
        return elem->sp();
    }

    // ebmlElement* ebmlElementClass::_cdecode(const ioBase_sp& file, off_t offset, off_t& endOffset) const {
    //     if (file == nullptr) {
    //         throw std::runtime_error("ebmlElementClass::decode(const ioBase_sp&, off_t&): nullptr detected.");
    //     }
    //
    //     return _cdecode(*file, offset, endOffset);
    // }

    // ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(const ioBase_sp& file, off_t offset, off_t& endOffset) const {
    //     const auto elem = _cdecode(file, offset, endOffset);
    //     return elem->sp();
    // }

    ebmlElement* ebmlElementClass::_cdecode(ioBase& file, off_t offset, off_t& endOffset) const {
        auto parsed = parseFile(file, offset);
        auto elem = _cdecode(parsed);
        endOffset = parsed.endOffset();
        return elem;
    }

    ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(ioBase& file, off_t offset, off_t& endOffset) const {
        const auto elem = _cdecode(file, offset, endOffset);
        return elem->sp();
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(const char* data, size_t size) const {
        return static_cast<ebmlinst_t*>(this->_decode(data, size))->sp();
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(const std::string& data) const {
        return static_cast<ebmlinst_t*>(this->_decode(data))->sp();
    }

    inline ebmlElement* ebmlElementClass::_decode_nocheck(const parseString& parsed) const {
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

    inline ebmlElement* ebmlElementClass::_decode_nocheck(const parseFile& parsed) const {
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
        // file->seek(parsed.endOffset());
        return elem;
    }

    inline ebmlElement* ebmlElementClass::_cdecode_nocheck(const parseString& parsed) const {
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

    inline ebmlElement* ebmlElementClass::_cdecode_nocheck(const parseFile& parsed) const {
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
        // file->seek(parsed.endOffset());
        return elem;
    }

    template<typename T>
    inline T& ebmlElementClass::as() {
        if (auto recast = dynamic_cast<T*>(this)) {
            return *recast;
        }

        throw std::bad_cast();
    }

    template<typename T>
    inline T& ebmlElementClass::as() const {
        if (auto recast = dynamic_cast<T*>(this)) {
            return *recast;
        }

        throw std::bad_cast();
    }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // template<typename... Args>
    // inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::operator()(const Args&... args) const {
    //     auto elem = new ebmlinst_t(static_cast<const ebmltype_t*>(this), args...);
    //     elem->_validate();
    //     return elem->sp();
    // }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    template<typename... Args>
    inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::operator()(Args&&... args) const {
        auto elem = new ebmlinst_t(static_cast<const ebmltype_t*>(this), std::forward<Args>(args)...);
        elem->_validate();
        return elem->sp();
    }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // template<typename... Args>
    // inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::operator()(Args... args) const {
    //     auto elem = new ebmlinst_t(static_cast<const ebmltype_t*>(this), args...);
    //     elem->_validate();
    //     return elem->sp();
    // }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // template<typename... Args1, typename... Args2>
    // inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::operator()(const Args1&... args1, Args2... args2) const {
    //     auto elem = new ebmlinst_t(static_cast<const ebmltype_t*>(this), args1..., args2...);
    //     elem->_validate();
    //     return elem->sp();
    // }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(const parseString& parsed) const {
        return static_cast<ebmlinst_t*>(this->_decode(parsed))->sp();
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(const parseFile& parsed) const {
        return static_cast<ebmlinst_t*>(this->_decode(parsed))->sp();
    }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(const char* data, size_t size) const {
    //     return static_cast<ebmlinst_t*>(this->_decode(data, size))->sp();
    // }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(const std::string& data) const {
    //     return static_cast<ebmlinst_t*>(this->_decode(data))->sp();
    // }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(const ioBase_sp& file) const {
    //     return static_cast<ebmlinst_t*>(this->_decode(file))->sp();
    // }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(ioBase& file) const {
        return static_cast<ebmlinst_t*>(this->_decode(file))->sp();
    }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(const ioBase_sp& file, off_t& offset) const {
    //     return static_cast<ebmlinst_t*>(this->_decode(file, offset))->sp();
    // }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(ioBase& file, off_t& offset) const {
        return static_cast<ebmlinst_t*>(this->_decode(file, offset))->sp();
    }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(const ioBase_sp& file, off_t offset, off_t& endoffset) const {
    //     return static_cast<ebmlinst_t*>(this->_decode(file, offset, endoffset))->sp();
    // }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(ioBase& file, off_t offset, off_t& endoffset) const {
        return static_cast<ebmlinst_t*>(this->_decode(file, offset, endoffset))->sp();
    }



    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const parseString& parsed) const {
        return static_cast<const ebmlinst_t*>(this->_cdecode(parsed))->sp();
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const parseFile& parsed) const {
        return static_cast<const ebmlinst_t*>(this->_cdecode(parsed))->sp();
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const char* data, size_t size) const {
        return static_cast<const ebmlinst_t*>(this->_cdecode(data, size))->sp();
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const std::string& data) const {
        return static_cast<const ebmlinst_t*>(this->_cdecode(data))->sp();
    }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const ioBase_sp& file) const {
    //     return static_cast<const ebmlinst_t*>(this->_cdecode(file))->sp();
    // }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(ioBase& file) const {
        return static_cast<const ebmlinst_t*>(this->_cdecode(file))->sp();
    }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const ioBase_sp& file, off_t& offset) const {
    //     return static_cast<const ebmlinst_t*>(this->_cdecode(file, offset))->sp();
    // }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(ioBase& file, off_t& offset) const {
        return static_cast<const ebmlinst_t*>(this->_cdecode(file, offset))->sp();
    }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const ioBase_sp& file, off_t offset, off_t& endoffset) const {
    //     return static_cast<const ebmlinst_t*>(this->_cdecode(file, offset, endoffset))->sp();
    // }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(ioBase& file, off_t offset, off_t& endoffset) const {
        return static_cast<const ebmlinst_t*>(this->_cdecode(file, offset, endoffset))->sp();
    }
}

#endif
