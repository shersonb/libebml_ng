#ifndef EBML_NG_PARSING_STRING_CPP
#define EBML_NG_PARSING_STRING_CPP

#include "libebml_ng/parsing/string.h"
#include "libebml_ng/vint.h"
#include "libebml_ng/exceptions.h"
#include "libebml_ng/struct/ull.h"
#include "libebml_ng/struct/ll.h"
#include "libebml_ng/struct/double.h"
#include "libebml_ng/struct/binary.h"
#include "libebml_ng/struct/unicode.h"
#include <utility>

namespace ebml {
    parseString::parseString()
    : ebmlID(0), ebmlIDWidth(0), dataSize(0), sizeWidth(0), offset(0), data(nullptr) {}

    parseString::parseString(const parseString& other)
    : ebmlID(other.ebmlID), ebmlIDWidth(other.ebmlIDWidth), dataSize(other.dataSize), sizeWidth(other.sizeWidth), offset(other.offset), data(other.data) {}

    parseString::parseString(parseString&& other)
    : ebmlID(std::exchange(other.ebmlID, 0)), ebmlIDWidth(std::exchange(other.ebmlIDWidth, 0)), dataSize(std::exchange(other.dataSize, 0)),
    sizeWidth(std::exchange(other.sizeWidth, 0)), offset(std::exchange(other.offset, 0)), data(std::exchange(other.data, nullptr)) {}

    parseString& parseString::operator=(const parseString& other) {
        this->ebmlID = other.ebmlID;
        this->ebmlIDWidth = other.ebmlIDWidth;
        this->dataSize = other.dataSize;
        this->sizeWidth = other.sizeWidth;
        this->offset = other.offset;
        this->data = other.data;
        return *this;
    }

    parseString& parseString::operator=(parseString&& other) {
        this->ebmlID = std::exchange(other.ebmlID, 0);
        this->ebmlIDWidth = std::exchange(other.ebmlIDWidth, 0);
        this->dataSize = std::exchange(other.dataSize, 0);
        this->sizeWidth = std::exchange(other.sizeWidth, 0);
        this->offset = std::exchange(other.offset, 0);
        this->data = std::exchange(other.data, nullptr);
        return *this;
    }

    parseString::parseString(
            ebmlID_t ebmlID, unsigned char ebmlIDWidth,
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

    void _parseString(
            const char* data, size_t size,
            ebmlID_t& ebmlID, unsigned char& ebmlIDWidth, size_t& dataSize, unsigned char& sizeWidth) {
        ebmlID = unpackVint(data, size, ebmlIDWidth);
        dataSize = unpackVint(data + ebmlIDWidth, size - ebmlIDWidth, sizeWidth);

        if (size < ebmlIDWidth + sizeWidth + dataSize) {
            throw ebmlUnexpectedEndOfData("_parseString(): Unexpected end of data.", 0);
        }
    }

    parseString::parseString(const char* data, size_t size) {
        _parseString(data, size, this->ebmlID, this->ebmlIDWidth, this->dataSize, this->sizeWidth);
        this->offset = 0;
        this->data = data + this->ebmlIDWidth + this->sizeWidth;
    }

    parseString::parseString(const char* data, size_t size, off_t offset) {
        try {
            _parseString(data, size, this->ebmlID, this->ebmlIDWidth, this->dataSize, this->sizeWidth);
        } catch (ebmlDecodeError& e) {
            e.add_to_offset(offset);
            throw e;
        }

        this->offset = offset;
        this->data = data + this->ebmlIDWidth + this->sizeWidth;
    }

    parseString::iterator parseString::begin() const {
        return parseString::iterator(this->data, this->dataSize);
    }

//     iterParseString parseString::end() const {
//         return iterParseString(this->data, this->dataSize, this->dataSize);
//     }

    unsigned long long parseString::outerSize() const {
        return this->ebmlIDWidth + this->sizeWidth + this->dataSize;
    }

//     iterParseString::iterParseString() {}

    parseString::iterator::iterator(const char* data, size_t size) {
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
    parseString parseString::iterator::operator*() {
        return this->_next;
    }

    parseString::iterator& parseString::iterator::operator++() {
        if (this->_offset >= this->_dataSize) {
            throw stopIteration();
        }

        this->_offset += (
            this->_next.ebmlIDWidth + this->_next.sizeWidth + this->_next.dataSize);

        if (this->_offset < this->_dataSize) {
            this->_next = parseString(
                this->_data + this->_offset, this->_dataSize - this->_offset, this->_offset);
        }
        return *this;
    }

    parseString::iterator parseString::iterator::operator++(int) {
        parseString::iterator i = *this;

            if (this->_offset >= this->_dataSize) {
            throw stopIteration();
        }

        this->_offset += (
            this->_next.ebmlIDWidth + this->_next.sizeWidth + this->_next.dataSize);

        if (this->_offset < this->_dataSize) {
            try {
                this->_next = parseString(
                    this->_data + this->_offset, this->_dataSize - this->_offset, this->_offset);
            } catch (ebmlDecodeError& e) {
                e.add_to_offset(this->_offset);
                throw e;
            }
        }
        return i;

    }

    bool parseString::iterator::operator==(const parseString::iterator& other) const {
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

    bool parseString::iterator::operator!=(const parseString::iterator& other) const {
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

    bool parseString::iterator::atEnd() const {
        return this->_offset >= this->_dataSize;
    }

    parseString::parseString(const parseFile& parsed, char* buffer) {
        parsed.read(buffer);
        this->ebmlID = parsed.ebmlID;
        this->ebmlIDWidth = parsed.ebmlIDWidth;
        this->dataSize = parsed.dataSize;
        this->sizeWidth = parsed.sizeWidth;
        this->offset = parsed.offset;
        this->data = buffer;
    }

    template unsigned long long parseString::unpack<unsigned long long>() const;
    template long long parseString::unpack<long long>() const;
    template double parseString::unpack<double>() const;
    template std::string parseString::unpack<std::string>() const;
    template std::wstring parseString::unpack<std::wstring>() const;
}
#endif
