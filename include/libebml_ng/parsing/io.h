#ifndef EBML_NG_PARSEFILE_H
#define EBML_NG_PARSEFILE_H

#include "libebml_ng/ebmlID_t.h"
#include "libebml_ng/io.h"
// #include "libebml_ng/parsing/string.h"
#include <memory>

namespace ebml {
    class iterParseFile;
    class parseFile;

    typedef std::shared_ptr<iterParseFile> iterParseFile_sp;
    typedef std::shared_ptr<parseFile> parseFile_sp;

    class parseFile {
    public:
        ebmlID_t ebmlID;
        unsigned char ebmlIDWidth;
        size_t dataSize;
        unsigned char sizeWidth;
        off_t offset;
        parseFile_sp parent;
        off_t dataOffset() const;
        size_t read(char*);
        size_t read(char*, size_t);
        iterParseFile_sp begin() const;
//         parseStringData to_parseStringData(char*); // TODO: Reimplement as a constructor to parseStringData
        size_t outerSize() const;
        bool seekable() const;
        parseFile(ioBase*);
        parseFile(ioBase*, char*);
        parseFile(ioBase*, parseFile_sp&);
        parseFile(ioBase*, char*, parseFile_sp&);
        parseFile(ioBase*, off_t);
        parseFile(ioBase*, off_t, char*);
        parseFile(ioBase*, off_t, parseFile_sp&); // Why?
        parseFile(ioBase*, off_t, char*, parseFile_sp&); // Why?
//         parseFile(const parseFile&);
//         parseFile& operator=(const parseFile&);
//         virtual ~parseFile();
    protected:
        void _update_bytes_read(size_t);
    private:
        void _init(ioBase*, off_t, char*);
        ioBase* _file;
        bool _seekable;
        size_t bytes_read;
        size_t _read_seekable(char*, size_t);
        size_t _read_unseekable(char*, size_t);
        void _cleanup();
        void _consume_remaining();
    };

//     std::unique_ptr<parseFileBase> parseFile(ioBase*);

//     struct iterParseFileState;

    class iterParseFile {
    public:
        iterParseFile(ioBase* file, off_t start);
        iterParseFile(ioBase* file, off_t start, off_t end);
        iterParseFile(ioBase* file, off_t start, parseFile*);
        iterParseFile(ioBase* file, off_t start, off_t end, parseFile*);
        parseFile_sp operator*();
        iterParseFile& operator++();
        iterParseFile operator++(int);
        bool atEnd() const;
        bool seekable() const;
    private:
        off_t _startoffset;
        off_t _offset;
        off_t _nextoffset;
        off_t _endoffset;
        ioBase* _file;
        parseFile_sp _parent;
        bool _seekable;
    };

//     class iterParseFile {
//         FILE* _file;
//         unsigned long long _startoffset;
//         unsigned long long _offset;
//         unsigned long long _endoffset;
//         parseFileData _next;
//         char buffer[16];
//     public:
//         iterParseFile(void);
//         iterParseFile(FILE*);
//         iterParseFile(FILE*, unsigned long long);
//     //     iterParseFile(FILE*, unsigned long long, unsigned long long);
//         iterParseFile(
//             FILE*, unsigned long long, unsigned long long,
//             unsigned long long);
//         parseFileData operator*(void);
//         iterParseFile& operator++(void);
//         iterParseFile operator++(int);
//         bool operator==(const iterParseFile&) const;
//         bool operator!=(const iterParseFile&) const;
//     };
}
#endif
