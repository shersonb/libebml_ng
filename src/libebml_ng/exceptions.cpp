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

    ebmlException::ebmlException(
            const char* message, unsigned long long lineno, const char* file) {
        this->message = message;
        this->lineno = lineno;
        this->file = file;
        this->_what = make_exc_msg(message, lineno, file);
    }

    const char* ebmlException::what() {
        return this->_what.c_str();
    }
}
#endif
