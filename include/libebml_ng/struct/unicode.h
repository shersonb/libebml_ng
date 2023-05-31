#ifndef EBML_NG_STRUCT_UNICODE_H
#define EBML_NG_STRUCT_UNICODE_H

#include "libebml_ng/struct.h"
#pragma message("unicode.h included")

namespace ebml {
    // struct ebmlStringData {
    //     unichar* data;
    //     unsigned long long size;
    //     unsigned long long bufsize;
    //     ebmlStringData(void);
    //     ebmlStringData(unsigned long long);
    //     ~ebmlStringData(void);
    //     void resize(unsigned long long bufsize);
    //     void setData(const unichar*, unsigned long long);
    // };
    //
    // struct ebmlStringDataInitArgs {
    //     unichar* data;
    //     unsigned long long size;
    // };
    //
    // class ebmlString : public ebmlElementClass {
    //     void _init(ebmlElement*, const void*) const;
    //     void _init(ebmlElement*, const ebmlStringDataInitArgs*) const;
    //     void _create(ebmlElement*) const;
    //     void _destroy(ebmlElement*) const;
    //     void _clone(const ebmlPayload&, ebmlElement*) const;
    //     void _decode(ebmlElement*, const parseStringData&) const;
    //     unsigned long long _encode(const ebmlElement*, char*, unsigned long long) const;
    //     unsigned long long _size(const ebmlElement*) const;
    // public:
    //     ebmlString(unsigned long long _ebmlID, std::string _name) : ebmlElementClass(_ebmlID, _name) {};
    // };
    typedef char32_t unichar;

    class unicode : public std::u32string {
    protected:
        union {
            size_t charcount[4];
            struct {
                size_t bmp;
                size_t smp;
                size_t sip;
                size_t sup;
            } planes;
        };
    public:
        unicode();
        unicode(const std::u32string&);
        unicode(const unicode&);
        unicode(const unichar*);

        unicode& operator=(const std::u32string&);
        unicode& operator=(const unicode&);
        unicode& operator=(const unichar*);

        unicode& append(const std::u32string& str);
        unicode& append(const std::u32string& str, size_t subpos, size_t sublen);
        unicode& append(const unichar* s, size_t n);
        unicode& append(const unichar* s);
        unicode& append(size_t n, unichar c);
        unicode& append(const unichar* first, const unichar* last);

        unicode& assign(const std::u32string& str);
        unicode& assign(const std::u32string& str, size_t subpos, size_t sublen);
        unicode& assign(const unichar* s, size_t n);
        unicode& assign(const unichar* s);
        unicode& assign(size_t n, unichar c);
        unicode& assign(const unichar* first, const unichar* last);

        unicode& insert(size_t pos, const std::u32string& str);
        unicode& insert(size_t pos, const std::u32string& str, size_t subpos, size_t sublen);
        unicode& insert(size_t pos, const unichar* s, size_t n);
        unicode& insert(size_t pos, const unichar* s);
        unicode& insert(size_t pos, size_t n, unichar c);

        unicode::iterator erase(std::u32string::iterator p);
        unicode::iterator erase(std::u32string::iterator first, std::u32string::iterator last);

        void push_back(unichar c);

        unicode& replace(size_t pos, size_t len, const std::u32string& str);
        unicode& replace(size_t pos, size_t len, const std::u32string& str, size_t subpos, size_t sublen);
        unicode& replace(size_t pos, size_t len, const unichar* s, size_t n);
        unicode& replace(size_t pos, size_t len, const unichar* s);
        unicode& replace(size_t pos, size_t len, size_t n, unichar c);
        unicode& replace(iterator i1, iterator i2, const unichar* j1, const unichar* j2);
    };

    size_t size(const unicode&);

    size_t pack(const unicode&, size_t, char*);

    template<>
    unicode unpack<unicode>(const char*, size_t);

//     class ebmlElementClassBase;
//
//     template<typename T>
//     class ebmlElementClass;

//     template<>
//     class ebmlElementClass<unicode> : public ebmlElementClassBase {
//     public:
// //         ebmlElementClass(ebmlID_t, const std::u32string&, const std::u32string&);
//         ebmlElementClass(ebmlID_t _ebmlID, const std::u32string& _name, const std::u32string& _defaultval) : ebmlElementClassBase(_ebmlID, _name) {};
//         virtual ebmlElement_sp operator()(const std::u32string&) const;
//     };
//
//     template<>
//     class ebmlElement<unicode> : public ebmlElementBase {
//         // Constructor/Destructor
//     protected:
//         ebmlElement(const ebmlElementClass<unicode>*, const std::u32string&);
//     };

//     class ebmlUnicode;
//
//     class ebmlUnicodeClass : public ebmlElementClass<unicode> {
//     public:
//         ebmlUnicodeClass(unsigned long long _ebmlID, std::string _name) : ebmlElementClass(_ebmlID, _name) {};
//         std::shared_ptr<ebmlElementBase> operator()() const override;
//         std::shared_ptr<ebmlElementBase> operator()(const unichar*, unsigned long long) const;
//         std::shared_ptr<ebmlElementBase> operator()(std::u32string&) const;
//         friend class ebmlUnicode;
//     };
//
//     class ebmlUnicode : public ebmlElement<unicode> {
//         void _clonedata(const ebmlElementBase*);
//         ebmlUnicode(const ebmlUnicodeClass*);
//         ebmlUnicode(const ebmlUnicodeClass*, const unichar*, size_t);
//         ebmlUnicode(const ebmlUnicodeClass*, const std::u32string&);
// //         ebmlUnicode(const ebmlUnicodeClass*, const unicode&); // Already implemented in template parent class.
// //         void _decode(const parseStringData&);
// //         size_t _encode(char* dest, unsigned long long dataSize) const;
//     public:
// //         unicode data;
// //         size_t dataSize() const;
// //         ~ebmlUnicode();
//         friend class ebmlUnicodeClass;
//     };
}
#endif
