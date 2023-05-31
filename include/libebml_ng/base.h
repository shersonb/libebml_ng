#ifndef EBML_NG_BASE_H
#define EBML_NG_BASE_H

#include "stdio.h"
#include <iostream>
#include <exception>
#include <memory>
#include <mutex>

#include "libebml_ng/parsing/string.h"
#include "libebml_ng/parsing/io.h"
#include "exceptions.h"
#include "struct.h"
#include "vint.h"
#include "ebmlID_t.h"

namespace ebml {
    class ebmlElementBase;
    class ebmlDocument;
    class ebmlSchema;

    typedef std::shared_ptr<ebmlElementBase> ebmlElement_sp;
    typedef std::weak_ptr<ebmlElementBase> ebmlElement_wp;
    typedef std::shared_ptr<ebmlDocument> ebmlDocument_sp;
    typedef std::weak_ptr<ebmlDocument> ebmlDocument_wp;

    class ebmlElementClassBase {
        friend class ebmlElementBase;
    public:
        ebmlElementClassBase(ebmlID_t, const std::u32string&);
        ebmlID_t ebmlID;
        std::u32string name;
        virtual ebmlElement_sp operator()() const = 0;

        // Decode methods taking parse data classes.
        ebmlElement_sp decode(const parseString&) const;
        ebmlElement_sp decode(const parseFile_sp&) const;

        // Decode methods applied directly to character and file-like objects.
        ebmlElement_sp decode(const char*, size_t) const;
        ebmlElement_sp decode(const std::string&) const;
        ebmlElement_sp decode(ioBase*) const;

        // Repr function
        virtual std::u32string repr() const;
    };

//     template<typename T>
//     class ebmlElement;
//
//     template<typename T>
//     class ebmlElementClass : public ebmlElementClassBase {
//         friend class ebmlElement<T>;
//     public:
//         T defaultval;
//         ebmlElementClass(ebmlID_t, const std::u32string&);
//         ebmlElementClass(ebmlID_t, const std::u32string&, const T&);
//         virtual ebmlElement_sp operator()() const;
//         virtual ebmlElement_sp operator()(const T&) const;
//     };
//
    class ebmlElementBase : std::enable_shared_from_this<ebmlElementBase> {
        friend class ebmlElementClassBase;

        // Constructor/Destructor
    protected:
        ebmlElementBase(const ebmlElementClassBase*);

    public:
        virtual ~ebmlElementBase();

        // class and ID
    protected:
        const ebmlElementClassBase* _cls;

    public:
        const ebmlElementClassBase* cls() const;
        unsigned long long ebmlID() const;

        // Hierarchy members.
    public:
        ebmlElement_sp parent() const;
        ebmlElement_sp root() const;
        ebmlDocument_sp document() const;

    private:
        bool _weakparent{};
        union {
            ebmlElement_sp _parent;
            ebmlElement_wp _parent_weak;
        };
        ebmlDocument_wp _document;

    protected:
        void _setParent(ebmlElement_sp parent, bool weak = true);
        bool _hasParent() const;
        void _detach();

        // Size functions.
    public:
        unsigned char sizeWidth;
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
        virtual size_t _encode(char*) const = 0;
//         virtual size_t _encode(char*, size_t) const = 0; // May be deprecated

    public:
        std::string encode() const;
        size_t encode(char*) const;
        size_t encode(char*, size_t) const;

        // Decode functions:
    protected:
        virtual void _decode(const parseString&) = 0;
        virtual void _decode(const parseFile_sp&);

        // Cloning functions:
    protected:
        virtual void _clonedata(const ebmlElementBase*) = 0;

    public:
        ebmlElement_sp clone() const;

        // Repr function:
        virtual std::u32string repr() const;
    };

//     template<typename T>
//     class ebmlElement : public ebmlElementBase {
//         friend class ebmlElementClass<T>;
//
//         // Constructor/Destructor
//     protected:
//         ebmlElement(const ebmlElementClass<T>*, const T&);
//
//     public:
//         T data;
//
//         // Size functions.
//     public:
//         virtual size_t dataSize() const;
//
//         // Encode functions.
//     protected:
//         virtual size_t _encode(char*) const;
// //         virtual size_t _encode(char*, unsigned long long) const;
//
//         // Decode functions:
//     protected:
//         virtual void _decode(const parseString&);
//         virtual void _decode(const parseFile_sp&);
//
//         // Cloning functions:
//     protected:
//         virtual void _clonedata(const ebmlElementBase*);
//
//     };
//
//     class ebmlDocument : std::enable_shared_from_this<ebmlDocument> {
//     private:
//         FILE* _file;
//         int _fd;
//         const ebmlSchema* _schema;
//         ebmlElement_sp _root;
//     public:
//         ebmlElement_sp root() const;
//         void setRoot(ebmlElement_sp);
//         void setRoot(ebmlElementBase*);
//
//         const ebmlSchema* schema() const;
//     };
    class ebmlDocument : std::enable_shared_from_this<ebmlDocument> {
    private:
        FILE* _file;
        int _fd;
        const ebmlSchema* _schema;
        ebmlElement_sp _root;
    public:
        ebmlElement_sp root() const;
        void setRoot(ebmlElement_sp);
        void setRoot(ebmlElementBase*);

        const ebmlSchema* schema() const;
    };
}
// #include "libebml_ng/base.tpp"
#endif
