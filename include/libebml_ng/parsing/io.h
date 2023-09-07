#ifndef EBML_NG_PARSING_IO_H
#define EBML_NG_PARSING_IO_H

#include "libebml_ng/ebmlID_t.h"
#include "libebml_ng/io.h"
// #include "libebml_ng/parsing/string.h"
#include <memory>

namespace ebml {
    class parseFile {
    public:
        parseFile(ioBase*);
        parseFile(ioBase*, parseFile&);
        parseFile(ioBase*, off_t);
        parseFile(ioBase*, off_t, parseFile&);

        parseFile(ioBase_sp&);
        parseFile(ioBase_sp&, parseFile&);
        parseFile(ioBase_sp&, off_t);
        parseFile(ioBase_sp&, off_t, parseFile&);

        ebmlID_t ebmlID;
        unsigned char ebmlIDWidth;
        size_t dataSize;
        unsigned char sizeWidth;
        off_t offset;
        parseFile* parent;

        off_t dataOffset() const;
        off_t endOffset() const;
        size_t read(char*) const;
        size_t read(char*, size_t) const;
        size_t read(char*, off_t, size_t) const;
        off_t seek(off_t) const;
        off_t tell() const;
        size_t outerSize() const;

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

            iterator(ioBase_sp& file);
            iterator(ioBase_sp& file, off_t end);
            iterator(ioBase_sp& file, off_t start, off_t end);
            iterator(ioBase_sp& file, off_t end, parseFile&);
            iterator(ioBase_sp& file, off_t start, off_t end, parseFile&);

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
}
#endif
