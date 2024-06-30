#ifndef EBML_NG_PARSING_IO_CPP
#define EBML_NG_PARSING_IO_CPP

#include <algorithm>

#include "libebml_ng/parsing/io.h"
#include "libebml_ng/vint.h"
#include "libebml_ng/exceptions.h"
#include "libebml_ng/struct/ull.h"
#include "libebml_ng/struct/ll.h"
#include "libebml_ng/struct/double.h"
#include "libebml_ng/struct/binary.h"
#include "libebml_ng/struct/unicode.h"

namespace ebml {
    size_t parseFile::outerSize() const {
        return this->ebmlIDWidth + this->sizeWidth + this->dataSize;
    }

    void _parseFile(
            ioBase& file,
            ebmlID_t& ebmlID, unsigned char& ebmlIDWidth, size_t& dataSize, unsigned char& sizeWidth) {
        ebmlID = unpackVint(file, ebmlIDWidth);

        if (ebmlIDWidth == 0) {
            sizeWidth = 0;
            dataSize = 0;
            return;
        }

        dataSize = unpackVint(file, sizeWidth);

        if (sizeWidth == 0) {
            throw ebmlUnexpectedEndOfData("Unexpected end of data.", nullptr, file.tell() - ebmlIDWidth, 0, ebmlIDWidth);
        }
    }

    // parseFile::parseFile(const ioBase_sp& file) : parseFile(*file) {}

    parseFile::parseFile(ioBase& file) : offset(file.tell()), _file(&file) {
        _parseFile(file, ebmlID, ebmlIDWidth, dataSize, sizeWidth);
    }

    void _parseFile(
            ioBase& file, off_t offset,
            ebmlID_t& ebmlID, unsigned char& ebmlIDWidth, size_t& dataSize, unsigned char& sizeWidth) {

        ebmlID = unpackVint(file, offset, ebmlIDWidth);

        if (ebmlIDWidth == 0) {
            sizeWidth = 0;
            dataSize = 0;
            return;
        }

        dataSize = unpackVint(file, offset + ebmlIDWidth, sizeWidth);

        if (sizeWidth == 0) {
            throw ebmlUnexpectedEndOfData("Unexpected end of data.", nullptr, offset, 0, ebmlIDWidth);
        }
    }

    // parseFile::parseFile(const ioBase_sp& file, off_t _offset) : parseFile(*file, _offset) {}

    parseFile::parseFile(ioBase& file, off_t offset) : offset(offset), _file(&file) {
        _parseFile(file, offset, ebmlID, ebmlIDWidth, dataSize, sizeWidth);
    }

    parseFile::parseFile(ioBase& file, parseFile& parent) : offset(0), parent(&parent), _file(&file) {
        _parseFile(file, offset, ebmlID, ebmlIDWidth, dataSize, sizeWidth);
    }

    // parseFile::parseFile(const ioBase_sp& file, parseFile& parent) : parseFile(*file, parent) {}

    parseFile::parseFile(ioBase& file, off_t offset, parseFile& parent) : offset(offset), parent(&parent), _file(&file) {
        _parseFile(file, offset, ebmlID, ebmlIDWidth, dataSize, sizeWidth);
    }

    // parseFile::parseFile(const ioBase_sp& file, off_t offset, parseFile& parent) : parseFile(*file, offset, parent) {}

    parseFile::parseFile(
            ioBase* file, ebmlID_t ebmlID, unsigned char ebmlIDWidth, size_t dataSize, unsigned char sizeWidth, off_t offset)
    : ebmlID(ebmlID), ebmlIDWidth(ebmlIDWidth), dataSize(dataSize), sizeWidth(sizeWidth), offset(offset), _file(file) {}

    parseFile::parseFile(
            ioBase* file, ebmlID_t ebmlID, unsigned char ebmlIDWidth, size_t dataSize,
            unsigned char sizeWidth, off_t offset, parseFile& parent)
    : ebmlID(ebmlID), ebmlIDWidth(ebmlIDWidth), dataSize(dataSize), sizeWidth(sizeWidth), offset(offset), parent(&parent), _file(file) {}

    off_t parseFile::seek(off_t offset) const {
        if (offset < 0) {
            offset = 0;
        } else if (offset > this->dataSize){
            offset = this->dataSize;
        }
        auto _dataOffset = this->dataOffset();
        return this->_file->seek(_dataOffset + offset) - _dataOffset;
    }

    size_t parseFile::read(char* dest) const {
        auto _endOffset = this->endOffset();
        auto _tell = this->_file->tell();

        if (_tell >= _endOffset) {
            return 0;
        }

        size_t remaining = static_cast<size_t>(_endOffset - _tell);
        return this->_file->read(dest, remaining);
    }

    size_t parseFile::read(char* dest, size_t size) const {
        auto _endOffset = this->endOffset();
        auto _tell = this->_file->tell();

        if (_tell >= _endOffset) {
            return 0;
        }

        size_t remaining = static_cast<size_t>(_endOffset - _tell);
        size = std::min(size, remaining);

        return this->_file->read(dest, size);
    }

    size_t parseFile::read(char* dest, off_t offset, size_t size) const {
        auto _endOffset = this->endOffset();

        if (offset < 0) {
            return 0;
        }

        size_t remaining = static_cast<size_t>(_endOffset - offset);
        size = std::min(size, remaining);

        return this->_file->read(dest, this->dataOffset() + offset, size);
    }

    parseFile::iterator parseFile::begin() const {
        auto _dataOffset = this->dataOffset();

        if (this->dataSize < 0xffffffffffffffff) {
            return parseFile::iterator(
                this->_file, _dataOffset, _dataOffset + this->dataSize);
        } else {
            return parseFile::iterator(this->_file, _dataOffset, 0xffffffffffffffff);
        }
    }

    off_t parseFile::dataOffset() const {
        return this->offset + this->ebmlIDWidth + this->sizeWidth;
    }

    off_t parseFile::endOffset() const {
        if (this->dataSize == 0xffffffffffffffff) {
            return 0xffffffffffffffff;
        }

        return this->offset + this->ebmlIDWidth + this->sizeWidth + this->dataSize;
    }

    // parseFile::iterator::iterator(ioBase_sp& file, off_t endoffset) : iterator(file.get(), endoffset) {}

    parseFile::iterator::iterator(ioBase* file, off_t endoffset) {
        this->_file = file;
        this->_startoffset = this->_offset = file->tell();
        this->_endoffset = endoffset;
    }

    // parseFile::iterator::iterator(ioBase_sp& file) : iterator(file, 0xffffffffffffffff) {}

    parseFile::iterator::iterator(ioBase* file) : iterator(file, 0xffffffffffffffff) {}

    // parseFile::iterator::iterator(ioBase_sp& file, off_t endoffset, parseFile& parent) : iterator(file, endoffset) {
    //     this->_parent = &parent;
    // }

    parseFile::iterator::iterator(ioBase* file, off_t endoffset, parseFile& parent) : iterator(file, endoffset) {
        this->_parent = &parent;
    }

    // parseFile::iterator::iterator(ioBase_sp& file, off_t startoffset, off_t endoffset) : iterator(file.get(), startoffset, endoffset) {}

    parseFile::iterator::iterator(ioBase* file, off_t startoffset, off_t endoffset) {
        this->_file = file;
        this->_startoffset = this->_offset = startoffset;
        this->_endoffset = endoffset;
    }

    // parseFile::iterator::iterator(ioBase_sp& file, off_t startoffset, off_t endoffset, parseFile& parent) : iterator(file, startoffset, endoffset) {
    //     this->_parent = &parent;
    // }

    parseFile parseFile::iterator::operator*() {
        if ((this->_endoffset >= 0) and (this->_offset >= this->_endoffset)) {
            throw stopIteration();
        }
        if (this->_ebmlIDWidth == 0) {
            _parseFile(*_file, this->_offset, this->_ebmlID, this->_ebmlIDWidth, this->_dataSize, this->_sizeWidth);

            auto result = parseFile(
                this->_file, this->_ebmlID, this->_ebmlIDWidth, this->_dataSize, this->_sizeWidth,
                this->_offset, *this->_parent);
            result.parentOffset = this->_startoffset;
            return result;
        } else {
            parseFile result = parseFile(*_file, _offset, *_parent);

            if ((this->_endoffset >= 0) and (result.endOffset() > this->_endoffset)) {
                // throw ebmlBoundError("Data extends past end of parent.", -1, this->_startoffset, this->_endoffset, ret.offset, ret.endOffset());
                throw ebmlDecodeError("Data extends past end of parent.", nullptr, result.offset, result.ebmlIDWidth + result.sizeWidth, result.endOffset());
            }

            this->_ebmlID = result.ebmlID;
            this->_ebmlIDWidth = result.ebmlIDWidth;
            this->_dataSize = result.dataSize;
            this->_sizeWidth = result.sizeWidth;
            result.parentOffset = this->_startoffset;
            return result;
        }
    }

    parseFile::iterator& parseFile::iterator::operator++() {
        if ((_endoffset >= 0) and (_offset >= _endoffset)) {
            throw stopIteration();
        }

        if (_ebmlIDWidth == 0) {
            _parseFile(*_file, _offset, _ebmlID, _ebmlIDWidth, _dataSize, _sizeWidth);
        }

        _offset += _ebmlIDWidth + _sizeWidth + _dataSize;
        _ebmlIDWidth = 0;

        if ((_endoffset >= 0) and (_offset > _endoffset)) {
            throw ebmlException("ebmlBoundError");
        }

        return *this;
    }

    bool parseFile::iterator::atEnd() {
        if (_endoffset >= 0) {
            return (_offset >= _endoffset);
        }

        if (_ebmlIDWidth > 0) {
            return false;
        }

        _parseFile(*_file, _offset, _ebmlID, _ebmlIDWidth, _dataSize, _sizeWidth);
        return _ebmlIDWidth == 0;
    }

    template unsigned long long parseFile::unpack<unsigned long long>() const;
    template long long parseFile::unpack<long long>() const;
    template double parseFile::unpack<double>() const;
    template std::string parseFile::unpack<std::string>() const;
    template std::wstring parseFile::unpack<std::wstring>() const;
}
#endif
