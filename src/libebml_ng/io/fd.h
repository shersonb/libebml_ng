#ifndef EBML_NG_IO_FD_H
#define EBML_NG_IO_FD_H

#include "../io.h"

namespace ebml {
    template<>
    int io<int>::_open(const std::string& filename, const std::ios_base::openmode& mode);

    template<>
    void io<int>::_close();

    template<>
    bool io<int>::seekable();

    template<>
    off_t io<int>::_seek(off_t, int);

    template<>
    off_t io<int>::_tell();

    template<>
    size_t io<int>::_read(char*, size_t);

    template<>
    size_t io<int>::_write(const char*, size_t);

    template<>
    size_t io<int>::read(char*, off_t, size_t);

    template<>
    size_t io<int>::write(const char*, off_t, size_t);

    template<>
    struct stat io<int>::stat();

    template<>
    int io<int>::_open(const std::string& filename, const std::ios_base::openmode& mode);

    template<>
    void io<int>::_close();

    template<>
    bool io<int>::seekable();

    template<>
    off_t io<int>::_seek(off_t, int);

    template<>
    off_t io<int>::_tell();

    template<>
    size_t io<int>::_read(char*, size_t);

    template<>
    size_t io<int>::_write(const char*, size_t);

    template<>
    size_t io<int>::read(char*, off_t, size_t);

    template<>
    size_t io<int>::write(const char*, off_t, size_t);

    template<>
    struct stat io<int>::stat();

    template<>
    void io<int>::truncate();

    template<>
    void io<int>::truncate(off_t);

    /**
     * @brief Specialization of the io class for file descriptors.
     * @ingroup file_io
     *
     * Implements IO operations using POSIX system calls.
     */
    extern template class io<int>;
}

#endif
