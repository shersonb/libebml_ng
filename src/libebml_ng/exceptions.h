#ifndef EBML_NG_EXCEPTIONS_H
#define EBML_NG_EXCEPTIONS_H

#include <string>

#include "ptrs.h"

#define DECODE_ERR_SIG_DECL const std::string& message, const ebmlElementType* cls=nullptr, off_t offset=-1, unsigned char headSize=0, off_t erroroffset=-1
#define DECODE_ERR_SIG const std::string& message, const ebmlElementType* cls, off_t offset, unsigned char headSize, off_t erroroffset
#define DECODE_ERR_ARGS message, cls, offset, headSize, erroroffset

// Supplies default arguments for cls, offset, and headSize in case thrown from inside an unpack function that may not have this information.
#define DECODE_ERR_DEFAULT nullptr, 0, 0


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
        ebmlNotImplementedError(const std::string& message, const ebmlElementType* cls=nullptr);
        const ebmlElementType* cls;
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
        ebmlDecodeError(DECODE_ERR_SIG_DECL);
        off_t offset; // offset of element start (head)
        unsigned char headSize;
        off_t erroroffset; // offset of error.
        const ebmlElementType* cls;
        virtual void add_to_offset(off_t);
    };

    class ebmlInvalidVint : public ebmlDecodeError {
    public:
        ebmlInvalidVint(DECODE_ERR_SIG_DECL);
    };

    class ebmlNoMatch : public ebmlDecodeError {
    public:
        ebmlNoMatch(DECODE_ERR_SIG_DECL);
    };

    class ebmlNoChildMatch : public ebmlDecodeError {
    public:
        ebmlNoChildMatch(DECODE_ERR_SIG_DECL);
    };

    // class ebmlBoundError : public ebmlDecodeError {
    // private:
    // public:
    //     ebmlBoundError(
    //         const std::string& message, off_t offset=-1,
    //         off_t parentstart=-1, off_t parentend=-1, off_t childstart=-1, off_t childend=-1,
    //         const ebmlElementType* cls=nullptr);
    //     void add_to_offset(off_t);
    //     off_t parentstart;
    //     off_t parentend;
    //     off_t childstart;
    //     off_t childend;
    // };

    class ebmlUnexpectedEndOfData : public ebmlDecodeError {
    public:
        ebmlUnexpectedEndOfData(DECODE_ERR_SIG_DECL);
    };

    class ebmlDataContinues : public ebmlDecodeError {
    public:
        ebmlDataContinues(DECODE_ERR_SIG_DECL);
    };

    class ebmlFormatError : public ebmlDecodeError {
    public:
        ebmlFormatError(DECODE_ERR_SIG_DECL);
    };

    class unicodeDecodeError : public ebmlFormatError {
    public:
        unicodeDecodeError(DECODE_ERR_SIG_DECL, off_t end=-1, const std::string& object="");
        std::string object;
        off_t start;
        off_t end;
    };

    class unicodeEncodeError : public ebmlEncodeError {
    public:
        unicodeEncodeError(const std::string& message, const std::wstring& object=L"", off_t start=-1, off_t end=-1, const c_ebmlElement_sp& elem=nullptr);
        std::wstring object;
        off_t start;
        off_t end;
    };

    class ebmlWriteError : public ebmlException {
    public:
        ebmlWriteError(const std::string&);
    };

    class ebmlInsertionError : public ebmlWriteError {
    public:
        ebmlInsertionError(const std::string&, off_t, off_t, off_t, off_t, off_t);
        ebmlInsertionError(const std::string&, off_t, off_t, off_t, off_t, off_t, const c_ebmlElement_sp&);
        ebmlInsertionError(const std::string&, off_t, off_t, off_t, off_t, off_t, c_ebmlElement_sp&&);
        off_t prevOffset;
        off_t prevEnd;
        off_t offset;
        off_t endOffset;
        off_t nextOffset;
        c_ebmlElement_sp elem;
    };

    class ebmlMoveError : public ebmlWriteError {
    public:
        ebmlMoveError(const std::string&, off_t, off_t, off_t, off_t, off_t, off_t, off_t, off_t);
        ebmlMoveError(const std::string&, off_t, off_t, off_t, off_t, off_t, off_t, off_t, off_t, const c_ebmlElement_sp&);
        ebmlMoveError(const std::string&, off_t, off_t, off_t, off_t, off_t, off_t, off_t, off_t, c_ebmlElement_sp&&);
        off_t prevEndOld;
        off_t src_offset;
        off_t nextOffsetOld;
        off_t prevOffset;
        off_t prevEnd;
        off_t dest_offset;
        off_t endOffset;
        off_t nextOffset;
        c_ebmlElement_sp elem;
    };

    class ebmlResizeError : public ebmlWriteError {
    public:
        ebmlResizeError(const std::string&, off_t, off_t, off_t, off_t, off_t);
        ebmlResizeError(const std::string&, off_t, off_t, off_t, off_t, off_t, const c_ebmlElement_sp&);
        ebmlResizeError(const std::string&, off_t, off_t, off_t, off_t, off_t, c_ebmlElement_sp&&);
        off_t offset;
        off_t endOffset;
        off_t lastChildOffset;
        off_t lastChildEnd;
        off_t nextSiblingOffset;
        c_ebmlElement_sp elem;
    };
}
#endif
