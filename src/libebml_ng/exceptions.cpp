#ifndef EBML_NG_EXCEPTIONS_CPP
#define EBML_NG_EXCEPTIONS_CPP

#include "exceptions.h"
#include "ebmlElement.h"

namespace ebml {
    std::string make_exc_msg(const char* msg, unsigned int lineno, const char* file) {
        return (
            std::string(msg) + std::string(" (") + std::to_string(lineno)
            + std::string(":") + std::string(file) + std::string(")")
        );
    }

    ebmlException::ebmlException(const std::string& message) {
        this->_message = message;
    }

    const char* ebmlException::what() const noexcept {
        return this->_message.c_str();
    }

    ebmlNotImplementedError::ebmlNotImplementedError(const std::string& message, const ebmlElementType* cls) : ebmlException(message) {
        this->cls = cls;
    }

    ebmlEncodeError::ebmlEncodeError(const std::string& message, const c_ebmlElement_sp& elem) : ebmlException(message), _elem(elem) {}

    ebmlDecodeError::ebmlDecodeError(DECODE_ERR_SIG) : ebmlException(message), offset(offset), headSize(headSize), erroroffset(erroroffset), cls(cls) {
        // this->offset = offset;
        // this->cls = cls;
    }

    void ebmlDecodeError::add_to_offset(off_t delta) {
        this->offset += delta;
    }

    ebmlInvalidVint::ebmlInvalidVint(DECODE_ERR_SIG) : ebmlDecodeError(DECODE_ERR_ARGS) {}
    ebmlNoMatch::ebmlNoMatch(DECODE_ERR_SIG) : ebmlDecodeError(DECODE_ERR_ARGS) {}
    ebmlNoChildMatch::ebmlNoChildMatch(DECODE_ERR_SIG) : ebmlDecodeError(DECODE_ERR_ARGS) {}
    ebmlUnexpectedEndOfData::ebmlUnexpectedEndOfData(DECODE_ERR_SIG) : ebmlDecodeError(DECODE_ERR_ARGS) {}
    ebmlDataContinues::ebmlDataContinues(DECODE_ERR_SIG) : ebmlDecodeError(DECODE_ERR_ARGS) {}
    ebmlFormatError::ebmlFormatError(DECODE_ERR_SIG) : ebmlDecodeError(DECODE_ERR_ARGS) {}

    // ebmlBoundError::ebmlBoundError(
    //         const std::string& message, off_t offset,
    //         off_t parentstart, off_t parentend, off_t childstart, off_t childend,
    //         const ebmlElementType* cls) : ebmlDecodeError(message, offset, cls) {
    //     this->parentstart = parentstart;
    //     this->parentend = parentend;
    //     this->childstart = childstart;
    //     this->childend = childend;
    // }

    // void ebmlBoundError::add_to_offset(off_t delta) {
    //     ebmlDecodeError::add_to_offset(delta);
    //     this->parentend += delta;
    //     this->childstart += delta;
    //     this->childend += delta;
    // }

    unicodeDecodeError::unicodeDecodeError(DECODE_ERR_SIG, off_t end, const std::string& object) : ebmlFormatError(DECODE_ERR_ARGS) {
        this->end = end;
        this->object = object;
    }

    unicodeEncodeError::unicodeEncodeError(const std::string& message, const std::wstring& object, off_t start, off_t end, const c_ebmlElement_sp& elem) : ebmlEncodeError(message, elem) {
        this->object = object;
        this->start = start;
        this->end = end;
    }

    ebmlWriteError::ebmlWriteError(const std::string& message)
    : ebmlException(message) {}

    ebmlInsertionError::ebmlInsertionError(const std::string& message, off_t prevOffset, off_t prevEnd, off_t offset, off_t endOffset, off_t nextOffset)
    : ebmlWriteError(message), prevOffset(prevOffset), prevEnd(prevEnd), offset(offset), endOffset(endOffset), nextOffset(nextOffset) {}

    ebmlInsertionError::ebmlInsertionError(const std::string& message, off_t prevOffset, off_t prevEnd, off_t offset, off_t endOffset, off_t nextOffset, const c_ebmlElement_sp& elem)
    : ebmlWriteError(message), prevOffset(prevOffset), prevEnd(prevEnd), offset(offset), endOffset(endOffset), nextOffset(nextOffset), elem(elem) {}

    ebmlInsertionError::ebmlInsertionError(const std::string& message, off_t prevOffset, off_t prevEnd, off_t offset, off_t endOffset, off_t nextOffset, c_ebmlElement_sp&& elem)
    : ebmlWriteError(message), prevOffset(prevOffset), prevEnd(prevEnd), offset(offset), endOffset(endOffset), nextOffset(nextOffset), elem(std::move(elem)) {}

    ebmlMoveError::ebmlMoveError(
        const std::string& message,
        off_t prevEndOld, off_t src_offset, off_t nextOffsetOld,
        off_t prevOffset, off_t prevEnd, off_t dest_offset, off_t endOffset, off_t nextOffset)
    : ebmlWriteError(message),
    prevEndOld(prevEndOld), src_offset(src_offset), nextOffsetOld(nextOffsetOld),
    prevOffset(prevOffset), prevEnd(prevEnd), dest_offset(dest_offset), endOffset(endOffset), nextOffset(nextOffset) {}

    ebmlMoveError::ebmlMoveError(
        const std::string& message,
        off_t prevEndOld, off_t src_offset, off_t nextOffsetOld,
        off_t prevOffset, off_t prevEnd, off_t dest_offset, off_t endOffset, off_t nextOffset, const c_ebmlElement_sp& elem)
    : ebmlWriteError(message),
    prevEndOld(prevEndOld), src_offset(src_offset), nextOffsetOld(nextOffsetOld),
    prevOffset(prevOffset), prevEnd(prevEnd), dest_offset(dest_offset), endOffset(endOffset), nextOffset(nextOffset), elem(elem) {}

    ebmlMoveError::ebmlMoveError(
        const std::string& message,
        off_t prevEndOld, off_t src_offset, off_t nextOffsetOld,
        off_t prevOffset, off_t prevEnd, off_t dest_offset, off_t endOffset, off_t nextOffset, c_ebmlElement_sp&& elem)
    : ebmlWriteError(message),
    prevEndOld(prevEndOld), src_offset(src_offset), nextOffsetOld(nextOffsetOld),
    prevOffset(prevOffset), prevEnd(prevEnd), dest_offset(dest_offset), endOffset(endOffset), nextOffset(nextOffset), elem(std::move(elem)) {}

    ebmlResizeError::ebmlResizeError(
        const std::string& message,
        off_t offset, off_t endOffset,
        off_t lastChildOffset, off_t lastChildEnd,
        off_t nextSiblingOffset)
    : ebmlWriteError(message),
    offset(offset), endOffset(endOffset),
    lastChildOffset(lastChildOffset), lastChildEnd(lastChildEnd),
    nextSiblingOffset(nextSiblingOffset) {}

    ebmlResizeError::ebmlResizeError(
        const std::string& message,
        off_t offset, off_t endOffset,
        off_t lastChildOffset, off_t lastChildEnd,
        off_t nextSiblingOffset, const c_ebmlElement_sp& elem)
    : ebmlWriteError(message),
    offset(offset), endOffset(endOffset),
    lastChildOffset(lastChildOffset), lastChildEnd(lastChildEnd),
    nextSiblingOffset(nextSiblingOffset),
    elem(elem) {}

    ebmlResizeError::ebmlResizeError(
        const std::string& message,
        off_t offset, off_t endOffset,
        off_t lastChildOffset, off_t lastChildEnd,
        off_t nextSiblingOffset, c_ebmlElement_sp&& elem)
    : ebmlWriteError(message),
    offset(offset), endOffset(endOffset),
    lastChildOffset(lastChildOffset), lastChildEnd(lastChildEnd),
    nextSiblingOffset(nextSiblingOffset),
    elem(std::move(elem)) {}
}
#endif
