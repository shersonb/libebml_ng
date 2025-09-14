#ifndef EBML_NG_IO_FD_CPP
#define EBML_NG_IO_FD_CPP

#include <unistd.h>
#include <fcntl.h>
#include <bitset>
#include <cerrno>
#include <cstring>

#include "../io.h"
#include "../io.tpp"
// #include "libebml_ng/exceptions.cpp"
#include "fd.h"
// #include "libebml_ng/io/_fd.h"

namespace ebml {
    template<>
    int io<int>::_open(const std::string& filename, const std::ios_base::openmode& __mode) {
        int flags = O_CLOEXEC;
        int fileMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
        int ret = -1;

        if (__mode & std::ios_base::app) {
            flags |= O_APPEND | O_CREAT;
            fileMode |= S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

            if (__mode & std::ios_base::in) {
                flags |= O_RDWR;
            } else {
                flags |= O_WRONLY;
            }
        } else if (__mode & std::ios_base::out) {
            if (__mode & std::ios_base::trunc) {
                flags |= O_TRUNC | O_CREAT;
                fileMode |= S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
            }

            if (__mode & std::ios_base::in) {
                flags |= O_RDWR;
            } else {
                flags |= O_WRONLY;
            }
        } else if (__mode & std::ios_base::in) {
            flags |= O_RDONLY;
        } else {
            throw std::invalid_argument("Must specify read, write, or append flags.");
        }

        ret = ::open(filename.c_str(), flags, fileMode);

        if (ret == -1) {
            throw std::ios_base::failure("Unable to open file: " + filename);
        }

        return ret;
    }

    template<>
    void io<int>::_close() {
        ::close(this->_file);
    }

    template<>
    bool io<int>::seekable() {
        off_t offset = ::lseek(this->_file, 0, SEEK_CUR);

        return (offset >= 0);
    }

    template<>
    off_t io<int>::_seek(off_t offset, int whence) {
        off_t new_offset = ::lseek(this->_file, offset, whence);

        if (new_offset < 0) {
            throw std::ios_base::failure("Seek Error");
        }

        return new_offset;
    }

    template<>
    off_t io<int>::_tell() {
        off_t offset = ::lseek(this->_file, 0, SEEK_CUR);

        if (offset < 0) {
            throw std::ios_base::failure("Seek Error");
        }

        return offset;
    }

    template<>
    size_t io<int>::_read(char* dest, size_t size) {
        ssize_t bytes_read = ::read(this->_file, dest, size);

        if (bytes_read == -1) {
            std::cout << "Read Error: " << std::strerror(errno) << std::endl;
            throw std::ios_base::failure("Read Error");
        }

        return bytes_read;
    }

    template<>
    size_t io<int>::_write(const char* dest, size_t size) {
        ssize_t bytes_written = ::write(this->_file, dest, size);

        if (bytes_written == -1) {
            throw std::ios_base::failure("Write Error");
        }

        return bytes_written;
    }

    template<>
    size_t io<int>::read(char* dest, off_t offset, size_t size) {
        ssize_t bytes_read;

        if (!this->seekable()) {
            if (offset != this->_pos) {
                throw std::ios_base::failure("Read Error");
            }

            bytes_read = this->_read(dest, size);
            this->_pos += bytes_read;
            return bytes_read;
        }

        bytes_read = ::pread(this->_file, dest, size, offset);

        if (bytes_read == -1) {
            throw std::ios_base::failure("Read Error");
        }

        return bytes_read;
    }

    template<>
    size_t io<int>::write(const char* src, off_t offset, size_t size) {
        ssize_t bytes_written = ::pwrite(this->_file, src, size, offset);

        if (bytes_written == -1) {
            throw std::ios_base::failure("Write Error");
        }

        return bytes_written;
    }

    template<>
    void io<int>::truncate() {
        auto lock = this->acquireLock();
        auto pos = this->_tell();

        if (ftruncate(this->_file, pos) == -1) {
            throw std::ios_base::failure("Write Error");
        }
    }

    template<>
    void io<int>::truncate(off_t pos) {
        if (ftruncate(this->_file, pos) == -1) {
            throw std::ios_base::failure("Write Error");
        }
    }

    template<>
    struct stat io<int>::stat() {
        struct stat result;
        fstat(this->_file, &result);
        return result;
    }

    template class io<int>;
}

#endif
