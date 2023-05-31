#ifndef EBML_NG_EXCEPTIONS_H
#define EBML_NG_EXCEPTIONS_H
#include <string>

namespace ebml {
    std::string make_exc_msg(const char* msg, unsigned int lineno, const char* file);

    class ebmlException : public std::exception {
        std::string _what;
    public:
        const char* message;
        unsigned long long lineno;
        const char* file;
        ebmlException(const char*, unsigned long long, const char*);
        const char* what();
    };

    class ebmlClassNotDefined : public ebmlException {
    public:
        ebmlClassNotDefined(const char* msg, unsigned int lineno, const char* file): ebmlException(msg, lineno, file) {};
    };

    class ebmlFunctionNotDefined : public ebmlException {
    public:
        ebmlFunctionNotDefined(const char* msg, unsigned int lineno, const char* file): ebmlException(msg, lineno, file) {};
    };
}
#endif
