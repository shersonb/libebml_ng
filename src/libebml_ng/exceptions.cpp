#ifndef EBML_NG_EXCEPTIONS_CPP
#define EBML_NG_EXCEPTIONS_CPP

#include "libebml_ng/exceptions.h"

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

    ebmlNotImplementedError::ebmlNotImplementedError(const std::string& message, const ebmlElementClass* cls) : ebmlException(message) {
        this->cls = cls;
    }

    ebmlEncodeError::ebmlEncodeError(const std::string& message, const c_ebmlElement_sp& elem) : ebmlException(message), _elem(elem) {}

    ebmlDecodeError::ebmlDecodeError(DECODE_ERR_SIG) : ebmlException(message) {
        this->offset = offset;
        this->cls = cls;
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
    //         const ebmlElementClass* cls) : ebmlDecodeError(message, offset, cls) {
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

}
#endif
