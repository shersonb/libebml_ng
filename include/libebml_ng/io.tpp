#ifndef EBML_NG_IO_TPP
#define EBML_NG_IO_TPP

#include <stdio.h>
#include "libebml_ng/io.h"

namespace ebml {
    template<typename T>
    io<T>::io(T file) : ioBase() {
        this->_file = file;
        if (this->seekable()) {
            this->_pos = this->tell();
        };
    }

    // Default implementation of io<T>::read(char*, off_t, size_t)
    template<typename T>
    size_t io<T>::read(char* dest, off_t offset, size_t size) {
        auto lock = this->acquireLock();
        this->seek(offset);
        return ioBase::read(dest, size);
    }

    // Default implementation of io<T>::write(const char*, off_t, size_t)
    template<typename T>
    size_t io<T>::write(const char* src, off_t offset, size_t size) {
        auto lock = this->acquireLock();
        this->seek(offset);
        return ioBase::write(src, size);
    }
}

#endif
