#ifndef EBML_NG_PARSESTR_H
#define EBML_NG_PARSESTR_H

#include <stddef.h>
#include <unistd.h>

#include "libebml_ng/ebmlID_t.h"
#include "libebml_ng/parsing/io.h"

namespace ebml {
    class parseString {
    public:
        ebmlID_t ebmlID;
        vintWidth_t ebmlIDWidth;
        size_t dataSize;
        vintWidth_t sizeWidth;
        off_t offset; // Offset relative to start of parent data.
        const char* data;
        parseString* parent;

        parseString();
        parseString(const parseString&);
        parseString(parseString&&);
        parseString& operator=(const parseString&);
        parseString& operator=(parseString&&);
        // parseString(const char*);
        // parseString(const char*, off_t);
        parseString(const char*, size_t);
        parseString(const char*, size_t, off_t);

        parseString(
            ebmlID_t, vintWidth_t,
            size_t, vintWidth_t,
            off_t, const char*);
        parseString(const parseFile&, char*);

        class iterator;
        parseString::iterator begin() const;
        unsigned long long outerSize() const;

        template<typename T>
        T unpack() const {
            return ebml::unpack<T>(this->data, this->dataSize);
        }
    };

    class parseString::iterator {
        const char* _data;
        off_t _offset;
        size_t _dataSize;
        parseString _next;
    public:
        iterator(const char*, size_t);
        parseString operator*();
        parseString::iterator& operator++();
        parseString::iterator operator++(int);
        bool operator==(const parseString::iterator&) const;
        bool operator!=(const parseString::iterator&) const;
        bool atEnd() const;
    };

    extern template unsigned long long parseString::unpack<unsigned long long>() const;
    extern template long long parseString::unpack<long long>() const;
    extern template double parseString::unpack<double>() const;
    extern template std::string parseString::unpack<std::string>() const;
    extern template std::wstring parseString::unpack<std::wstring>() const;
}
#endif
