#ifndef EBML_NG_IO_H
#define EBML_NG_IO_H

#include <mutex>

namespace ebml {
    class ioBase {
    public:
        ioBase() : _pos(0), _mutex() {};
        // Basic interface functions for file-like objects.
        size_t read(char*, size_t);
        size_t write(const char*, size_t);
        off_t seek(off_t, int whence=SEEK_SET);
        virtual bool seekable() = 0;
        virtual off_t tell() = 0;

        // Additional read/write overloads that accept position argument.
        virtual size_t read(char*, off_t, size_t) = 0;
        virtual size_t write(const char*, off_t, size_t) = 0;

        // TODO: Implement readable and writable.
        // virtual bool readable() = 0;
        // virtual bool writable() = 0;

        // Lock,
        std::unique_lock<std::recursive_mutex> acquireLock() {
            return std::unique_lock<std::recursive_mutex>(this->_mutex);
        }
    protected:
        off_t _pos;

        // Custom implementations of read, write, and seek. When overriding, any
        // errors must be thrown as c++ exceptions.
        virtual size_t _read(char*, size_t) = 0;
        virtual size_t _write(const char*, size_t) = 0;
        virtual off_t _seek(off_t, int) = 0;

        // Prohibit copy constructor and copy assignment.
        ioBase(const ioBase&);
        ioBase& operator=(const ioBase&);
    private:
        std::recursive_mutex _mutex;
    };

    template<typename T>
    class io : public ioBase {
    public:
        io(T);
        bool seekable();
        off_t tell();

        // Read and write functions at specified offset.
        // These functions have a default implementation of seek-then-read/write.
        // Specializations may be defined, for example, to use pread and pseek for
        // file descriptors.
        size_t read(char*, off_t, size_t);
        size_t write(const char*, off_t, size_t);
    private:
        T _file;
        size_t _read(char*, size_t);
        size_t _write(const char*, size_t);
        off_t _seek(off_t, int);
    };
}

#include "libebml_ng/io.tpp"
#endif
