#ifndef EBML_NG_TEMPLATE_H
#define EBML_NG_TEMPLATE_H

#include "libebml_ng/base.h"
#pragma message("template.h included")

namespace ebml {
    template<typename T>
    class ebmlElementTemplate;

    template<typename T>
    class ebmlElementClassTemplate : public ebmlElementClassBase {
        friend class ebmlElementTemplate<T>;
    public:
        T defaultval;
        ebmlElementClassTemplate(ebmlID_t, const std::u32string&);
        ebmlElementClassTemplate(ebmlID_t, const std::u32string&, const T&);
        virtual ebmlElement_sp operator()() const;
        virtual ebmlElement_sp operator()(const T&) const;
    };

    template<typename T>
    class ebmlElementTemplate : public ebmlElementBase {
        friend class ebmlElementClassTemplate<T>;

        // Constructor/Destructor
    protected:
        ebmlElementTemplate(const ebmlElementClassTemplate<T>*, const T&);

    public:
        T data;

        // Size functions.
    public:
        virtual size_t dataSize() const;

        // Encode functions.
    protected:
        virtual size_t _encode(char*) const;
//         virtual size_t _encode(char*, unsigned long long) const;

        // Decode functions:
    protected:
        virtual void _decode(const parseString&);
        virtual void _decode(const parseFile_sp&);

        // Cloning functions:
    protected:
        virtual void _clonedata(const ebmlElementBase*);

    };

    template<typename T>
    class ebmlElement;

    template<typename T>
    class ebmlElementClass : public ebmlElementClassTemplate<T> {
        friend class ebmlElement<T>;
//     public:
//         ebmlElementClass(ebmlID_t _ebmlID, const std::u32string& _name) : ebmlElementClassTemplate(_ebmlID, _name) {};
//         ebmlElementClass(ebmlID_t _ebmlID, const std::u32string& _name, const T& _defaultval) : ebmlElementClassTemplate(_ebmlID, _name, _defaultval) {};
    };

    template<typename T>
    class ebmlElement : public ebmlElementTemplate<T> {
        friend class ebmlElementClass<T>;
    };
}
#endif
