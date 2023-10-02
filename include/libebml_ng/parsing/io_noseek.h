#ifndef EBML_NG_PARSEFILE_NOSEEK_H
#define EBML_NG_PARSEFILE_NOSEEK_H

#include "libebml_ng/ebmlID_t.h"
#include "libebml_ng/io.h"
// #include "libebml_ng/parsing/string.h"
#include <memory>

// TODO: Implement parsing from NON-seekable streams and files.

namespace ebml {
//     class parseFile;
//
//     typedef std::shared_ptr<parseFile> parseFile_sp;
//
//     class parseFile {
//     public:
//         class iterator;
//         ebmlID_t ebmlID;
//         unsigned char ebmlIDWidth;
//         size_t dataSize;
//         unsigned char sizeWidth;
//         off_t offset;
//         parseFile_sp parent;
//         off_t dataOffset() const;
//         off_t endOffset() const;
//         size_t read(char*);
//         size_t read(char*, size_t);
//         std::shared_ptr<parseFile::iterator> begin();
//         size_t outerSize() const;
//         bool seekable() const;
//         parseFile(ioBase_sp&);
//         parseFile(ioBase_sp&, char*);
//         parseFile(ioBase_sp&, parseFile_sp&);
//         parseFile(ioBase_sp&, char*, parseFile_sp&);
//         parseFile(ioBase_sp&, off_t);
//         parseFile(ioBase_sp&, off_t, char*);
//         parseFile(ioBase_sp&, off_t, parseFile_sp&); // Why?
//         parseFile(ioBase_sp&, off_t, char*, parseFile_sp&); // Why?
// //         parseFile(const parseFile&);
// //         parseFile& operator=(const parseFile&);
// //         virtual ~parseFile();
//     protected:
//         void _update_bytes_read(size_t);
//         void _consume_remaining();
//     private:
//         // void _init(ioBase_sp&, off_t, char*);
//         void _init(ioBase_sp&);
//         void _init(ioBase_sp&, off_t);
//         ioBase_sp _file;
//         bool _seekable;
//         size_t bytes_read;
//         size_t _read_seekable(char*, size_t);
//         size_t _read_unseekable(char*, size_t);
//         void _cleanup();
//         unsigned long _itercount;
//     };
//
//     typedef std::shared_ptr<parseFile::iterator> iterParseFile_sp;
// //     std::unique_ptr<parseFileBase> parseFile(ioBase_sp&);
//
// //     struct iterParseFileState;
//
//     class parseFile::iterator {
//     public:
//         iterator(ioBase_sp& file);
//         iterator(ioBase_sp& file, off_t end);
//         iterator(ioBase_sp& file, off_t start, off_t end);
//         iterator(ioBase_sp& file, off_t end, parseFile_sp&);
//         iterator(ioBase_sp& file, off_t start, off_t end, parseFile_sp&);
//         parseFile_sp& operator*();
//         parseFile::iterator& operator++();
//         // parseFile::iterator operator++(int); Postincrement not supported
//         bool atEnd() const;
//         bool seekable() const;
//     private:
//         off_t _startoffset;
//         // off_t _offset;
//         // off_t _nextoffset;
//         off_t _endoffset;
//         ioBase_sp _file;
//         parseFile_sp _parent;
//         parseFile_sp _next;
//         bool _seekable;
//     };
//
// //     class iterParseFile {
// //         FILE* _file;
// //         unsigned long long _startoffset;
// //         unsigned long long _offset;
// //         unsigned long long _endoffset;
// //         parseFileData _next;
// //         char buffer[16];
// //     public:
// //         iterParseFile(void);
// //         iterParseFile(FILE*);
// //         iterParseFile(FILE*, unsigned long long);
// //     //     iterParseFile(FILE*, unsigned long long, unsigned long long);
// //         iterParseFile(
// //             FILE*, unsigned long long, unsigned long long,
// //             unsigned long long);
// //         parseFileData operator*(void);
// //         iterParseFile& operator++(void);
// //         iterParseFile operator++(int);
// //         bool operator==(const iterParseFile&) const;
// //         bool operator!=(const iterParseFile&) const;
// //     };
}
#endif
