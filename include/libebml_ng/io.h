#ifndef EBML_NG_IO_H
#define EBML_NG_IO_H

#include <mutex>
#include <memory>
#include <iostream>

namespace ebml {
    class ioBase {
    public:
        ioBase() : _pos(0), _mutex() {};
        ~ioBase();
        // Basic interface functions for file-like objects.
        size_t read(char*, size_t);
        size_t write(const char*, size_t);
        off_t seek(off_t, int whence=SEEK_SET);
        virtual bool seekable() = 0;
        off_t tell();

        // Additional read/write overloads that accept position argument.
        virtual size_t read(char*, off_t, size_t) = 0;
        virtual size_t write(const char*, off_t, size_t) = 0;
        void close();
        bool closed() const;

        // TODO
        // virtual size_t truncate() = 0;
        // virtual size_t truncate(off_t);
        // virtual bool readable() = 0;
        // virtual bool writable() = 0;

        // Lock,
        std::unique_lock<std::recursive_mutex> acquireLock() {
            return std::unique_lock<std::recursive_mutex>(this->_mutex);
        }
    protected:
        off_t _pos;
        bool _closed = 0;
        bool _close_on_dealloc = 0;

        // Custom implementations of read, write, and seek. When overriding, any
        // errors must be thrown as c++ exceptions.
        virtual size_t _read(char*, size_t) = 0;
        virtual size_t _write(const char*, size_t) = 0;
        virtual off_t _seek(off_t, int) = 0;
        virtual off_t _tell() = 0;
        virtual void _close() = 0;

        // Prohibit copy constructor and copy assignment.
        ioBase(const ioBase&);
        ioBase& operator=(const ioBase&);
    private:
        std::recursive_mutex _mutex;
    };

    typedef std::shared_ptr<ioBase> ioBase_sp;

    template<typename T>
    class io : public ioBase {
    public:
        io(T);
        // io(T&);
        // ~io();
        static ioBase_sp wrap(T);
        // static ioBase_sp wrap(T&);
        static ioBase_sp open(const std::string&, const std::ios_base::openmode&); // Static method for opening file from path, fstream-style.
        std::ios_base::openmode mode() const;
        bool seekable();

        // Read and write functions at specified offset.
        // These functions have a default implementation of seek-then-read/write.
        // Specializations may be defined, for example, to use pread and pseek for
        // file descriptors.
        size_t read(char*, off_t, size_t);
        size_t write(const char*, off_t, size_t);
        void close();
    private:
        T _file;
        static T _open(const std::string&, const std::ios_base::openmode&);
        size_t _read(char*, size_t);
        size_t _write(const char*, size_t);
        off_t _seek(off_t, int);
        void _close();
        off_t _tell();
        std::ios_base::openmode _mode;
    };
}

// #include "libebml_ng/io.tpp"
#endif
