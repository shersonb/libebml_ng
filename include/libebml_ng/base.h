#ifndef EBML_NG_BASE_H
#define EBML_NG_BASE_H

#include "stdio.h"
#include <iostream>
#include <exception>
#include <memory>
#include <mutex>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "parsing/string.h"
#include "parsing/io.h"

#include "struct.h"
#include "vint.h"
#include "ebmlID_t.h"

namespace ebml {
    class ebmlElement;
    class ebmlMasterElement;
    class ebmlDocument;
    class ebmlSchema;

    typedef std::shared_ptr<ebmlElement> ebmlElement_sp;
    typedef std::shared_ptr<const ebmlElement> c_ebmlElement_sp;

    typedef std::weak_ptr<ebmlElement> ebmlElement_wp;
    typedef std::weak_ptr<const ebmlElement> c_ebmlElement_wp;

    typedef std::shared_ptr<ebmlDocument> ebmlDocument_sp;
    typedef std::weak_ptr<ebmlDocument> ebmlDocument_wp;

    // Abstract base class for EBML Element Type objects. Every subclass of ebmlElementClass must have a companion subclass of
    // ebmlElement declared as a friend class.

    class ebmlElementClass {
        friend class ebmlElement;
        friend class ebmlMasterElement;
    public:
        ebmlElementClass();
        ebmlElementClass(ebmlID_t, const std::wstring&);
        ebmlID_t ebmlID;
        std::wstring name;

        // This must always be overriden in subclasses to return a shared_ptr to an instance of its companion
        // EBMLElement class.
        ebmlElement_sp operator()() const;

    protected:
        virtual ebmlElement* _new() const = 0;

        // Decode methods taking parse data classes.
    public:
        ebmlElement_sp decode(const parseString&) const;
        ebmlElement_sp decode(const parseFile&) const;

        // Decode methods applied directly to character and file-like objects.
        ebmlElement_sp decode(const char*, size_t) const;
        ebmlElement_sp decode(const std::string&) const;
        ebmlElement_sp decode(ioBase_sp&) const;
        ebmlElement_sp decode(ioBase*) const;

        // Decode functions that return an immutable ebmlElement instance.
        // These are virtual because it may be necessary to reimplement in some cases.
        c_ebmlElement_sp cdecode(const parseString&) const;
        c_ebmlElement_sp cdecode(const parseFile&) const;
        c_ebmlElement_sp cdecode(const char*, size_t) const;
        c_ebmlElement_sp cdecode(const std::string&) const;
        c_ebmlElement_sp cdecode(ioBase_sp&) const;
        c_ebmlElement_sp cdecode(ioBase*) const;

        // Needed for ebmlMasterElement to decode children that cannot be stored as const.
    protected:
        ebmlElement_sp _cdecode(const parseString&) const;
        ebmlElement_sp _cdecode(const parseFile&) const;

        // Repr function
    public:
        virtual std::wstring minirepr() const;
        std::wstring repr() const;
    };

    // Abstract base class for EBML Element objects. Every subclass of ebmlElement must have a companion subclass of
    // ebmlElementClass declared as a friend class.

    class ebmlElement : public std::enable_shared_from_this<ebmlElement> {
        friend class ebmlElementClass;

        // Constructor/Destructor
    protected:
        // Only its companion class is allowed to construct instances of this class.
        ebmlElement(const ebmlElementClass*);

    public:
        virtual ~ebmlElement();

        // Pointer to an instance of its companion ebmlElmeentClass subclass instance.
    protected:
        const ebmlElementClass* _cls;

    public:
        // This method *should* be overriden in subclasses to dyanmic cast its _cls member to that of the
        // companion ebmlElementClass subclass.
        virtual const ebmlElementClass* cls() const;

        ebmlID_t ebmlID() const;

        // Hierarchy members.
    public:
        bool parent_is_const() const; // Member function that specifies if pointer is to a const parent or non-const parent.
        ebmlElement_sp parent() const; // Returns pointer to non-const parent. Throws ebmlException if parent_is_const() returns true.
        c_ebmlElement_sp c_parent() const; // Returns pointer to const parent.

    private:
        unsigned char _parent_flags = 0;

        union {
            ebmlElement_sp _parent;
            ebmlElement_wp _parent_weak;
            c_ebmlElement_sp _const_parent;
            c_ebmlElement_wp _const_parent_weak;
        };

    protected:
        void _setParent(const ebmlElement_sp& parent, bool weak = true);
        void _setParent(const c_ebmlElement_sp& parent, bool weak = true);
        void _setParent(ebmlElement_sp&& parent, bool weak = true);
        void _setParent(c_ebmlElement_sp&& parent, bool weak = true);
        virtual void _attachChild(const ebmlElement_sp& child, bool weak = true);
        virtual void _attachChild(const ebmlElement_sp& child, bool weak = true) const;

        void _detach();
        void _detachChild(const ebmlElement_sp& child);

    public:
        ebmlElement_sp root() const;
        c_ebmlElement_sp c_root() const;

        ebmlDocument_sp document() const;
        ebmlDocument_wp _document;

    public:
        bool hasParent() const;

        // Size functions.
    public:
        unsigned char sizeWidth = 0;
        unsigned char headSize() const;
        unsigned char headSize(size_t) const; // May be deprecated
        size_t outerSize() const;
        size_t outerSize(size_t) const; // May be deprecated
        virtual size_t dataSize() const = 0;

        // Location attributes and methods.
    public:
        off_t offsetInParent();
        off_t offsetInFile();
        off_t dataOffsetInParent();
        off_t dataOffsetInFile();

    protected:
        off_t _offsetInParent; // value of -1 indicates undefined.

        // Encode functions.
    protected:
        size_t _encode_head(char*, size_t) const;
        virtual size_t _encode(char*) const = 0;

    public:
        std::string encode() const;
        virtual size_t encode(char*) const;
        virtual size_t encode(ioBase*) const;
        size_t encode(char*, size_t) const;

        // Decode functions:
    protected:
        virtual void _decode(const parseString&) = 0;
        virtual void _decode(const parseFile&);
        virtual void _cdecode(const parseString&);
        virtual void _cdecode(const parseFile&);

        // Cloning functions:
    protected:
        virtual void _clonedata(const ebmlElement*) = 0;

    public:
        ebmlElement_sp clone() const;

        // Repr function:
        virtual std::wstring minirepr() const = 0;
        virtual std::wstring repr() const;
    };

    class ebmlVoid;

    class ebmlVoidClass : public ebmlElementClass {
    public:
        ebmlVoidClass();
        ebmlElement_sp operator()(size_t) const;

    protected:
        ebmlElement* _new() const;
        friend class ebmlVoid;
    };

    class ebmlVoid : public ebmlElement {
    protected:
        ebmlVoid(const ebmlVoidClass*);
        ebmlVoid(const ebmlVoidClass*, size_t);

    public:
        size_t voidsize;
        size_t dataSize() const;

        // Encode functions.
    protected:
        size_t _encode(char*) const;

    public:
        size_t encode(ioBase*) const;

        // Decode functions:
    protected:
        void _decode(const parseString&);
        void _decode(const parseFile&);

        // Cloning functions:
    protected:
        void _clonedata(const ebmlElement*);

    public:
        std::wstring minirepr() const;
        std::wstring repr() const;

        friend class ebmlVoidClass;
    };

    static ebmlVoidClass Void;

    class ebmlDocument : std::enable_shared_from_this<ebmlDocument> {
    private:
        FILE* _file;
        int _fd;
        const ebmlSchema* _schema;
        ebmlElement_sp _root;
    public:
        ebmlElement_sp root() const;
        void setRoot(ebmlElement_sp);
        void setRoot(ebmlElement*);

        const ebmlSchema* schema() const;
    };

}
// #include "libebml_ng/base.tpp"
#endif
