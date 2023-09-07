#ifndef EBML_NG_PARSING_IO_CPP
#define EBML_NG_PARSING_IO_CPP

#include "libebml_ng/parsing/string.h"
#include "libebml_ng/parsing/io.h"
#include "libebml_ng/vint.cpp"
#include "libebml_ng/exceptions.cpp"
#include <algorithm>

namespace ebml {
    size_t parseFile::outerSize() const {
        return this->ebmlIDWidth + this->sizeWidth + this->dataSize;
    }

    void _parseFile(
            ioBase* file,
            ebmlID_t& ebmlID, unsigned char& ebmlIDWidth, size_t& dataSize, unsigned char& sizeWidth) {
        ebmlID = unpackVint(file, ebmlIDWidth);

        if (ebmlIDWidth == 0) {
            sizeWidth = 0;
            dataSize = 0;
            return;
        }

        dataSize = unpackVint(file, sizeWidth);

        if (sizeWidth == 0) {
            throw ebmlUnexpectedEndOfData("Unexpected end of data.", nullptr, file->tell() - ebmlIDWidth, 0, ebmlIDWidth);
        }
    }

    parseFile::parseFile(ioBase_sp& file) : parseFile(file.get()) {}

    parseFile::parseFile(ioBase* file) {
        this->_file = file;
        this->offset = file->tell();

        _parseFile(file, this->ebmlID, this->ebmlIDWidth, this->dataSize, this->sizeWidth);
    }

    void _parseFile(
            ioBase* file, off_t offset,
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

    parseFile::parseFile(ioBase_sp& file, off_t _offset) : parseFile(file.get(), _offset) {}

    parseFile::parseFile(ioBase* file, off_t _offset) {
        this->_file = file;
        this->offset = _offset;
        _parseFile(file, _offset, this->ebmlID, this->ebmlIDWidth, this->dataSize, this->sizeWidth);
    }

    parseFile::parseFile(ioBase* file, parseFile& _parent) : parseFile(file) {
        this->parent = &_parent;
    }

    parseFile::parseFile(ioBase_sp& file, parseFile& _parent) : parseFile(file) {
        this->parent = &_parent;
    }

    parseFile::parseFile(ioBase* file, off_t _offset, parseFile& _parent) : parseFile(file, _offset) {
        this->parent = &_parent;
    }

    parseFile::parseFile(ioBase_sp& file, off_t _offset, parseFile& _parent) : parseFile(file, _offset) {
        this->parent = &_parent;
    }

    parseFile::parseFile(
            ioBase* file, ebmlID_t _ebmlID, unsigned char _ebmlIDWidth, size_t _dataSize,
            unsigned char _sizeWidth, off_t _offset) {
        this->_file = file;
        this->ebmlID = _ebmlID;
        this->ebmlIDWidth = _ebmlIDWidth;
        this->dataSize = _dataSize;
        this->sizeWidth = _sizeWidth;
        this->offset = _offset;
    }

    parseFile::parseFile(
            ioBase* file, ebmlID_t _ebmlID, unsigned char _ebmlIDWidth, size_t _dataSize,
            unsigned char _sizeWidth, off_t _offset, parseFile& _parent) :
        parseFile(file, _ebmlID, _ebmlIDWidth, _dataSize, _sizeWidth, _offset) {
            this->parent = &(_parent);
        }

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

    parseFile::iterator parseFile::begin() const {
        auto _dataOffset = this->dataOffset();
        // std::cout << "_dataOffset: " << _dataOffset << std::endl;

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

    parseFile::iterator::iterator(ioBase_sp& file, off_t endoffset) : iterator(file.get(), endoffset) {}

    parseFile::iterator::iterator(ioBase* file, off_t endoffset) {
        this->_file = file;
        this->_startoffset = this->_offset = file->tell();
        this->_endoffset = endoffset;
    }

    parseFile::iterator::iterator(ioBase_sp& file) : iterator(file, 0xffffffffffffffff) {}

    parseFile::iterator::iterator(ioBase* file) : iterator(file, 0xffffffffffffffff) {}

    parseFile::iterator::iterator(ioBase_sp& file, off_t endoffset, parseFile& parent) : iterator(file, endoffset) {
        this->_parent = &parent;
    }

    parseFile::iterator::iterator(ioBase* file, off_t endoffset, parseFile& parent) : iterator(file, endoffset) {
        this->_parent = &parent;
    }

    parseFile::iterator::iterator(ioBase_sp& file, off_t startoffset, off_t endoffset) : iterator(file.get(), startoffset, endoffset) {}

    parseFile::iterator::iterator(ioBase* file, off_t startoffset, off_t endoffset) {
        this->_file = file;
        this->_startoffset = this->_offset = startoffset;
        this->_endoffset = endoffset;
    }

    parseFile::iterator::iterator(ioBase_sp& file, off_t startoffset, off_t endoffset, parseFile& parent) : iterator(file, startoffset, endoffset) {
        this->_parent = &parent;
    }

    parseFile parseFile::iterator::operator*() {
        if ((this->_endoffset >= 0) and (this->_offset >= this->_endoffset)) {
            throw stopIteration();
        }
        if (this->_ebmlIDWidth == 0) {
            _parseFile(this->_file, this->_offset, this->_ebmlID, this->_ebmlIDWidth, this->_dataSize, this->_sizeWidth);

            return parseFile(
                this->_file, this->_ebmlID, this->_ebmlIDWidth, this->_dataSize, this->_sizeWidth,
                this->_offset, *this->_parent);
        } else {
            parseFile ret = parseFile(this->_file, this->_offset, *this->_parent);

            if ((this->_endoffset >= 0) and (ret.endOffset() > this->_endoffset)) {
                // throw ebmlBoundError("Data extends past end of parent.", -1, this->_startoffset, this->_endoffset, ret.offset, ret.endOffset());
                throw ebmlDecodeError("Data extends past end of parent.", nullptr, ret.offset, ret.ebmlIDWidth + ret.sizeWidth, ret.endOffset());
            }

            this->_ebmlID = ret.ebmlID;
            this->_ebmlIDWidth = ret.ebmlIDWidth;
            this->_dataSize = ret.dataSize;
            this->_sizeWidth = ret.sizeWidth;
            return ret;
        }
    }

    parseFile::iterator& parseFile::iterator::operator++() {
        if ((this->_endoffset >= 0) and (this->_offset >= this->_endoffset)) {
            throw stopIteration();
        }

        if (this->_ebmlIDWidth == 0) {
            _parseFile(this->_file, this->_offset, this->_ebmlID, this->_ebmlIDWidth, this->_dataSize, this->_sizeWidth);
        }

        this->_offset += this->_ebmlIDWidth + this->_sizeWidth + this->_dataSize;
        this->_ebmlIDWidth = 0;

        if ((this->_endoffset >= 0) and (this->_offset > this->_endoffset)) {
            throw ebmlException("ebmlBoundError");
        }

        return *this;
    }

    bool parseFile::iterator::atEnd() {
        if (this->_endoffset >= 0) {
            return (this->_offset >= this->_endoffset);
        }

        if (this->_ebmlIDWidth > 0) {
            return false;
        }

        _parseFile(this->_file, this->_offset, this->_ebmlID, this->_ebmlIDWidth, this->_dataSize, this->_sizeWidth);
        return this->_ebmlIDWidth == 0;
    }
}
#endif
