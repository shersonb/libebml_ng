#ifndef EBML_NG_VOID_H
#define EBML_NG_VOID_H

#include "ebmlElementClass.h"
#include "ebmlElement.h"

namespace ebml {
    class ebmlVoid;

    class ebmlVoidClass : public ClsMixin<ebmlVoidClass, ebmlVoid> {
        friend class ebmlVoid;
    public:
        ebmlVoidClass();
        // ebmlElement_sp operator()(size_t) const;

    protected:
        // ebmlElement* _new() const override;
        // ebmlElement_sp _decode(const parseString&) const override;
        // ebmlElement_sp _decode(const parseFile&) const override;

    public:
        unsigned char writeVoid(char*, size_t) const;
        // unsigned char writeVoid(const ioBase_sp&, off_t, size_t) const;
        unsigned char writeVoid(ioBase&, off_t, size_t) const;
    };

    class ebmlVoid : public InstMixin<ebmlVoidClass, ebmlVoid> {
        friend class ClsMixin<ebmlVoidClass, ebmlVoid>;
        friend class InstMixin<ebmlVoidClass, ebmlVoid>;

    protected:
        // using InstMixin<ebmlVoidClass, ebmlVoid>::_cloneNEW;
        // using InstMixin<ebmlVoidClass, ebmlVoid>::cloneNEW;
        ebmlVoid(const ebmlVoidClass*);
        ebmlVoid(const ebmlVoidClass*, const parseString&);
        ebmlVoid(const ebmlVoidClass*, const parseFile&);
        ebmlVoid(const ebmlVoidClass*, size_t);
        ebmlVoid(const ebmlVoid&);

    public:
        size_t voidsize;
        size_t dataSize() const;

        // Encode functions.
    protected:
        size_t _encode(char*) const;

    public:
        size_t encode(ioBase*) const;

    //     // Decode functions:
    // protected:
    //     void _decode(const parseString&);
    //     void _decode(const parseFile&);

        // Cloning functions:
    protected:
        void _clonedata(const ebmlElement*);

    public:
        std::wstring minirepr() const;
        std::wstring repr() const;

        friend class ebmlVoidClass;
    };

    extern ebmlVoidClass Void;
}
#endif
