#ifndef EBML_NG_IO_FD_CPP
#define EBML_NG_IO_FD_CPP

#include <unistd.h>
#include "libebml_ng/io.cpp"
#include "libebml_ng/exceptions.cpp"
// #include "libebml_ng/io/fd.h"
#include "libebml_ng/io/_fd.h"

namespace ebml {
    template<>
    bool io<int>::seekable() {
        off_t offset = ::lseek(this->_file, 0, SEEK_CUR);

        return (offset >= 0);
    }

    template<>
    off_t io<int>::_seek(off_t offset, int whence) {
        off_t new_offset = ::lseek(this->_file, offset, whence);

        if (new_offset < 0) {
            throw ebmlException("Seek Error", __LINE__, __FILE__);
        }

        return new_offset;
    }

    template<>
    off_t io<int>::tell() {
        off_t offset = ::lseek(this->_file, 0, SEEK_CUR);

        if (offset < 0) {
            throw ebmlException("Seek Error", __LINE__, __FILE__);
        }

        return offset;
    }

    template<>
    size_t io<int>::_read(char* dest, size_t size) {
        ssize_t bytes_read = ::read(this->_file, dest, size);

        if (bytes_read == -1) {
            throw ebmlException("Read Error", __LINE__, __FILE__);
        }

        return bytes_read;
    }

    template<>
    size_t io<int>::_write(const char* dest, size_t size) {
        ssize_t bytes_written = ::write(this->_file, dest, size);

        if (bytes_written == -1) {
            throw ebmlException("Write Error", __LINE__, __FILE__);
        }

        return bytes_written;
    }

    template<>
    size_t io<int>::read(char* dest, off_t offset, size_t size) {
        ssize_t bytes_read = ::pread(this->_file, dest, size, offset);

        if (bytes_read == -1) {
            throw ebmlException("Read Error", __LINE__, __FILE__);
        }

        return bytes_read;
    }

    template<>
    size_t io<int>::write(const char* src, off_t offset, size_t size) {
        ssize_t bytes_written = ::pwrite(this->_file, src, size, offset);

        if (bytes_written == -1) {
            throw ebmlException("Write Error", __LINE__, __FILE__);
        }

        return bytes_written;
    }
}

#endif
