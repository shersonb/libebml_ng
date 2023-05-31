#ifndef EBML_NG_PARSESTR_H
#define EBML_NG_PARSESTR_H

#include <stddef.h>
#include <unistd.h>

#include "libebml_ng/ebmlID_t.h"
#include "libebml_ng/parsing/io.h"

namespace ebml {
    class iterParseString;

    // class parseData {
    // public:
    //     unsigned long long ebmlID;
    //     unsigned char ebmlIDWidth;
    //     unsigned long long dataSize;
    //     unsigned char sizeWidth;
    //     parseData* parent;
    //     unsigned long long outerSize() const;
    //     parseData() = 0;
    //     unsigned long long as_uint() const = 0;
    //     long long as_int() const = 0;
    //     double as_double() const = 0;
    //     virtual const char* read_chars(char*, &size_t) = 0;
    //     virtual const char* read_chars(char*, size_t, &size_t) = 0;
    //     size_t bytes_read() const;
    //     virtual iterParse begin() const = 0;
    // };

    // class parseString : public parseData {
    // public:
    //     const char* data
    //     off_t offset
    //     parseData(const char*, size_t size)
    //     parseData(const char*, off_t offset, size_t size)
    //     unsigned long long as_uint() const;
    //     long long as_int() const;
    //     double as_double() const;
    //     iterParse begin() const;
    // }


    class parseString {
    public:
        ebmlID_t ebmlID;
        unsigned char ebmlIDWidth;
        size_t dataSize;
        unsigned char sizeWidth;
        off_t offset;
        const char* data;
        parseString* parent;
        parseString();

        parseString(const char*);
        parseString(const char*, off_t);
        parseString(const char*, size_t);
        parseString(const char*, size_t, off_t);

        parseString(
            unsigned long long, unsigned char,
            size_t, unsigned char,
            off_t, const char*);
        parseString(const parseFile_sp&, char*);

        iterParseString begin() const;
        unsigned long long outerSize() const;
    };

    class iterParseString {
        const char* _data;
        off_t _offset;
        size_t _dataSize;
        parseString _next;
    public:
    //     iterParseString();
        iterParseString(const char*, size_t);
    //     iterParseString(const char*, unsigned long long, unsigned long long);
        parseString operator*();
        iterParseString& operator++();
        iterParseString operator++(int);
        bool operator==(const iterParseString&) const;
        bool operator!=(const iterParseString&) const;
        bool atEnd();
    };
}
#endif
