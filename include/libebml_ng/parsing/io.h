#ifndef EBML_NG_PARSING_IO_H
#define EBML_NG_PARSING_IO_H

#include <memory>

#include "libebml_ng/ebmlID_t.h"
#include "libebml_ng/io.h"
#include "libebml_ng/struct.h"

namespace ebml {
    class parseFile {
    public:
        parseFile(ioBase&);
        parseFile(ioBase&, parseFile&);
        parseFile(ioBase&, off_t);
        parseFile(ioBase&, off_t, parseFile&);

        // parseFile(const ioBase_sp&);
        // parseFile(const ioBase_sp&, parseFile&);
        // parseFile(const ioBase_sp&, off_t);
        // parseFile(const ioBase_sp&, off_t, parseFile&);

        ebmlID_t ebmlID;
        unsigned char ebmlIDWidth;
        size_t dataSize;
        unsigned char sizeWidth;
        off_t offset;
        off_t parentOffset = 0;
        parseFile* parent;

        off_t dataOffset() const;
        off_t endOffset() const;
        size_t read(char*) const;
        size_t read(char*, size_t) const;
        size_t read(char*, off_t, size_t) const;
        off_t seek(off_t) const;
        off_t tell() const;
        size_t outerSize() const;

        template<typename T>
        T unpack() const {
            auto buffer_sp = std::make_unique<char[]>(this->dataSize);
            auto buffer = buffer_sp.get();
            this->read(buffer, 0, this->dataSize);
            return ebml::unpack<T>(buffer, this->dataSize);
        }

    protected:
        // For use by parseFile::iterator, just initializes the member variables and seeks file to beginning of data.
        parseFile(ioBase*, ebmlID_t, unsigned char, size_t, unsigned char, off_t);
        parseFile(ioBase*, ebmlID_t, unsigned char, size_t, unsigned char, off_t, parseFile&);

    public:
        class iterator {
        public:
            iterator(ioBase* file);
            iterator(ioBase* file, off_t end);
            iterator(ioBase* file, off_t start, off_t end);
            iterator(ioBase* file, off_t end, parseFile&);
            iterator(ioBase* file, off_t start, off_t end, parseFile&);

            // iterator(ioBase_sp& file);
            // iterator(ioBase_sp& file, off_t end);
            // iterator(ioBase_sp& file, off_t start, off_t end);
            // iterator(ioBase_sp& file, off_t end, parseFile&);
            // iterator(ioBase_sp& file, off_t start, off_t end, parseFile&);

            parseFile operator*();
            parseFile::iterator& operator++();
            // parseFile::iterator operator++(int); Postincrement not supported
            bool atEnd();
        private:
            // ioBase_sp _file;
            ioBase* _file;
            parseFile* _parent = nullptr;
            off_t _startoffset;
            off_t _endoffset;

            // Necessary information to dereference/construct parseFile object.
            ebmlID_t _ebmlID;
            unsigned char _ebmlIDWidth;
            size_t _dataSize;
            unsigned char _sizeWidth;
            off_t _offset;

        };

        iterator begin() const;
    private:
        mutable ioBase* _file;

        friend class parseFile::iterator;
    };

    extern template unsigned long long parseFile::unpack<unsigned long long>() const;
    extern template long long parseFile::unpack<long long>() const;
    extern template double parseFile::unpack<double>() const;
    extern template std::string parseFile::unpack<std::string>() const;
    extern template std::wstring parseFile::unpack<std::wstring>() const;
}
#endif
