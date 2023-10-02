#ifndef EBML_NG_IO_STDIO_CPP
#define EBML_NG_IO_STDIO_CPP

#include <stdexcept>

#include "libebml_ng/io.h"
#include "libebml_ng/io/_stdio.h"
#include "libebml_ng/io.tpp"

namespace ebml {
    template<>
    FILE* io<FILE*>::_open(const std::string& filename, const std::ios_base::openmode& __mode) {
        std::string modechars;
        FILE* ret;

        if (__mode & std::ios_base::app) {
            modechars = "a";

            if (__mode & std::ios_base::in) {
                modechars += "+";
            }
        } else if (__mode & std::ios_base::out) {
            if (__mode & std::ios_base::trunc) {
                modechars = "w+";
            } else if (__mode & std::ios_base::in) {
                modechars = "r+";
            } else {
                modechars = "w";
            }
        } else if (__mode & std::ios_base::in) {
            modechars = "r";
        } else {
            throw std::invalid_argument("Must specify read, write, or append flags.");
        }
        if (__mode & std::ios_base::binary) {
            modechars += "b";
        }
        ret = fopen(filename.c_str(), modechars.c_str());
        if (ret == nullptr) {
            throw std::ios_base::failure("Unable to open file: " + filename);
        }
        return ret;
    }

    template<>
    void io<FILE*>::_close() {
        fclose(this->_file);
    }

    template<>
    long io<FILE*>::_seek(long offset, int whence) {
        int result = fseek(this->_file, offset, whence);

        if (result != 0) {
            throw std::ios_base::failure("Seek Error");
        }

        return this->_tell();
    }

    template<>
    bool io<FILE*>::seekable() {
        long current_offset = ftell(this->_file);

        if (current_offset == -1L) {
            // If ftell() fails, the stream is not seekable
            return false;
        }

        int result = fseek(this->_file, 0L, SEEK_END);
        if (result != 0) {
            return false;
        }

        result = fseek(this->_file, current_offset, SEEK_SET);
        if (result != 0) {
            throw std::ios_base::failure("Failed to seek to original position.");
        }

        return true;
    }

    template<>
    long io<FILE*>::_tell() {
        long result = ftell(this->_file);

        if (result == -1L) {
            throw std::ios_base::failure("Tell Error");
        }

        return result;
    }

    template<>
    size_t io<FILE*>::_read(char* data, size_t count) {
        size_t result = fread(data, 1, count, this->_file);

        if (result == 0) {
            if (feof(this->_file)) {
                return 0;
            }

            throw std::ios_base::failure("Read Error");
        }

        return result;
    }

    template<>
    size_t io<FILE*>::_write(const char* data, size_t count) {
        size_t result = fwrite(data, 1, count, this->_file);

        if (result == 0) {
            throw std::ios_base::failure("Write Error");
        }

        return result;
    }

    // template class io<FILE*>;
}
#endif
