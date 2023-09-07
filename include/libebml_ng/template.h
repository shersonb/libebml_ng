#ifndef EBML_NG_TEMPLATE_H
#define EBML_NG_TEMPLATE_H

#include "libebml_ng/base.h"

namespace ebml {
    template<typename T>
    class ebmlDataElement;

    template<typename T>
    class ebmlDataElementClass : public ebmlElementClass {
        friend class ebmlDataElement<T>;
    public:
        T defaultval;
        static T defaultdefault;
        ebmlDataElementClass();
        ebmlDataElementClass(ebmlID_t, const std::wstring&);
        ebmlDataElementClass(ebmlID_t, const std::wstring&, const T&);
        ebmlDataElementClass(ebmlID_t, const std::wstring&, T&&);

        // virtual ebmlElement_sp operator()() const;
        virtual ebmlElement_sp operator()(const T&) const;
        virtual ebmlElement_sp operator()(T&&) const;

    protected:
        ebmlElement* _new() const;

    };

    template<typename T>
    class ebmlDataElement : public ebmlElement {
        friend class ebmlDataElementClass<T>;

        // Constructor/Destructor
    protected:
        ebmlDataElement(const ebmlDataElementClass<T>*, const T&);
        ebmlDataElement(const ebmlDataElementClass<T>*, T&&);

    public:
        T data;

        // Size functions.
    public:
        virtual size_t dataSize() const;

        // Encode functions.
    protected:
        virtual size_t _encode(char*) const;
        // virtual size_t _encode(char*, size_t) const;

        // Decode functions:
    protected:
        virtual void _decode(const parseString&);
        virtual void _decode(const parseFile&);

        // Cloning functions:
    protected:
        virtual void _clonedata(const ebmlElement*);

    public:
        // Repr function
        std::wstring minirepr() const;
    };

    template<typename T>
    using ebmlDataElement_sp = std::shared_ptr<ebmlDataElement<T>>;

    // Provides easy read-write access to data member of elements.
    template<typename T>
    T& data(const ebmlElement_sp& elem);

    template<typename T>
    T data(const c_ebmlElement_sp& elem);

}
#endif
