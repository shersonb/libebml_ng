#ifndef EBML_NG_EXCEPTIONS_H
#define EBML_NG_EXCEPTIONS_H
#include <string>
#include "base.h"

namespace ebml {
    std::string make_exc_msg(const char* msg, unsigned int lineno, const char* file);

    class stopIteration : public std::exception {};

    class ebmlException : public std::exception {
    private:
        std::string _message;

    public:
        ebmlException(const std::string& message);
        const char* what() const noexcept override;
    };

    class ebmlNotImplementedError : public ebmlException {
    public:
        ebmlNotImplementedError(const std::string& message, const ebmlElementClass* cls=nullptr);
        const ebmlElementClass* cls;
    };

    class ebmlEncodeError : public ebmlException {
    private:
        c_ebmlElement_sp _elem;

    public:
        // ebmlEncodeError(const std::string& message);
        ebmlEncodeError(const std::string& message, const c_ebmlElement_sp& elem=nullptr);
        c_ebmlElement_sp& elem() const;
    };

    class ebmlDecodeError : public ebmlException {
    public:
        ebmlDecodeError(const std::string& message, off_t offset=-1, const ebmlElementClass* cls=nullptr);
        off_t offset;
        const ebmlElementClass* cls;
        virtual void add_to_offset(off_t);
    };

    class ebmlInvalidVint : public ebmlDecodeError {
    public:
        ebmlInvalidVint(const std::string& message, off_t offset=-1, const ebmlElementClass* cls=nullptr);
    };

    class ebmlNoMatch : public ebmlDecodeError {
    public:
        ebmlNoMatch(const std::string& message, off_t offset=-1, const ebmlElementClass* cls=nullptr);
    };

    class ebmlNoChildMatch : public ebmlDecodeError {
    public:
        ebmlNoChildMatch(const std::string& message, off_t offset=-1, const ebmlElementClass* cls=nullptr);
    };

    class ebmlBoundError : public ebmlDecodeError {
    private:
    public:
        ebmlBoundError(
            const std::string& message, off_t offset=-1,
            off_t parentstart=-1, off_t parentend=-1, off_t childstart=-1, off_t childend=-1,
            const ebmlElementClass* cls=nullptr);
        void add_to_offset(off_t);
        off_t parentstart;
        off_t parentend;
        off_t childstart;
        off_t childend;
    };

    class ebmlUnexpectedEndOfData : public ebmlDecodeError {
    public:
        ebmlUnexpectedEndOfData(const std::string& message, off_t offset=-1, const ebmlElementClass* cls=nullptr);
    };

    class ebmlDataContinues : public ebmlDecodeError {
    public:
        ebmlDataContinues(const std::string& message, off_t offset=-1, const ebmlElementClass* cls=nullptr);
    };

    class ebmlFormatError : public ebmlDecodeError {
    public:
        ebmlFormatError(const std::string& message, off_t offset=-1, const ebmlElementClass* cls=nullptr);
    };

    class unicodeDecodeError : public ebmlFormatError {
    public:
        unicodeDecodeError(const std::string& message, off_t offset=-1, const std::string& object="", off_t start=-1, off_t end=-1,
                           const ebmlElementClass* cls=nullptr);
        std::string object;
        off_t start;
        off_t end;
    };

    class unicodeEncodeError : public ebmlEncodeError {
    public:
        unicodeEncodeError(const std::string&  message, const std::wstring& object=L"", off_t start=-1, off_t end=-1, const c_ebmlElement_sp& elem=nullptr);
        std::wstring object;
        off_t start;
        off_t end;
    };
}
#endif
