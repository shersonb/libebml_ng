#ifndef EBML_NG_IO__FD_H
#define EBML_NG_IO__FD_H

#include "libebml_ng/io.h"

namespace ebml {
    template<>
    bool io<int>::seekable();

    template<>
    off_t io<int>::_seek(off_t, int);

    template<>
    off_t io<int>::tell();

    template<>
    size_t io<int>::_read(char*, size_t);

    template<>
    size_t io<int>::_write(const char*, size_t);

    template<>
    size_t io<int>::read(char*, off_t, size_t);

    template<>
    size_t io<int>::write(const char*, off_t, size_t);

    template class io<int>;
}

#endif
