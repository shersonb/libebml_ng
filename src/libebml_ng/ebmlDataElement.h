#ifndef EBML_NG_DATAELEMENT_H
#define EBML_NG_DATAELEMENT_H

#include "ebmlElementType.h"
#include "ebmlElement.h"
#include "ebmlElement.tpp"

#include "struct/ull.h"
#include "struct/ll.h"
#include "struct/double.h"
#include "struct/binary.h"
#include "struct/unicode.h"
#include "struct/datetime.h"
#include "repr.h"

namespace ebml {
    template<typename T>
    class ebmlDataElement;

    template<typename T>
    using ebmlDataElement_sp = std::shared_ptr<ebmlDataElement<T>>;

    /**
     * @brief Template class for EBML data types.
     *
     * Provides a factory and decoding interface for EBML elements that store data
     * of type T.
     *
     * In order to implement `ebmlDataType<T>` and `ebmlDataElement<T>`, you only
     * need to implement these functions:
     * - `size_t size(const T& value)`,
     * - `size_t ebml::pack(const T&, char* dest)` or
     *      `size_t ebml::pack(const T&, size_t size, char* dest)`,
     * - `T unpack(const char*, size_t)`,
     * and define the static variable `ebmlDataType<T>::defaultdefault`.
     *
     * @tparam T The type of data stored in the element.
     * @ingroup type_definitions
     * @ingroup type_templates
     */
    template<typename T>
    class ebmlDataType : public ebmlTypeCRTP<ebmlDataType<T>, ebmlDataElement<T>> {
        friend class ebmlDataElement<T>;
    public:
        T defaultval;
        static T defaultdefault;

        ebmlDataType(const char*, const std::wstring&);
        ebmlDataType(const char*, const std::wstring&, const T&);
        ebmlDataType(const char*, const std::wstring&, T&&);

        ebmlDataType(ebmlID_t, const std::wstring&);
        ebmlDataType(ebmlID_t, const std::wstring&, const T&);
        ebmlDataType(ebmlID_t, const std::wstring&, T&&);

        ebmlDataType(const char*, const std::wstring&, const seekHelper_t*);
        ebmlDataType(const char*, const std::wstring&, const T&, const seekHelper_t*);
        ebmlDataType(const char*, const std::wstring&, T&&, const seekHelper_t*);

        ebmlDataType(ebmlID_t, const std::wstring&, const seekHelper_t*);
        ebmlDataType(ebmlID_t, const std::wstring&, const T&, const seekHelper_t*);
        ebmlDataType(ebmlID_t, const std::wstring&, T&&, const seekHelper_t*);

        // ebmlDataElement_sp<T> operator()() const;
        // ebmlDataElement_sp<T> operator()(const T&) const;
        // ebmlDataElement_sp<T> operator()(T&&) const;

    protected:
        // ebmlElement* _new() const override;
        // ebmlElement_sp _decode(const parseString&) const override;
        // ebmlElement_sp _decode(const parseFile&) const override;
    };

    template<typename T>
    class ebmlDataElement : public ebmlElementCRTP<ebmlDataType<T>, ebmlDataElement<T>> {
        friend class ebmlDataType<T>;
        friend class ebmlTypeCRTP<ebmlDataType<T>, ebmlDataElement<T>>;
        friend class ebmlElementCRTP<ebmlDataType<T>, ebmlDataElement<T>>;

        // Constructor/Destructor
    protected:
        ebmlDataElement(const ebmlDataType<T>*);
        ebmlDataElement(const ebmlDataType<T>*, const T&);
        ebmlDataElement(const ebmlDataType<T>*, T&&);
        ebmlDataElement(const ebmlDataType<T>*, const parseString&);
        ebmlDataElement(const ebmlDataElement<T>&);

    public:
        T data;

        // Size functions.
    public:
        virtual size_t dataSize() const;

        // Encode functions.
    protected:
        virtual size_t _encode(char*) const;
        // virtual size_t _encode(char*, size_t) const;

    //     // Decode functions:
    // protected:
    //     virtual void _decode(const parseString&);
    //     virtual void _decode(const parseFile&);

        // Cloning functions:
    protected:
        virtual void _clonedata(const ebmlElement*);

    public:
        // Repr function
        std::wstring minirepr() const;
    };

    // Specializations for const T are needed to support move construction.

    template<typename T>
    class ebmlDataType<const T> : public ebmlTypeCRTP<ebmlDataType<const T>, ebmlDataElement<const T>> {
        friend class ebmlDataElement<const T>;
    public:
        const T defaultval;
        static const T defaultdefault;

        ebmlDataType(const char*, const std::wstring&);
        ebmlDataType(const char*, const std::wstring&, const T&);
        ebmlDataType(const char*, const std::wstring&, T&&);

        ebmlDataType(ebmlID_t, const std::wstring&);
        ebmlDataType(ebmlID_t, const std::wstring&, const T&);
        ebmlDataType(ebmlID_t, const std::wstring&, T&&);
    };

    template<typename T>
    // class ebmlDataElement<const T> : public ebmlElementTmpl<ebmlDataType<const T>, ebmlDataElement<const T>> {
    class ebmlDataElement<const T> : public ebmlElementCRTP<ebmlDataType<const T>, ebmlDataElement<const T>> {
        friend class ebmlDataType<const T>;
        friend class ebmlTypeCRTP<ebmlDataType<const T>, ebmlDataElement<const T>>;
        friend class ebmlElementCRTP<ebmlDataType<const T>, ebmlDataElement<const T>>;

        // Constructor/Destructor
    protected:
        ebmlDataElement(const ebmlDataType<const T>*, const T&);
        ebmlDataElement(const ebmlDataType<const T>*, T&&);
        ebmlDataElement(const ebmlDataType<const T>*, const parseString&);
        ebmlDataElement(const ebmlDataElement<const T>&);

    public:
        T data;
        // const ebmlDataType<const T>* cls() const;

        // Size functions.
    public:
        virtual size_t dataSize() const;

        // Encode functions.
    protected:
        virtual size_t _encode(char*) const;
        // virtual size_t _encode(char*, size_t) const;

    //     // Decode functions:
    // protected:
    //     void _decode(const parseString&);
    //     void _decode(const parseFile&);

        // Cloning functions:
    public:
        ebmlElement_sp clone() const;

    protected:
        void _clonedata(const ebmlElement*);

    public:
        // Repr function
        std::wstring minirepr() const;
    };

    // Provides easy read-write access to data member of elements.
    template<typename T>
    T& data(const ebmlElement_sp& elem);

    template<typename T>
    const T& data(const c_ebmlElement_sp& elem);
}

#define DECL_DATA_CLS(T) \
    extern template class ebml::ebmlDataType<T>; \
    extern template class ebml::ebmlDataElement<T>; \
    extern template T& ebml::data<T>(const ebmlElement_sp&); \
    extern template const T& ebml::data<T>(const c_ebmlElement_sp&)

#define DECL_EBML_ALIAS(T, alias) \
    typedef ::ebml::ebmlDataType<T> alias##Type; \
    typedef ::ebml::ebmlDataElement<T> alias;

#define INST_EBML_CLS(T, defaultval) \
    template<> \
    T ebml::ebmlDataType<T>::defaultdefault = defaultval; \
    template class ebml::ebmlDataType<T>; \
    template class ebml::ebmlDataElement<T>; \
    template T& ebml::data<T>(const ebml::ebmlElement_sp&); \
    template const T& ebml::data<T>(const ebml::c_ebmlElement_sp&)

#define INST_CONST_EBML_CLS(T, defaultval) \
    template<> \
    const T ebml::ebmlDataType<const T>::defaultdefault = defaultval; \
    template class ebml::ebmlDataType<const T>; \
    template class ebml::ebmlDataElement<const T>; \
    template const T& ebml::data<const T>(const ebml::c_ebmlElement_sp&);

namespace ebml {
    /**
     * @brief EBML Unsigned Integer Type.
     * @ingroup type_definitions
     * @ingroup type_implementations
     */
    typedef ebmlDataType<unsigned long long> ebmlUnsignedIntegerType;

    /**
     * @brief EBML Signed Integer Type.
     * @ingroup type_definitions
     * @ingroup type_implementations
     */
    typedef ebmlDataType<long long> ebmlSignedIntegerType;

    /**
     * @brief EBML Float Type.
     * @ingroup type_definitions
     * @ingroup type_implementations
     */
    typedef ebmlDataType<double> ebmlFloatType;

    /**
     * @brief EBML Binary Type.
     * @ingroup type_definitions
     * @ingroup type_implementations
     */
    typedef ebmlDataType<std::string> ebmlBinaryType;

    /**
     * @brief EBML Unicode Type.
     * @ingroup type_definitions
     * @ingroup type_implementations
     */
    typedef ebmlDataType<std::wstring> ebmlUnicodeType;

    /**
     * @brief EBML Date Type.
     * @ingroup type_definitions
     * @ingroup type_implementations
     */
    typedef ebmlDataType<ebml::timepoint_t> ebmlDateTimeType;
}

DECL_DATA_CLS(unsigned long long);
DECL_DATA_CLS(long long);
DECL_DATA_CLS(double);
DECL_DATA_CLS(std::string);
DECL_DATA_CLS(std::wstring);
DECL_DATA_CLS(ebml::timepoint_t);
#endif
