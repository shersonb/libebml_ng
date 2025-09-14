#ifndef EBML_NG_PARSING_IO_H
#define EBML_NG_PARSING_IO_H

#include <memory>

#include "libebml_ng/ebmlID_t.h"
#include "libebml_ng/io.h"
#include "libebml_ng/struct.h"
#include "libebml_ng/struct/unicode.h"

namespace ebml {
    /**
     * @brief Represents a parsed EBML file segment.
     *
     * The parseFile class encapsulates the data read from an IO interface (ioBase)
     * for an EBML element. It stores the EBML ID, widths, data size, file offset,
     * and parent relations. Underlying IO operations (such as seek, read, write,
     * or truncate) may throw exceptions derived from std::ios_base::failure (or
     * other exceptions such as ebmlUnexpectedEndOfData and ebmlDecodeError) if
     * errors occur in the IO operations.
     *
     * @throws std::ios_base::failure Thrown if seeking, reading, writing, or truncating
     *    fails due to an underlying IO error.
     * @throws std::invalid_argument Thrown by the specialized IO implementations (such as
     *    when no read, write, or append flag is specified).
     * @throws ebml::ebmlUnexpectedEndOfData Thrown if a parsed element header indicates more data
     *    than available.
     * @throws ebml::ebmlDecodeError Thrown if there is an error in decoding the EBML header fields.
     *
     * @ingroup Parsing
     */
    class parseFile {
    public:
        parseFile(ioBase&);
        parseFile(ioBase&, parseFile&);
        parseFile(ioBase&, off_t);
        parseFile(ioBase&, off_t, parseFile&);

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

        /**
         * @brief Template method to unpack data from the element read from file.
         *
         * This method reads the element's data into a temporary buffer and then converts it
         * to a value of the requested type using ebml::unpack.
         *
         * @tparam T The target type.
         * @return The unpacked value of type T.
         * @throws std::ios_base::failure if an IO error occurs during the read.
         */
        template<typename T>
        inline T unpack() const {
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
        /**
         * @brief Iterator class for parseFile.
         *
         * Provides sequential access to the EBML elements contained within the file segment.
         * IO exceptions (such as std::ios_base::failure or ebmlDecodeError) may be thrown
         * by dereferencing or incrementing the iterator if underlying IO errors occur.
         *
         * @ingroup Parsing
         */
        class iterator {
        public:
            iterator(ioBase* file);
            iterator(ioBase* file, off_t end);
            iterator(ioBase* file, off_t start, off_t end);
            iterator(ioBase* file, off_t end, parseFile&);
            iterator(ioBase* file, off_t start, off_t end, parseFile&);

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
