#ifndef EBML_NG_VOID_H
#define EBML_NG_VOID_H

#include "ebmlElementType.h"
#include "ebmlElement.h"

namespace ebml {
    class ebmlVoid;

    /**
     * @ingroup type_definitions
     * @ingroup type_implementations
     * @class ebmlVoidType
     * @brief Represents a void EBML element.
     *
     * This class defines an EBML element that is used for padding or reserving space within a file.
     * It does not contain any meaningful data.
     */
    class ebmlVoidType : public ebmlTypeCRTP<ebmlVoidType, ebmlVoid> {
        friend class ebmlVoid;
    public:
        ebmlVoidType();
        unsigned char writeVoid(char*, size_t) const;
        unsigned char writeVoid(ioBase&, off_t, size_t) const;
    };

    class ebmlVoid : public ebmlElementCRTP<ebmlVoidType, ebmlVoid> {
        friend class ebmlTypeCRTP<ebmlVoidType, ebmlVoid>;
        friend class ebmlElementCRTP<ebmlVoidType, ebmlVoid>;

    protected:
        ebmlVoid(const ebmlVoidType*);
        ebmlVoid(const ebmlVoidType*, const parseString&);
        ebmlVoid(const ebmlVoidType*, const parseFile&);
        ebmlVoid(const ebmlVoidType*, size_t);
        ebmlVoid(const ebmlVoid&);

    public:
        size_t voidsize;
        size_t dataSize() const;

    protected:
        size_t _encode(char*) const;

    public:
        size_t encode(ioBase*) const;

    protected:
        void _clonedata(const ebmlElement*);

    public:
        std::wstring minirepr() const;
        std::wstring repr() const;

        friend class ebmlVoidType;
    };

    extern ebmlVoidType Void;
}
#endif
