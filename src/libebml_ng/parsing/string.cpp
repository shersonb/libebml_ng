#ifndef EBML_NG_PARSING_STRING_CPP
#define EBML_NG_PARSING_STRING_CPP

#include "libebml_ng/parsing/string.h"
#include "libebml_ng/vint.cpp"
#include "libebml_ng/exceptions.cpp"

namespace ebml {
    parseString::parseString() {
        this->ebmlID = 0;
        this->ebmlIDWidth = 0;
        this->dataSize = 0;
        this->sizeWidth = 0;
        this->offset = 0;
        this->data = NULL;
    }

    parseString::parseString(
            unsigned long long ebmlID, unsigned char ebmlIDWidth,
            size_t dataSize, unsigned char sizeWidth,
            off_t offset, const char* data) {
        this->ebmlID = ebmlID;
        this->ebmlIDWidth = ebmlIDWidth;
        this->dataSize = dataSize;
        this->sizeWidth = sizeWidth;
        this->offset = offset;
        this->data = data;
    }

//     unsigned long long parseString::as_uint() const {
//         return unpackU(this->data, this->dataSize);
//     }
// 
//     long long parseString::as_int() const {
//         return unpackS(this->data, this->dataSize);
//     }
// 
//     double parseString::as_double() const {
//         if (this->dataSize == 8) {
//             return unpackD(this->data);
//         }
//         if (this->dataSize == 4) {
//             return (double)unpackF(this->data);
//         }
//         if (this->dataSize == 0) {
//             return 0;
//         }
//         throw ebmlException("Unsupported dataSize.", __LINE__, __FILE__);
//     }

    parseString::parseString(const char* data, size_t size) {
        unsigned long long outerSize;

        this->ebmlID = unpackVint(data, size, &this->ebmlIDWidth);
        this->dataSize = unpackVint(
            data + this->ebmlIDWidth, size - this->ebmlIDWidth, &this->sizeWidth);

        outerSize = this->ebmlIDWidth + this->sizeWidth + this->dataSize;

        if (size < outerSize) {
            throw ebmlException("Unexpected end of data.", __LINE__, __FILE__);
        }

        this->offset = 0;
        this->data = data + this->ebmlIDWidth + this->sizeWidth;
    }

    parseString::parseString(const char* data, size_t size, off_t offset) {
        this->ebmlID = unpackVint(data, size, &this->ebmlIDWidth);
        this->dataSize = unpackVint(
            data + this->ebmlIDWidth, size - this->ebmlIDWidth, &this->sizeWidth);

        if (size < this->ebmlIDWidth + this->sizeWidth + this->dataSize) {
            throw ebmlException("Unexpected end of data.", __LINE__, __FILE__);
        }

        this->offset = offset;
        this->data = data + this->ebmlIDWidth + this->sizeWidth;
    }

    iterParseString parseString::begin() const {
        return iterParseString(this->data, this->dataSize);
    }

//     iterParseString parseString::end() const {
//         return iterParseString(this->data, this->dataSize, this->dataSize);
//     }

    unsigned long long parseString::outerSize() const {
        return this->ebmlIDWidth + this->sizeWidth + this->dataSize;
    }

//     iterParseString::iterParseString() {}

    iterParseString::iterParseString(const char* data, size_t size) {
        this->_data = data;
        this->_offset = 0;
        this->_dataSize = size;
        this->_next = parseString(this->_data, this->_dataSize);
    }

//     iterParseString::iterParseString(
//             const char* data, unsigned long long size, unsigned long long offset) {
//         this->_data = data;
//         this->_offset = offset;
//         this->_dataSize = size;
// 
//         if (offset < size) {
//             this->_next = parseOnce(this->_data, offset, this->_dataSize);
//         }
//     }
// 
    parseString iterParseString::operator*(void) {
        return this->_next;
    }

    iterParseString& iterParseString::operator++(void) {
        if (this->_offset >= this->_dataSize) {
            throw ebmlException("Stop Iteration.", __LINE__, __FILE__);
        }

        this->_offset += (
            this->_next.ebmlIDWidth + this->_next.sizeWidth + this->_next.dataSize);

        if (this->_offset < this->_dataSize) {
            this->_next = parseString(
                this->_data + this->_offset, this->_offset, this->_dataSize - this->_offset);
        }
        return *this;
    }

    iterParseString iterParseString::operator++(int) {
        iterParseString i = *this;

            if (this->_offset >= this->_dataSize) {
            throw ebmlException("Stop Iteration.", __LINE__, __FILE__);
        }

        this->_offset += (
            this->_next.ebmlIDWidth + this->_next.sizeWidth + this->_next.dataSize);

        if (this->_offset < this->_dataSize) {
            this->_next = parseString(
                this->_data + this->_offset, this->_offset, this->_dataSize - this->_offset);
        }
        return i;

    }

    bool iterParseString::operator==(const iterParseString& other) const {
        if (this->_data != other._data) {
            return false;
        }
        if (this->_dataSize != other._dataSize) {
            return false;
        }
        if ((this->_offset >= this->_dataSize)
                and (other._offset >= other._dataSize)) {
            return true;
        }
        return (this->_offset == other._offset);
    }

    bool iterParseString::operator!=(const iterParseString& other) const {
        if (this->_data != other._data) {
            return true;
        }
        if (this->_dataSize != other._dataSize) {
            return true;
        }
        if ((this->_offset >= this->_dataSize)
                and (other._offset >= other._dataSize)) {
            return false;
        }
        return (this->_offset != other._offset);
    }

    parseString::parseString(const parseFile_sp& parsed, char* buffer) {
        parsed->read(buffer);
        this->ebmlID = parsed->ebmlID;
        this->ebmlIDWidth = parsed->ebmlIDWidth;
        this->dataSize = parsed->dataSize;
        this->sizeWidth = parsed->sizeWidth;
        this->offset = parsed->offset;
        this->data = buffer;
    }
}
#endif
