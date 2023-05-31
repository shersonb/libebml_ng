#ifndef EBML_NG_IO_CPP
#define EBML_NG_IO_CPP

#include "libebml_ng/io.h"
#include <stdio.h>

namespace ebml {
    off_t ioBase::seek(off_t pos, int whence) {
        auto lock = this->acquireLock();
        off_t newpos = this->_seek(pos, whence);
        this->_pos = newpos;
        return newpos;
    }

    size_t ioBase::read(char* dest, size_t size) {
        auto lock = this->acquireLock();
        size_t bytes_read = this->_read(dest, size);
        this->_pos += bytes_read;
        return bytes_read;
    }

    size_t ioBase::write(const char* src, size_t size) {
        auto lock = this->acquireLock();
        size_t bytes_written = this->_write(src, size);
        this->_pos += bytes_written;
        return bytes_written;
    }

}

#endif
