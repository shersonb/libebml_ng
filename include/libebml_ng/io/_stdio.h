#ifndef EBML_NG_IO__STDIO_H
#define EBML_NG_IO__STDIO_H

#include "libebml_ng/io.h"
#include <stdio.h>

namespace ebml {
    template<>
    FILE* io<FILE*>::_open(const std::string& filename, const std::ios_base::openmode& mode);

    template<>
    void io<FILE*>::_close();

    template<>
    bool io<FILE*>::seekable();

    template<>
    off_t io<FILE*>::_seek(off_t, int);

    template<>
    off_t io<FILE*>::_tell();

    template<>
    size_t io<FILE*>::_read(char*, size_t);

    template<>
    size_t io<FILE*>::_write(const char*, size_t);

    template<>
    size_t io<FILE*>::_read(char*, size_t);

    template<>
    size_t io<FILE*>::_write(const char*, size_t);

    template<>
    struct stat io<FILE*>::stat();
}

#endif
