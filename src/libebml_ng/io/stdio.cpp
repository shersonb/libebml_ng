#ifndef EBML_NG_IO_STDIO_CPP
#define EBML_NG_IO_STDIO_CPP

#include "libebml_ng/io.cpp"
#include "libebml_ng/exceptions.cpp"
#include "libebml_ng/io/_stdio.h"
#include <stdexcept>

namespace ebml {
    template<>
    long io<FILE*>::_seek(long offset, int whence) {
        int result = fseek(this->_file, offset, whence);

        if (result != 0) {
            throw ebmlException("Seek Error", __LINE__, __FILE__);
        }

        return this->tell();
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
            throw std::runtime_error("Failed to seek to original position.");
        }

        return true;
    }

    template<>
    long io<FILE*>::tell() {
        long result = ftell(this->_file);

        if (result == -1L) {
            throw ebmlException("Tell Error", __LINE__, __FILE__);
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

            throw ebmlException("Read Error", __LINE__, __FILE__);
        }

        return result;
    }

    template<>
    size_t io<FILE*>::_write(const char* data, size_t count) {
        size_t result = fwrite(data, 1, count, this->_file);

        if (result == 0) {
            throw ebmlException("Write Error", __LINE__, __FILE__);
        }

        return result;
    }
}
#endif
