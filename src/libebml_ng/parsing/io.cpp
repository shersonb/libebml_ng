#ifndef EBML_NG_PARSING_IO_CPP
#define EBML_NG_PARSING_IO_CPP

#include "libebml_ng/parsing/string.h"
#include "libebml_ng/parsing/io.h"
#include "libebml_ng/vint.cpp"
#include "libebml_ng/exceptions.cpp"
#include <algorithm>

namespace ebml {
//     struct parseFileState {
//         ebmlID_t ebmlID;
//         unsigned char ebmlIDWidth;
//         size_t dataSize;
//         unsigned char sizeWidth;
//         off_t offset;
//         ioBase* _file;
//         bool _seekable;
//         size_t bytes_read;
//         unsigned int instance_count;
//         unsigned int iter_count;
//         parseFile* parent;
//         parseFileState() {
//             this->instance_count = 1;
//             this->bytes_read = 0;
//             this->iter_count = 0;
//         }
//     };

    size_t parseFile::outerSize() const {
        return this->ebmlIDWidth + this->sizeWidth + this->dataSize;
    }

    parseFile::parseFile(ioBase* file) {
        char buffer[16];
        off_t _offset = file->tell();
        this->_init(file, _offset, buffer);
    }

    parseFile::parseFile(ioBase* file, off_t _offset) {
        char buffer[16];
        this->_init(file, _offset, buffer);
    }

    parseFile::parseFile(ioBase* file, char* buffer) {
        off_t _offset = file->tell();
        this->_init(file, _offset, buffer);
    }

    parseFile::parseFile(ioBase* file, off_t _offset, char* buffer) {
        this->_init(file, _offset, buffer);
    }

    parseFile::parseFile(ioBase* file, parseFile_sp& _parent) : parseFile(file) {
        this->parent = _parent;
    }

    parseFile::parseFile(ioBase* file, off_t _offset, parseFile_sp& _parent) : parseFile(file, _offset) {
        this->parent = _parent;
    }

    parseFile::parseFile(ioBase* file, char* buffer, parseFile_sp& _parent) : parseFile(file, buffer) {
        this->parent = _parent;
    }

    parseFile::parseFile(ioBase* file, off_t _offset, char* buffer, parseFile_sp& _parent) : parseFile(file, _offset, buffer) {
        this->parent = _parent;
    }

    void parseFile::_init(ioBase* file, off_t _offset, char* buffer) {
        this->_file = file;
        this->_seekable = file->seekable();
        this->offset = _offset;

        this->ebmlID = unpackVint(file, buffer, &(this->ebmlIDWidth));

        if (this->ebmlIDWidth == 0) {
            this->sizeWidth = 0;
            return;
        }

        this->dataSize = unpackVint(file, buffer + this->ebmlIDWidth, &(this->sizeWidth));

        if (this->sizeWidth == 0) {
            throw ebmlException("Unexpected end of data.", __LINE__, __FILE__);
        }
    }

    size_t parseFile::read(char* dest) {
        if (this->bytes_read >= this->dataSize) {
            return 0;
        }

        return this->read(dest, this->dataSize - this->bytes_read);
    }

    size_t parseFile::read(char* dest, size_t size) {
        if (this->_seekable) {
            return this->_read_seekable(dest, size);
        }
        return this->_read_unseekable(dest, size);
    }

    size_t parseFile::_read_unseekable(char* dest, size_t size) {
        if (this->bytes_read >= this->dataSize) {
            return 0;
        }

        size_t remaining = this->dataSize - this->bytes_read;
        size = std::min(size, remaining);

        size_t _bytes_read = this->_file->read(dest, size);
        this->bytes_read += _bytes_read;

        if (_bytes_read < remaining) {
            throw ebmlException("Unexpected end of data.", __LINE__, __FILE__);
        };

        return _bytes_read;
    }

    size_t parseFile::_read_seekable(char* dest, size_t size) {
        if (this->bytes_read >= this->dataSize) {
            return 0;
        }

        off_t absoffset = this->offset + this->ebmlIDWidth + this->sizeWidth + this->bytes_read;
        size_t remaining = this->dataSize - this->bytes_read;
        size = std::min(size, remaining);

        size_t _bytes_read = this->_file->read(dest, absoffset, size);
        this->bytes_read += _bytes_read;

        if (_bytes_read < remaining) {
            throw ebmlException("Unexpected end of data.", __LINE__, __FILE__);
        };

        return _bytes_read;
    }

//     iterParseFile parseFile::begin() const {
//         if (this->_seekable > 0) {
//             if (this->iter_count > 0) {
//                 throw std::runtime_error("begin() can only be called once on unseekable streams.");
//             }
//             if (this->bytes_read > 0) {
//                 throw std::runtime_error("begin() can only be called on unseekable streams when no data has been read.");
//             }
//         }
//
//         ++this->iter_count;
//
//         if (this->dataSize < 0xffffffffffffffff) {
//             return iterParseFile(
//                 this->_file, this->offset, this->offset + this->dataSize);
//         } else {
//             return iterParseFile(this->_file, this->offset);
//         }
//     }
//
//     off_t parseFile::dataOffset() const {
//         return this->offset + this->ebmlIDWidth + this->sizeWidth;
//     }
//
//     void parseFile::_consume_remaining() {
//         char buffer[65536];
//         size_t data_read;
//
//         while (this->bytes_read < this->dataSize) {
//             data_read = this->_read_unseekable(buffer, 65536);
//
//             if (data_read == 0) {
//                 throw std::runtime_error("Unexpected end of data.");
//             }
//         }
//     }
//
//     parseFile::~parseFile() {
//         this->_decref();
//     }
//
//     void parseFile::_incref() {
//         if (this != nullptr) {
//             ++this->instance_count;
//         }
//     }
//
//     void parseFile::_cleanup() {
//         if (!this->_seekable) {
//             if (std::uncaught_exception()) {
//                 if (this->bytes_read < this->dataSize) {
//                     this->_consume_remaining();
//                 }
//
//                 if (this->parent != nullptr) {
//                     off_t startOfParentData = this->parent->dataOffset();
//                     off_t current = this->dataOffset() + this->bytes_read;
//                     this->parent->_update_bytes_read(current - startOfParentData);
//                 }
//             }
//         }
//
//     }
//
//     void parseFile::_decref() {
//         if (this != nullptr) {
//             if (--this->instance_count == 0) {
//                 this->_cleanup();
//                 delete this;
//             }
//         }
//     }
//
//     parseFile::parseFile(const parseFile& other) {
//         this = other._state;
//         this->_incref();
//     }
//
//     parseFile& parseFile::operator=(const parseFile& other) {
//         if (this != other._state) {
//             this->_decref();
//             this = other._state;
//             this->_incref();
//         }
//         return *this;
//     }
//
//     iterParseFile& iterParseFile::operator++() {
//         if (this->_nextoffset == this->_offset) {
//             auto parse = **this;
//             this->_nextoffset += parse.outerSize();
//         }
//         this->_offset = this->_nextoffset;
//         return *this;
//     }
//
//     iterParseFile iterParseFile::operator++(int) {
//         auto old = *this;
//         ++(*this);
//         return old;
//     }

//     iterParseFile::iterParseFile() {}
// 
//     iterParseFile::iterParseFile(FILE* file) {
//         this->_file = file;
//         this->_next = parseOnce(file);
//         this->_offset = this->_next.offset;
//         this->_startoffset = this->_offset;
//         this->_endoffset = 0xffffffffffffffff;
//     }
// 
//     iterParseFile::iterParseFile(
//             FILE* file, unsigned long long endoffset) {
//         this->_file = file;
//         this->_next = parseOnce(file);
//         this->_offset = this->_next.offset;
//         this->_startoffset = this->_offset;
//         this->_endoffset = endoffset;
// 
//     //     if (this->_offset < endoffset) {
//     //         this->_next = parseOnce(this->_file, &(this->buffer[0]));
//     //     }
//     }
// 
//     iterParseFile::iterParseFile(
//             FILE* file, unsigned long long startoffset,
//             unsigned long long offset, unsigned long long endoffset) {
//         this->_file = file;
//     //     this->_next = parseOnce(file);
//         this->_startoffset = startoffset;
//         this->_offset = offset;
//         this->_endoffset = endoffset;
// 
//         if (offset < endoffset) {
//             fseek(file, offset, SEEK_SET);
//             this->_next = parseOnce(this->_file, &(this->buffer[0]));
//         }
//     }
// 
//     parseFileData iterParseFile::operator*(void) {
//         unsigned long long size = this->_next.outerSize();
// 
//         if (size < 0xffffffffffffffff
//             and this->_next.offset + size > this->_endoffset) {
//                 throw ebmlException("Element extends past expected end of data.", __LINE__, __FILE__);
//             }
//         return this->_next;
//     }
// 
//     iterParseFile& iterParseFile::operator++(void) {
//         if (this->_offset >= this->_endoffset) {
//             throw ebmlException("Stop Iteration.", __LINE__, __FILE__);
//         }
// 
//         this->_offset += this->_next.outerSize();
//         fseek(this->_file, this->_offset, SEEK_SET);
// 
//         if (this->_offset < this->_endoffset) {
//             this->_next = parseOnce(this->_file, &(this->buffer[0]));
//         }
//         return *this;
//     }
// 
//     iterParseFile iterParseFile::operator++(int) {
//         iterParseFile i = *this;
// 
//             if (this->_offset >= this->_endoffset) {
//             throw ebmlException("Stop Iteration.", __LINE__, __FILE__);
//         }
// 
//         this->_offset += (
//             this->_next.ebmlIDWidth + this->_next.sizeWidth + this->_next.dataSize);
// 
//         if (this->_offset < this->_endoffset) {
//             this->_next = parseOnce(this->_file, &(this->buffer[0]));
//         }
//         return i;
// 
//     }
// 
//     bool iterParseFile::operator==(const iterParseFile& other) const {
//         if (this->_file != other._file) {
//             return false;
//         }
//         if (this->_endoffset != other._endoffset) {
//             return false;
//         }
//         if ((this->_offset >= this->_endoffset)
//                 and (other._offset >= other._endoffset)) {
//             return true;
//         }
//         return (this->_offset == other._offset);
//     }
// 
//     bool iterParseFile::operator!=(const iterParseFile& other) const {
//         if (this->_file != other._file) {
//             return true;
//         }
//         if (this->_endoffset != other._endoffset) {
//             return true;
//         }
//         if ((this->_offset >= this->_endoffset)
//                 and (other._offset >= other._endoffset)) {
//             return false;
//         }
//         return (this->_offset != other._offset);
//     }
// 
//     parseFileData parseOnce(FILE* file) {
//         char buffer[16];
//         return parseOnce(file, &buffer[0]);
//     }
//     parseFileData parseOnce(FILE* file, char* buffer) {
//         parseFileData ret;
// 
//         ret.offset = ftell(file);
//         ret.ebmlID = unpackVint(file, buffer, &ret.ebmlIDWidth);
// 
//         if (ret.ebmlIDWidth == 0) {
//             return ret;
//         }
// 
//         ret.dataSize = unpackVint(file, buffer + ret.ebmlIDWidth, &ret.sizeWidth);
// 
//         if (ret.sizeWidth == 0) {
//             throw ebmlException("Unexpected end of data.", __LINE__, __FILE__);
//         }
// 
//         ret.file = file;
//         return ret;
//     }
// 
//     parseStringData parseFileData::to_parseStringData(char* buffer) const {
//         this->read(buffer);
//         return parseStringData(
//             this->ebmlID, this->ebmlIDWidth,
//             this->dataSize, this->sizeWidth,
//             this->offset, buffer);
//     }
}
#endif
