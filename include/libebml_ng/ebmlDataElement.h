#ifndef EBML_NG_DATAELEMENT_H
#define EBML_NG_DATAELEMENT_H

#include "libebml_ng/ebmlElementClass.h"
#include "libebml_ng/ebmlElement.h"
#include "libebml_ng/ebmlElement.tpp"

#include "libebml_ng/struct/ull.h"
#include "libebml_ng/struct/ll.h"
#include "libebml_ng/struct/double.h"
#include "libebml_ng/struct/binary.h"
#include "libebml_ng/struct/unicode.h"
#include "libebml_ng/struct/datetime.h"

namespace ebml {
    template<typename T>
    class ebmlDataElement;

    template<typename T>
    using ebmlDataElement_sp = std::shared_ptr<ebmlDataElement<T>>;

    template<typename T>
    class ebmlDataElementClass : public ClsMixin<ebmlDataElementClass<T>, ebmlDataElement<T>> {
    // class ebmlDataElementClass : public ebmlElementClass {
        friend class ebmlDataElement<T>;
    public:
        T defaultval;
        static T defaultdefault;
        // ebmlDataElementClass();

        ebmlDataElementClass(const char*, const std::wstring&);
        ebmlDataElementClass(const char*, const std::wstring&, const T&);
        ebmlDataElementClass(const char*, const std::wstring&, T&&);

        ebmlDataElementClass(ebmlID_t, const std::wstring&);
        ebmlDataElementClass(ebmlID_t, const std::wstring&, const T&);
        ebmlDataElementClass(ebmlID_t, const std::wstring&, T&&);

        ebmlDataElementClass(const char*, const std::wstring&, const seekHelper_t*);
        ebmlDataElementClass(const char*, const std::wstring&, const T&, const seekHelper_t*);
        ebmlDataElementClass(const char*, const std::wstring&, T&&, const seekHelper_t*);

        ebmlDataElementClass(ebmlID_t, const std::wstring&, const seekHelper_t*);
        ebmlDataElementClass(ebmlID_t, const std::wstring&, const T&, const seekHelper_t*);
        ebmlDataElementClass(ebmlID_t, const std::wstring&, T&&, const seekHelper_t*);

        // ebmlDataElement_sp<T> operator()() const;
        // ebmlDataElement_sp<T> operator()(const T&) const;
        // ebmlDataElement_sp<T> operator()(T&&) const;

    protected:
        // ebmlElement* _new() const override;
        // ebmlElement_sp _decode(const parseString&) const override;
        // ebmlElement_sp _decode(const parseFile&) const override;
    };

    template<typename T>
    class ebmlDataElement : public InstMixin<ebmlDataElementClass<T>, ebmlDataElement<T>> {
    // class ebmlDataElement : public ebmlElement {
        friend class ebmlDataElementClass<T>;
        friend class ClsMixin<ebmlDataElementClass<T>, ebmlDataElement<T>>;
        friend class InstMixin<ebmlDataElementClass<T>, ebmlDataElement<T>>;

        // Constructor/Destructor
    protected:
        ebmlDataElement(const ebmlDataElementClass<T>*);
        ebmlDataElement(const ebmlDataElementClass<T>*, const T&);
        ebmlDataElement(const ebmlDataElementClass<T>*, T&&);
        ebmlDataElement(const ebmlDataElementClass<T>*, const parseString&);
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
    // class ebmlDataElementClass<const T> : public ebmlElementClassTmpl<ebmlDataElementClass<const T>, ebmlDataElement<const T>> {
    class ebmlDataElementClass<const T> : public ClsMixin<ebmlDataElementClass<const T>, ebmlDataElement<const T>> {
        friend class ebmlDataElement<const T>;
    public:
        const T defaultval;
        static const T defaultdefault;
        // ebmlDataElementClass();

        ebmlDataElementClass(const char*, const std::wstring&);
        ebmlDataElementClass(const char*, const std::wstring&, const T&);
        ebmlDataElementClass(const char*, const std::wstring&, T&&);

        ebmlDataElementClass(ebmlID_t, const std::wstring&);
        ebmlDataElementClass(ebmlID_t, const std::wstring&, const T&);
        ebmlDataElementClass(ebmlID_t, const std::wstring&, T&&);

        // virtual ebmlElement_sp operator()() const;
        // ebmlDataElement_sp<const T> operator()(const T&) const;
        // ebmlDataElement_sp<const T> operator()(T&&) const;

        // virtual ebmlElement_sp decode(const parseString&) const;
        // virtual ebmlElement_sp decode(const parseFile&) const;

    protected:
        // ebmlElement* _new() const override;
        // ebmlElement_sp _decode(const parseString&) const override;
        // ebmlElement_sp _decode(const parseFile&) const override;
        // ebmlElement_sp _cdecode(const parseString&) const override;
        // ebmlElement_sp _cdecode(const parseFile&) const override;
    };

    template<typename T>
    // class ebmlDataElement<const T> : public ebmlElementTmpl<ebmlDataElementClass<const T>, ebmlDataElement<const T>> {
    class ebmlDataElement<const T> : public InstMixin<ebmlDataElementClass<const T>, ebmlDataElement<const T>> {
        friend class ebmlDataElementClass<const T>;
        friend class ClsMixin<ebmlDataElementClass<const T>, ebmlDataElement<const T>>;
        friend class InstMixin<ebmlDataElementClass<const T>, ebmlDataElement<const T>>;

        // Constructor/Destructor
    protected:
        ebmlDataElement(const ebmlDataElementClass<const T>*, const T&);
        ebmlDataElement(const ebmlDataElementClass<const T>*, T&&);
        ebmlDataElement(const ebmlDataElementClass<const T>*, const parseString&);
        ebmlDataElement(const ebmlDataElement<const T>&);

    public:
        T data;
        // const ebmlDataElementClass<const T>* cls() const;

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
    extern template class ebml::ebmlDataElementClass<T>; \
    extern template class ebml::ebmlDataElement<T>; \
    extern template T& ebml::data<T>(const ebmlElement_sp&); \
    extern template const T& ebml::data<T>(const c_ebmlElement_sp&)

#define DECL_EBML_ALIAS(T, alias) \
    typedef ::ebml::ebmlDataElementClass<T> alias##Class; \
    typedef ::ebml::ebmlDataElement<T> alias;

#define INST_EBML_CLS(T, defaultval) \
    template<> \
    T ebml::ebmlDataElementClass<T>::defaultdefault = defaultval; \
    template class ebml::ebmlDataElementClass<T>; \
    template class ebml::ebmlDataElement<T>; \
    template T& ebml::data<T>(const ebml::ebmlElement_sp&); \
    template const T& ebml::data<T>(const ebml::c_ebmlElement_sp&)

#define INST_CONST_EBML_CLS(T, defaultval) \
    template<> \
    const T ebml::ebmlDataElementClass<const T>::defaultdefault = defaultval; \
    template class ebml::ebmlDataElementClass<const T>; \
    template class ebml::ebmlDataElement<const T>; \
    template const T& ebml::data<const T>(const ebml::c_ebmlElement_sp&);


DECL_DATA_CLS(unsigned long long);
DECL_DATA_CLS(long long);
DECL_DATA_CLS(double);
DECL_DATA_CLS(std::string);
DECL_DATA_CLS(std::wstring);
DECL_DATA_CLS(ebml::timepoint_t);
#endif
