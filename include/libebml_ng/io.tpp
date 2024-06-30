#ifndef EBML_NG_IO_TPP
#define EBML_NG_IO_TPP

#include <stdio.h>
#include "libebml_ng/io.h"

namespace ebml {
    template<typename T>
    io<T>::io(const T& file) : ioBase() {
        this->_file = file;
        if (this->seekable()) {
            this->_pos = this->tell();
        };
    }

    template<typename T>
    ioBase_sp io<T>::open(const std::string& filename, const std::ios_base::openmode& __mode) {
        T file = io<T>::_open(filename, __mode);
        auto ret = new io<T>(file);
        ret->_mode = __mode;
        ret->_close_on_dealloc = false;
        return ioBase_sp(ret);
    }

    template<typename T>
    ioBase_sp io<T>::wrap(const T& file) {
        // return std::make_shared<io<T>>(file);
        return ptr(new io<T>(file));
    }

    template<typename T>
    std::ios_base::openmode io<T>::mode() const {
        return this->_mode;
    }

    // Default implementation of io<T>::read(char*, off_t, size_t)
    template<typename T>
    size_t io<T>::read(char* dest, off_t offset, size_t size) {
        auto lock = this->acquireLock();

        if (this->_pos != offset) {
            this->seek(offset);
        }

        return ioBase::read(dest, size);
    }

    // Default implementation of io<T>::write(const char*, off_t, size_t)
    template<typename T>
    size_t io<T>::write(const char* src, off_t offset, size_t size) {
        auto lock = this->acquireLock();

        if (this->_pos != offset) {
            this->seek(offset);
        }

        return ioBase::write(src, size);
    }
}
#endif
